/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief USB custom hid example.
 **
 **   - 2021-04-16  CDT   First version for USB custom hid device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "RD_V2_DeviceBSP.h"
extern const unsigned int PrintMotoHalfStep[8];
unsigned short g_ADCValue[3];
void vcard_control(uint8_t state)
{
    if (state == 1)
    {
        SYSCTRL_HWCTRL(0) |= 0x1f;
    }
    else if (state == 2)
    {
        SYSCTRL_HWCTRL(0) &= 0xef;
        SYSCTRL_HWCTRL(0) |= 0x8;
    }
    else
    {
        SYSCTRL_HWCTRL(0) &= 0xf7;
    }
}	




// byte0： uuid length
//byte1-16: uuid
// 49535343-FE78-4AE5-8FA9-9FAFD205E455
uint8_t ble_service_uuid_lsps[] = {0x10,0x55,0xe4,0x05,0xd2,0xaf,0x9f,0xa9,0x8f,0xe5,0x4a,0x7d,0xfe,0x43,0x53,0x53,0x49};

/*
byte0: characterisitic
byte1:characterisitic uuid length
byte2-17:characterisitic uuid
byte18:write/read payload length
byte19-20:write/read payload
*/
//49535343-1E4D-4BD9-BA61-23C647249616
uint8_t ble_Characteristic_uuid_lsps_tx[] = {0x10,0x10,0x16,0x96,0x24,0x47,0xc6,0x23,0x61,0xba,0xd9,0x4b,0x4d,0x1e,0x43,0x53,0x53,0x49,0x01,0x00};
//49535343-8841-43F4-A8D4-ECBE34729BB3
uint8_t ble_Characteristic_uuid_lsps_rx[] = {0x0c,0x10,0xb3,0x9b,0x72,0x34,0xbe,0xec,0xd4,0xa8,0xf4,0x43,0x41,0x88,0x43,0x53,0x53,0x49,0x01,0x00};
//49535343-aca3-481c-91ec-d85e28a60318
uint8_t ble_Characteristic_uuid_flow_ctrl[] = {0x18,0x10,0x18,0x03,0xa6,0x28,0x5e,0xd8,0xec,0x91,0x1c,0x48,0xa3,0xac,0x43,0x53,0x53,0x49,0x01,0x00};


#define IFLASH_NVRAM_ADDR 0x1000000+((512-4)*1024)
#define NVRAM_LEN 170//Nvram 长度170bytes,每组Nvram长度为34bytes,最多可存储5组配对信息

uint16_t ble_send_handle=0x2a;
//uint8_t bt_cod[3] = {0x24,0x04,0x04};

uint8_t bt_cod[3] = {0x80,0x06,0x04};

uint8_t NvramData[NVRAM_LEN]={0};


//write flash
uint8_t   ReserveNV(uint8_t* nvram);
extern tick SystickCount;

void BT_IRQHandler()
{
	while(IPC_have_data())
	{
		BT_ParseBTData();
	}

	BT_CONFIG &= (~(1<<BT_INIT_FLAG));
}

void SYSTICK_IRQHandler()
{
	SystickCount++;
	if(SystickCount>=TICK_MAX_VALUE)	SystickCount=0;
}



static uint8_t hex_lookup[16] = { '0', '1', '2', '3', '4', '5', '6', '7','8','9', 'a', 'b', 'c', 'd', 'e', 'f' };
static void array2hex(uint8_t* in, int inlen, uint8_t* out)
{
	int i;
	for (i = 0; i < inlen; ++i)
	{
		out[2 * i] = hex_lookup[in[inlen-1-i] >> 4];
		out[2 * i + 1] = hex_lookup[in[inlen-1-i] & 0x0f];
	}
}

void genBleAdvData(uint8_t *ble_name,int ble_name_len,uint8_t *ble_mac,uint8_t *ble_adv_data)
{
    int index=0;
    //property
    ble_adv_data[index++]=0x02;
    ble_adv_data[index++]=0x01;
    ble_adv_data[index++]=0x02;
    //name
    ble_adv_data[index++]=(ble_name_len+1);
    ble_adv_data[index++]=0x09;//name flag
    for(int i=0;i<ble_name_len;i++)
    {
        ble_adv_data[index++]=ble_name[i];
    }
    //manufacturer info
    ble_adv_data[index++]=0x09;
    ble_adv_data[index++]=0xff;//manufacturer info flag
    ble_adv_data[index++]=0x00;
    ble_adv_data[index++]=0x00;
    for(int i=0;i<6;i++)
    {
        ble_adv_data[index++]=ble_mac[5-i];
    }
    //fill
    while(index<31)
    {
        ble_adv_data[index++]=0x00;
    }
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void Power_off_test(void);

int BTSysInit(void)
{
	uint8_t bt_name[15] = "YP10";
	uint8_t bt_addr[6] =  {0x00,0x00,0x00,0x33,0x22,0x10};
	uint8_t chip_id[6]={0};
    uint8_t ble_adv_data[31]={0x02,0x01,0x02,0x00};
	uint16_t temp_handle;

	int UARTCMD = 0;
	uint8_t getbtstatus = 0;
	int i = 0;

	MyPrintf("bt ble demo\n");

	
	BT_Init();
	enable_intr(INTR_BT);

	read_chipid(chip_id);
	array2hex(chip_id,3,bt_name+8);
	bt_name[14]='\0';
	memcpy(bt_addr,chip_id,3);


	if(BT_SetBleName(bt_name,sizeof(bt_name)-1)==TRUE)
		MyPrintf("SetBleName_suc ble name:%s\n",bt_name);
	else
		MyPrintf("SetBleName_failed\n");

	if(BT_SetBtName(bt_name,sizeof(bt_name)-1) == TRUE)//bt与ble名字地址可以设置成一样
		MyPrintf("SetbtName_suc\n");
	else
		MyPrintf("SetbtName_fail\n");

	if(BT_SetBleAddr(bt_addr) == TRUE)
		MyPrintf("SetBleAddr_suc\n");
	else
		MyPrintf("SetBleAddr_fail\n");

	if(BT_SetBtAddr(bt_addr) == TRUE)
		MyPrintf("SetBtAddr_suc\n");
	else
		MyPrintf("SetBtAddr_fail\n");

	if(BT_SetParingMode(0x03) == TRUE)//设置配对模式为confirmkey
		MyPrintf("set confirmkey mode success\n");
	else
		MyPrintf("set confirmkey mode failed\n");

	if(BT_SetCOD(bt_cod) == TRUE) //设置COD
		MyPrintf("set COD sucess\n");
	else
		MyPrintf("set COD failed\n");

	if(BT_SetLpmMode(0x00) == TRUE) //设置低功耗模式
		MyPrintf("set lpm mode sucess\n");
	else
		MyPrintf("set lpm mode failed\n");


	if(BT_DeleteService() == TRUE) //  删除用户自定义服务
		MyPrintf("delete service sucess\n");
	else
		MyPrintf("delete service failed\n");

	temp_handle=BT_AddBleService(ble_service_uuid_lsps,sizeof(ble_service_uuid_lsps));
	if( temp_handle!= 0)  //增加服务 返回handle无需保存
		MyPrintf("add service sucess,handle=%04x\n",temp_handle);
	else
		MyPrintf("add service failed,return=%04x\n",temp_handle);

	ble_send_handle=BT_AddBleCharacteristic(ble_Characteristic_uuid_lsps_tx,sizeof(ble_Characteristic_uuid_lsps_tx));
	if( ble_send_handle!= 0)  //增加服务特征  write返回的handle需要保存，发数据使用
		MyPrintf("add Characteristic tx sucess,handle=%04x\n",ble_send_handle);
	else
		MyPrintf("add Characteristic tx failed,return=%04x\n",ble_send_handle);

	temp_handle=BT_AddBleCharacteristic(ble_Characteristic_uuid_lsps_rx,sizeof(ble_Characteristic_uuid_lsps_rx));
	if( temp_handle!= 0)
		MyPrintf("add Characteristic rx sucess;handle=%04x\n",temp_handle);
	else
		MyPrintf("add Characteristic rx failed,return=%04x\n",temp_handle);

	temp_handle=BT_AddBleCharacteristic(ble_Characteristic_uuid_flow_ctrl,sizeof(ble_Characteristic_uuid_flow_ctrl));
	if( temp_handle!= 0)
		MyPrintf("add Characteristic flow_ctrl sucess;handle=%04x\n",temp_handle);
	else
		MyPrintf("add Characteristic flow_ctrl failed,return=%04x\n",temp_handle);

    genBleAdvData(bt_name,sizeof(bt_name)-1,bt_addr,ble_adv_data);
    if(BT_SetLEAdvData(ble_adv_data,sizeof(ble_adv_data)) == TRUE)
		MyPrintf("BT_SetLEAdvData sucess\n");
	else
		MyPrintf("BT_SetLEAdvData failed\n");

    if(BT_SetLEScanData(ble_adv_data,sizeof(ble_adv_data)) == TRUE)
		MyPrintf("BT_SetLEScanData sucess\n");
	else
		MyPrintf("BT_SetLEScanData failed\n");

	if(BT_SetVisibility(0x01,0x01,0x01) == TRUE)  //设置可发现
		MyPrintf("SetVisibility sucess\n");
	else
		MyPrintf("SetVisibility failed\n");

	MyPrintf("bt version=%x\n",BT_GetVersion());

	qspi_flash_read(IFLASH_NVRAM_ADDR,NvramData,NVRAM_LEN);
	if(NvramData[34*0] > 0x05||NvramData[34*1] > 0x05||NvramData[34*2] > 0x05||NvramData[34*3] > 0x05||NvramData[34*4] > 0x05)
	{
		memset(NvramData,0x00,NVRAM_LEN);
	}
	i=0;
	if(BT_SetNVRAM(NvramData,NVRAM_LEN) == TRUE)
	{
		MyPrintf("set nvram success:\n");
		while(i<NVRAM_LEN)
			MyPrintf("0x%02X ",NvramData[i++]);
		MyPrintf("\n");
	}
	else
		MyPrintf("set nvram failed\n");

}

/**
  * @brief  deal with data from bt core event
  * @param  None
  * @retval None
  */
extern unsigned short BT_IRQ_ReceiveNBytes(unsigned char *mbuff,unsigned short mlen);

void BT_Progress()
{
	int eventCmd= BT_GetEventOpcode();
	switch(eventCmd)
	{
		case BT_DATA:
			{
				uint8_t i = 0;
				uint8_t btData[MAX_BLUETOOTH_DATA_LEN];
				int BtDataLen=0;
				BtDataLen=BT_ReadBTData(btData);
				if(BtDataLen>0)
				{
			//		MyPrintf("received %d bytes bt data:\n",BtDataLen);
					//for(int i=0;i<BtDataLen;i++)	MyPrintf("%02x ",btData[i]);
					//MyPrintf("\n");
					BT_IRQ_ReceiveNBytes(btData,BtDataLen);
					//btData[0]= 0xAA;
					//BT_SendSppData(btData,BtDataLen);
//					MyPrintf("send %d bytes bt data:\n",BtDataLen);
//					for(int i=0;i<BtDataLen;i++)	MyPrintf("%02x ",btData[i]);
//					MyPrintf("\n");
					#if (MEM_SPP_FLOWCTRL_FLAG_VALUE==0x01)
					if(BT_SetSppFlowcontrol(1) != TRUE)	MyPrintf("set Flowcontrol failed\n");
					#endif
				}
				else	MyPrintf("read bt data error\r\n");
			}
		break;

		case BLE_DATA:
			{
				uint8_t bleData[MAX_BLUETOOTH_DATA_LEN]={0};
				int BleDataLen=0;
				BleDataLen=BT_ReadBTData(bleData);
				if(BleDataLen>0)
				{
			//		MyPrintf("received %d bytes ble data[handle:%02x %02x]:\n",BleDataLen-2,bleData[0],bleData[1]);
		//	for(int i=0;i<(BleDataLen-2);i++)	MyPrintf("%02x ",bleData[2+i]);
			//		MyPrintf("\n");
					BT_IRQ_ReceiveNBytes(bleData,BleDataLen);
					//add handle
				//	bleData[0]=(ble_send_handle&0xff);
				//	bleData[1]=((ble_send_handle>>8)&0xff);
				//	BT_SendBleData(bleData,BleDataLen);
//					MyPrintf("send %d bytes ble data[%02x %02x]:\n",BleDataLen-2,bleData[0],bleData[1]);
//					for(int i=0;i<(BleDataLen-2);i++)	MyPrintf("%02x ",bleData[2+i]);
//					MyPrintf("\n");
				}
				else	MyPrintf("read ble data error\r\n");
			}
		break;

		case BT_CONNECTED:
			MyPrintf("bt connected\n");
		break;

		case BLE_CONNECTED:
			MyPrintf("ble connected\n");
		break;

		case BT_DISCONNECTED:
			MyPrintf("bt disconnected\n");
		break;

		case BLE_DISCONNECTED:
			MyPrintf("ble disconnected\n");
		break;

		case NVRAM_DATA:
			{
				uint8_t NvramData[MAX_BLUETOOTH_DATA_LEN];
				int NvramDataLen=0;
				NvramDataLen=BT_ReadBTData(NvramData);
				if(NvramDataLen>0)
				{
					MyPrintf("receive %dbytes Nvramdata:",NvramDataLen);
					for(int i=0;i<NvramDataLen;i++)
					{
						MyPrintf("%02X ",NvramData[i]);
					}
					MyPrintf("\n");
					ReserveNV(NvramData);//将最新的5个设备信息（NVRAM） 写入flash ,
				}
				else
					MyPrintf("read Nvram data error\r\n");
			}
		break;

		case CONFIRM_GKEY:
			{
				uint8_t GkeyData[MAX_BLUETOOTH_DATA_LEN];
				int GkeyDataLen=0;
				GkeyDataLen=BT_ReadBTData(GkeyData);
				if(GkeyDataLen>0)
				{
					MyPrintf("receive %dbytes CONFIRM_GKEY:",GkeyDataLen);
					for(int i=0;i<GkeyDataLen;i++)
					{
						MyPrintf("%02X ",GkeyData[i]);
					}
					MyPrintf("\n");
				}
				else	MyPrintf("read CONFIRM_GKEY data error\r\n");

				if(BT_ConfirmGkey(0) == TRUE)//BT_ConfirmGkey参数为0则继续配对，1则取消配对。
					MyPrintf("set confirmkey  success");
				else
					MyPrintf("set confirmkey  failed\n");
		}
		break;

		case PASSKEY:
			MyPrintf("request Passkey");
		break;

		case PAIRING_COMPLETED:
			{
				uint8_t PairingData[MAX_BLUETOOTH_DATA_LEN];
				int PairingDataLen=0;
				PairingDataLen=BT_ReadBTData(PairingData);
				if(PairingDataLen>0)
				{
					MyPrintf("receive %dbytes paring data:",PairingDataLen);
					for(int i=0;i<PairingDataLen;i++)
					{
						MyPrintf("%02X ",PairingData[i]);
					}
					MyPrintf("\n");
				}
				else	MyPrintf("read PAIRING_COMPLETED data error\r\n");
			}
		break;

		case PAIRING_STATE:
			{
				uint8_t PairingData[MAX_BLUETOOTH_DATA_LEN];
				int PairingDataLen=0;
				uint16_t pairing_status=0;
				PairingDataLen=BT_ReadBTData(PairingData);
				if(PairingDataLen==2)
				{
					pairing_status |=PairingData[0];
					pairing_status |=(PairingData[1]<<8);
					switch(pairing_status)
					{
						case 0x0001:
							MyPrintf("BT Pairing Success\r\n");
						break;

						case 0x0101:
							MyPrintf("BT Pairing fail\r\n");
						break;

						case 0x0080:
							MyPrintf("BLE Pairing Success\r\n");
						break;

						case 0x0180:
							MyPrintf("BLE Pairing fail\r\n");
						break;

						default:
							MyPrintf("unknown pairing_status[%04x]\r\n",pairing_status);
						break;
					}
				}
				else	MyPrintf("read PAIRING_STATE data error\r\n");
			}
		break;

		case -1:
			//no event
		break;

		default:
		{
			MyPrintf("unknown event:%02x\r\n",eventCmd);
			uint8_t unknownData[MAX_BLUETOOTH_DATA_LEN];
			int unknownDataLen=0;
			unknownDataLen=BT_ReadBTData(unknownData);
			if(unknownDataLen>0)
			{
				MyPrintf("datalen:%d\r\n",unknownDataLen);
				for(int i=0;i<unknownDataLen;i++)
				{
					MyPrintf("%02X ",unknownData[i]);
				}
				MyPrintf("\n");
			}
		}
		break;
	}
}

uint8_t   ReserveNV(uint8_t* nvram)
{
	MyPrintf("new nvram data,updata to flash\r\n");
	qspi_flash_sectorerase(IFLASH_NVRAM_ADDR);
	return qspi_flash_write(IFLASH_NVRAM_ADDR,nvram,NVRAM_LEN);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

		vcard_control(1);
	
    GPIO_InitStruct.GPIO_Pin = O_LEDG_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_LEDG_PORT, &GPIO_InitStruct);
		SET_H_O_LEDG;//拉高不输出

    GPIO_InitStruct.GPIO_Pin = I_LF_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(I_LF_PORT, &GPIO_InitStruct);

		//模拟输入
    GPIO_InitStruct.GPIO_Pin = A_TM_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(A_TM_PORT, &GPIO_InitStruct);	
	
	  GPIO_InitStruct.GPIO_Pin = A_LPS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(A_LPS_PORT, &GPIO_InitStruct);	
	
		//发射管
	  GPIO_InitStruct.GPIO_Pin = O_R6_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_R6_PORT, &GPIO_InitStruct);
	
		//BUSY
	  GPIO_InitStruct.GPIO_Pin = O_BUSY_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_BUSY_PORT, &GPIO_InitStruct);	
	
	
		//SWITCH
	  GPIO_InitStruct.GPIO_Pin = O_SWITCH_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_SWITCH_PORT, &GPIO_InitStruct);	
	
	
		//FLASH CS
	  GPIO_InitStruct.GPIO_Pin = O_MX25_CS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_MX25_CS_PORT, &GPIO_InitStruct);	
	
	
		//MOTO
	  GPIO_InitStruct.GPIO_Pin = O_MOTO1_PIN|O_MOTO2_PIN|O_MOTO3_PIN|O_MOTO4_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_MX25_CS_PORT, &GPIO_InitStruct);	
		
		
		//LAT
	  GPIO_InitStruct.GPIO_Pin = O_LAT_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_LAT_PORT, &GPIO_InitStruct);		
	
		//STB
	  GPIO_InitStruct.GPIO_Pin = O_STB1_PIN|O_STB2_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(O_STB1_PORT, &GPIO_InitStruct);			
	
}
void WDG_ResetCounter(void)
{
	
}


unsigned short g_pow_key_Cont = 0;

unsigned short ReadLFKeyPlus(void)
{
	unsigned short i,temp,key_Trg;
star_key:

	key_Trg = GET_I_KEY_LF;
	for(i=0;i<800;i++)
	{
		WDG_ResetCounter();
	}
	if(GET_I_KEY_LF != key_Trg)
	{
		goto star_key;
	}
	for(i=0;i<1000;i++)
	{
		WDG_ResetCounter();
	}
	if(GET_I_KEY_LF != key_Trg)
	{

		goto star_key;
	}

	temp = key_Trg^0xFFFF;// 1
	key_Trg = temp & (temp ^ g_pow_key_Cont);// 2
	g_pow_key_Cont = temp;// 3


	if(key_Trg & I_LF_PIN)
	{
		return 1;
	}
	return 0;
}

void SPIPrinter_init(void)
{
    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct.CPHA = SPI_CPHA_First_Edge;
    SPI_InitStruct.CPOL = SPI_CPOL_Low;
    SPI_InitStruct.Mode =  SPI_Mode_Master;
    SPI_InitStruct.RW_Delay = 5;
    GPIO_Config(O_CLK_PORT, O_CLK_PIN, SPID0_SCK);
    GPIO_Config(O_DI_PORT, O_DI_PIN, SPID0_MOSI);
    SPI_Init(SPI0, &SPI_InitStruct);
}


void SPI_Configuration(void)
{
    SPI_InitTypeDef SPI_InitStruct;

    GPIO_Config(O_MX25_SO_PORT, O_MX25_SO_PIN, SPID1_MISO);
    GPIO_Config(I_MX25_SI_PORT, I_MX25_SI_PIN, SPID1_MOSI);
    GPIO_Config(O_MX25_CS_PORT, O_MX25_CS_PIN, SPID1_NCS);
    GPIO_Config(O_MX25_SCK_PORT, O_MX25_SCK_PIN, SPID1_SCK);

    SPI_InitStruct.Mode = SPI_Mode_Master;
    SPI_InitStruct.BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct.CPHA = SPI_CPHA_First_Edge;
    SPI_InitStruct.CPOL = SPI_CPOL_Low;
    SPI_InitStruct.RW_Delay = 33;

    SPI_Init(SPI1, &SPI_InitStruct);
}

/**
  * @brief  Serial port 0 initialization function.
  * @param  None
  * @retval None
  */
void UART1_Configuration(unsigned int mBaud)
{
	UART_InitTypeDef UART_InitStruct;

	/* Configure serial ports 0 RX and TX for IO. */
	GPIO_Config(O_TXD1_PORT, O_TXD1_PIN, UART1_TXD);
	GPIO_Config(I_RXD1_PORT, I_RXD1_PIN, UART1_RXD);

	/* USARTx configured as follow:
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */

	UART_InitStruct.BaudRate = 115200;//mBaud;

	UART_InitStruct.DataBits = Databits_8b;
	UART_InitStruct.StopBits = StopBits_1;
	UART_InitStruct.Parity = Parity_None;
	UART_InitStruct.FlowCtrl = FlowCtrl_None;
	UART_InitStruct.Mode = Mode_duplex;

	UART_Init(UART1, &UART_InitStruct);

	UART_ITConfig(UART1, UART_IT_RX, ENABLE);
	UART_SetRxITNum(UART1, 10);
	UART_SetITTimeout(UART1, 0xff);	

	
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 0);	

}

/**
  * @brief  UART1 interrupt service function.
  * @param  None
  * @retval None
  */

extern void RS23_IRQ_DataRecive(void);
void UART1_IRQHandler(void)
{
    if (UART_GetITIdentity(UART1) == UART_IT_RX)
    {
//        MyPrintf("UART1 RX data:\n");
//        while(UART_IsRXFIFONotEmpty(UART1))
//        {
//            MyPrintf("%c",UART_ReceiveData(UART1));
//        }
			RS23_IRQ_DataRecive();
    }
//    else if (UART_GetITIdentity(UART1) == UART_IT_TX)
//    {
//        UART_ClearIT(UART1);
//        MyPrintf("UART1 TX INTERRUPT test successful!\n");
//    }
}




/**
  * @brief  Serial port 0 initialization function.
  * @param  None
  * @retval None
  */
void Debug_Configuration(void)
{
	UART_InitTypeDef UART_InitStruct;

	/* Configure serial ports 0 RX and TX for IO. */
	GPIO_Config(O_TXD0_PORT, O_TXD0_PIN, UART0_TXD);
	GPIO_Config(I_RXD0_PORT, I_RXD0_PIN, UART0_RXD);

	/* USARTx configured as follow:
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */

	UART_InitStruct.BaudRate = 115200;

	UART_InitStruct.DataBits = Databits_8b;
	UART_InitStruct.StopBits = StopBits_1;
	UART_InitStruct.Parity = Parity_None;
	UART_InitStruct.FlowCtrl = FlowCtrl_None;
	UART_InitStruct.Mode = Mode_duplex;

	UART_Init(UART0, &UART_InitStruct);
}



/**
  * @brief  TIM configuration function.
  * @param  None
  * @retval None
  */
void TIMER_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 48000;

    TIM_InitStruct.TIMx = TIM0;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM1;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM2;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM3;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM4;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM5;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM6;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM7;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM8;
    TIM_Init(&TIM_InitStruct);

    /* Configure timer for counting mode */
    TIM_ModeConfig(TIM0, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM1, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM2, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM3, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM4, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM5, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM6, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM7, TIM_Mode_TIMER);
    TIM_ModeConfig(TIM8, TIM_Mode_TIMER);

    /* The last step must be enabled */
    TIM_Cmd(TIM0, DISABLE);
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
    TIM_Cmd(TIM5, DISABLE);
    TIM_Cmd(TIM6, DISABLE);
    TIM_Cmd(TIM7, DISABLE);
    TIM_Cmd(TIM8, DISABLE);
		
		TIM_SetPeriod(TIM0,48000);
		
		
    NVIC_EnableIRQ(TIM0_IRQn);
    NVIC_SetPriority(TIM0_IRQn, 0);

    NVIC_EnableIRQ(TIM1_IRQn);
    NVIC_SetPriority(TIM1_IRQn, 0);

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);

    NVIC_EnableIRQ(TIM4_IRQn);
    NVIC_SetPriority(TIM4_IRQn, 0);

    NVIC_EnableIRQ(TIM5_IRQn);
    NVIC_SetPriority(TIM5_IRQn, 0);

    NVIC_EnableIRQ(TIM6_IRQn);
    NVIC_SetPriority(TIM6_IRQn, 0);

    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, 0);

    NVIC_EnableIRQ(TIM8_IRQn);
    NVIC_SetPriority(TIM8_IRQn, 0);		


		 TIM_Cmd(TIM0, ENABLE);
}


/**
  * @brief  TIM configuration function.
  * @param  None
  * @retval None
  */
void System_TIMER_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 48000;

    TIM_InitStruct.TIMx = TIM0;
    TIM_Init(&TIM_InitStruct);

    /* Configure timer for counting mode */
    TIM_ModeConfig(TIM0, TIM_Mode_TIMER);
	
			
    NVIC_EnableIRQ(TIM0_IRQn);
    NVIC_SetPriority(TIM0_IRQn, 2);
	
    /* The last step must be enabled */
 		 TIM_Cmd(TIM0, ENABLE);
}


void Printer_TIMER_Configuration(void)
{
    TIM_InitTypeDef TIM_InitStruct;

    TIM_InitStruct.period = 48000;

    TIM_InitStruct.TIMx = TIM1;
    TIM_Init(&TIM_InitStruct);

    TIM_InitStruct.TIMx = TIM2;
    TIM_Init(&TIM_InitStruct);	
	
    /* Configure timer for counting mode */
		TIM_ModeConfig(TIM1, TIM_Mode_TIMER);
		TIM_ModeConfig(TIM2, TIM_Mode_TIMER);
		
			
    NVIC_EnableIRQ(TIM1_IRQn);
    NVIC_SetPriority(TIM1_IRQn, 1);

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 2);
	
    /* The last step must be enabled */
 		 TIM_Cmd(TIM1, DISABLE);
		 TIM_Cmd(TIM2, DISABLE);
}



//按照us为基准进行设置
void EnableTimer(TIM_NumTypeDef TIMx,unsigned int mperiod)
{
	TIM_Cmd(TIMx, DISABLE);
	TIM_SetPeriod(TIMx,mperiod*48);
	TIM_Cmd(TIMx, ENABLE);
}

void DisableTimer(TIM_NumTypeDef TIMx)
{
	TIM_Cmd(TIMx, DISABLE);
}



/**
  * @brief  TIM0~TIM8 Interrupt service function.
  * @param  None
  * @retval None
  */
unsigned short g_ADContinueFlag;
extern void SysTick_IrqHandler(void);
extern void BT_Progress();
unsigned short g_BTCount;

unsigned short g_UsbStick;
extern void usb_last_packet(void);
extern int usb_get_buffer(uint8_t *buf);

extern unsigned short USB_IRQ_DataRecive(unsigned char *mbuff,unsigned short mlen);
uint8_t buf[65] = {0};
extern int buflen();
unsigned short g_big_buffer_have_space = 0;
unsigned int g_totaldatanum;

unsigned short g_ADValueCol[4];
unsigned short g_ADValueCount;


void TIMER0_IRQHandler(void)
{
	//1mS采样一次
	  uint8_t recvlen,i;
	unsigned int sum;
	if(g_ADContinueFlag == 0xA452)
	{
	g_ADValueCount++;
	g_ADValueCount &= 3;
	g_ADValueCol[g_ADValueCount] = ADC_GetOneVoltage(ADC_CHANNEL_4);
	sum = 0;
	for(i=0;i<4;i++)
	{
		sum += g_ADValueCol[i];
	}		
		
		
		g_ADCValue[0] = sum/4;
	}
	SysTick_IrqHandler();
	g_UsbStick++;
	if(g_UsbStick&2)
	{
		g_UsbStick = 0;
		usb_last_packet();

		recvlen = buflen();
		if (recvlen > 0)
		{
			if (g_big_buffer_have_space == 0)
			{
				/*receive data in to big buffer*/
				recvlen = usb_get_buffer(buf);
				g_totaldatanum += recvlen;
				g_big_buffer_have_space = USB_IRQ_DataRecive(buf,recvlen);
			}
		}
	}
	g_BTCount++;
	if(g_BTCount > 9)
	{
			BT_Progress();
		//------------------------------------------
	}
	//
}
extern void PrintRun_IrqHandler(void);
void TIMER1_IRQHandler(void)
{
	PrintRun_IrqHandler();
}
extern void Pro_LineData_IrqHandler(void);
void TIMER2_IRQHandler(void)
{
	Pro_LineData_IrqHandler();
}

void TIMER3_IRQHandler(void)
{

}

void TIMER4_IRQHandler(void)
{

}

void TIMER5_IRQHandler(void)
{

}

void TIMER6_IRQHandler(void)
{

}

void TIMER7_IRQHandler(void)
{

}

void TIMER8_IRQHandler(void)
{

}


void HVIN_ADC_Configuration(void)
{
	ADC_InitTypeDef ADCInitStruct;
	g_ADContinueFlag = 0;
	ADCInitStruct.ADC_Channel = ADC_CHANNEL_0;
	ADCInitStruct.ADC_Mode = ADC_HVIN;
	ADC_Init(&ADCInitStruct);
}

uint16_t  Get_HVIN_ADC_Value(void)
{
	uint16_t V = 0;
//	delay_ms(500);
	V = ADC_GetVoltage(ADC_CHANNEL_0);
	MyPrintf("ADC_Result:%d mV\n", V);
	return V;
}

/**
  * @brief  ADC initialization function.
  * @param  None
  * @retval None
  */
void GPIO_ADC1_Configuration(void)
{
	ADC_InitTypeDef ADCInitStruct;
	g_ADContinueFlag = 0;
	GPIO_Config(A_TM_PORT, A_TM_PIN, ANALOG);
	ADCInitStruct.ADC_Channel = ADC_CHANNEL_1;
	ADCInitStruct.ADC_Mode = ADC_GPIO;
	ADC_Init(&ADCInitStruct);
}

void GPIO_ADC4_Configuration(void)
{
	
	ADC_InitTypeDef ADCInitStruct;
	g_ADContinueFlag = 0xA452;
	GPIO_Config(A_LPS_PORT, A_LPS_PIN, ANALOG);
	ADCInitStruct.ADC_Channel = ADC_CHANNEL_4;
	ADCInitStruct.ADC_Mode = ADC_GPIO;
	ADC_Init(&ADCInitStruct);
}

uint16_t  Get_GPIO_ADC_Value(ADC_ChxTypeDef ADC_Channel)
{
	//return (ADC_GetResult(ADC_Channel) >>1)&1023;
	return ADC_GetOneVoltage(ADC_Channel);
}







void Power_off_test(void)
{
    uint8_t ret;

    if (TRUE == Power_Keyscan())
    {
        /* 如果power_key需要做独立按键可进行长短按设计 */
        delay_ms(1000);
		
        if (TRUE == Power_Keyscan())
        {
						
            MyPrintf("poweroff\n");
            /* 操作Power_off函数之前先进行关闭LCD等外设的操作 */
            ret = Power_off(); //如果插入外电，则不会关机，返回FALSE
					if(ret == FALSE)
					{
						 MyPrintf("poweroff failed\n");
					}
        }
    }
}


void SetMotoPWM(unsigned int value,unsigned short ParTime)
{
	if(value & O_MOTO1_PIN)
	{
		SET_H_O_MOTO1;
	}
	else
	{
		SET_L_O_MOTO1;
		
	}
	
	if(value & O_MOTO2_PIN)
	{
		SET_H_O_MOTO2;
	}
	else
	{
		SET_L_O_MOTO2;
		
	}
	
	
	if(value & O_MOTO3_PIN)
	{
		SET_H_O_MOTO3;
	}
	else
	{
		SET_L_O_MOTO3;

	}
	

	if(value & O_MOTO4_PIN)
	{
		SET_H_O_MOTO4;
	}
	else
	{
		SET_L_O_MOTO4;
		
	}
	
}

void SetMotoPWM_A(unsigned int value)
{
	if(value & O_MOTO1_PIN)
	{
		SET_H_O_MOTO1;
	}
	else
	{
		SET_L_O_MOTO1;
		
	}
	
	if(value & O_MOTO2_PIN)
	{
		SET_H_O_MOTO2;
	}
	else
	{
		SET_L_O_MOTO2;
		
	}
	
	
	if(value & O_MOTO3_PIN)
	{
		SET_H_O_MOTO3;
	}
	else
	{
		SET_L_O_MOTO3;
		
	}
	

	if(value & O_MOTO4_PIN)
	{
		SET_H_O_MOTO4;
	}
	else
	{
		SET_L_O_MOTO4;
		
	}
	
}


void ClearMoto(void)
{
	SET_L_O_MOTO1;
	SET_L_O_MOTO2;
	SET_L_O_MOTO3;
	SET_L_O_MOTO4;
}



void MOTORUN(unsigned short * p_g_MotoAll,unsigned short mRunStep)
{
	int i,j;
	for(i=0;i<mRunStep;i++)
	{
//		ClearMoto();
		WDG_ResetCounter(); //reload the watchdog 重装载看门狗定时器
		//SetMotoPWM(PrintMotoHalfStep[*p_g_MotoAll  & 7],100);
		SetMotoPWM_A(PrintMotoHalfStep[(*p_g_MotoAll)  & 7]);
		
		(*p_g_MotoAll)++;
		for(j=0;j<0x3000;j++);
		
		MyPrintf("%02X %02X ",g_ADCValue[0]&0xFF,(g_ADCValue[0] >>8)&0xFF );
		
	}
	ClearMoto();
}



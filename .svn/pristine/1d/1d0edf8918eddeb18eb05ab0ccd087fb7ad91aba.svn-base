/**
  ******************************************************************************
  * @file    3121Demo\ModuleDemo\BlueTooth\BT&BLE\user\main.c
  * @author  Yichip Application Team
  * @version V1.0.0
  * @date    18-Feb-2020
  * @brief   BT&BLE test program.
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 Yichip Microelectronics
  *
  * The purpose of this firmware is to provide guidance to customers engaged in
	* programming work to guide them smoothly to product development,
	* so as to save their time.
	*
	* Therefore, Yichip microelectronics shall not be responsible for any direct,
	* indirect or consequential damages caused by any content of this gu jiang
	* and/or contained in this code used by the customer in its products.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "yc3121.h"
#include "yc_gpio.h"
#include "yc_uart.h"
#include "yc_bt.h"
#include "yc_systick.h"
#include "yc_qspi.h"
#include "yc_ipc.h"
#include "board_config.h"
#include "yc_power.h"
#include "BTPro.h"


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
					for(int i=0;i<BtDataLen;i++)	MyPrintf("%02x ",btData[i]);
					MyPrintf("\n");

					btData[0]= 0xAA;
					BT_SendSppData(btData,BtDataLen);
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
					for(int i=0;i<(BleDataLen-2);i++)	MyPrintf("%02x ",bleData[2+i]);
					MyPrintf("\n");

					//add handle
					bleData[0]=(ble_send_handle&0xff);
					bleData[1]=((ble_send_handle>>8)&0xff);
					BT_SendBleData(bleData,BleDataLen);
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



/************************ (C) COPYRIGHT Yichip Microelectronics *****END OF FILE****/


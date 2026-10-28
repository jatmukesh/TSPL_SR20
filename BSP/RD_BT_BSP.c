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
 
#include "RD_BT_BSP.h"
#include "macros.h"

//----------------------------------------------------------------------
#ifdef BT_NACK
extern char *mediaType;
extern uint16_t label_length_mm;
extern void BT_NACK_Response_Media_And_Size(char *Media_Type,uint16_t Get_Media_Size);
#endif

#ifdef BT_CMD_REQ
extern uint16_t label_length_mm;
extern const char Get_Label_Size_cmd[];
extern void Get_Label_Size_BT_Response(uint16_t Media_Height);//void Get_Label_Size_BT_Response(char *Media_Type,uint16_t Media_Height);
extern int MyMemCmp(const void *ptr1, const void *ptr2, unsigned int len);
//unsigned int Length = 0;
#endif

//-----------------------------------------------------------------------

uint8_t chip_id[6]={0};

unsigned short g_Bt_Connect;
__align(32) const unsigned char CON_BT_Name_arry[128];
__align(32) const unsigned char CON_BT_KEY_arry[64];
/*
新建services数组格式如下：
byte0：service uuid length, 0x10即16个字节长度的uuid
byte1-16: uuid, 49535343-FE78-4AE5-8FA9-9FAFD205E455   
*/
uint8_t ble_service_uuid_test[] = {0x10,0x55,0xe4,0x05,0xd2,0xaf,0x9f,0xa9,0x8f,0xe5,0x4a,0x7d,0xfe,0x43,0x53,0x53,0x49};

/*
新建特征数组格式如下：
byte0: 特征的属性,即此uuid支持的功能。
    bit7-ExtendedProperites
    bit6-AuthenticatedSigned Writes
    bit5-Indicate
    bit4-Notify
    bit3-Write
    bit2-Write WithoutResponse
    bit1-Read
    bit0-Broadcast

byte1:characterisitic uuid length  
byte2-17:characterisitic uuid 
byte18:write/read payload length 
byte19-20:write/read payload
*/
//49535343-6daa-4d02-abf6-19569aca69fe
uint8_t ble_Characteristic_uuid_test_Wr[] = {0x08,0x10,0xfe,0x69,0xca,0x9a,0x56,0x19,0xf6,0xab,0x02,0x4d,0xaa,0x6d,0x43,0x53,0x53,0x49,0x01,0x00};
//49535343-1E4D-4BD9-BA61-23C647249616
uint8_t ble_Characteristic_uuid_test_tx[] = {0x10,0x10,0x16,0x96,0x24,0x47,0xc6,0x23,0x61,0xba,0xd9,0x4b,0x4d,0x1e,0x43,0x53,0x53,0x49,0x01,0x00};
//49535343-8841-43F4-A8D4-ECBE34729BB3
uint8_t ble_Characteristic_uuid_test_rx[] = {0x0c,0x10,0xb3,0x9b,0x72,0x34,0xbe,0xec,0xd4,0xa8,0xf4,0x43,0x41,0x88,0x43,0x53,0x53,0x49,0x01,0x00};
//49535343-aca3-481c-91ec-d85e28a60318
uint8_t ble_Characteristic_uuid_test_ctrl[] = {0x18,0x10,0x18,0x03,0xa6,0x28,0x5e,0xd8,0xec,0x91,0x1c,0x48,0xa3,0xac,0x43,0x53,0x53,0x49,0x01,0x00};
  
		
	
	
//ble ff000服务
uint8_t ble_service_uuid_lsps[] = {0x02,0x00,0xff};
uint8_t ble_Characteristic_uuid_lsps_tx[] = {0x10,0x02,0x01,0xff,0x01,0x00};
uint8_t ble_Characteristic_uuid_lsps_rx[] = {0x0c,0x02,0x02,0xff,0x01,0x00};
uint8_t ble_Characteristic_uuid_flow_ctrl[] = {0x10,0x02,0x3,0xff,0x01,0x01};   //流控的uuid


//ble FFF0服务
uint8_t ble_FFF0_service_uuid_lsps[] = {0x02,0xF0,0xFF};
uint8_t ble_FFF0_Characteristic_uuid_lsps_tx[] = {0x30,0x02,0xF1,0xff,0x01,0x00};
uint8_t ble_FFF0_Characteristic_uuid_lsps_rx[] = {0x0c,0x02,0xF2,0xff,0x01,0x00};


//ble fee7服务
uint8_t ble_FEE7_service_uuid_lsps[] = {0x02,0xE7,0xFE};

uint8_t ble_FEE7_Characteristic_uuid_lsps_rx[] = {0x08,0x02,0xC7,0xFE,0x01,0x00};

uint8_t ble_FEE7_Characteristic_uuid_lsps_In[] = {0x20,0x02,0xC8,0xFE,0x01,0x00};

uint8_t ble_FEE7_Characteristic_uuid_lsps_tx[] = {0x02,0x02,0xC9,0xFE,0x01,0x00};




#define IFLASH_NVRAM_ADDR 0x1000000+((512-4)*1024)
#define NVRAM_LEN 238//Nvram 长度170bytes,每组Nvram长度为34bytes,最多可存储5组配对信息

uint16_t tx_handle=0;
uint16_t rx_handle=0;
uint16_t flow_ctrl_handle=0;


uint16_t tx_handle_FF00=0;
uint16_t rx_handle_FF00=0;
uint16_t flow_ctrl_handle_FF00=0;


uint16_t Wr_handle_4953=0;
uint16_t tx_handle_4953=0;
uint16_t rx_handle_4953=0;
uint16_t flow_ctrl_handle_4953=0;



uint16_t tx_handle_FFF0=0;
uint16_t rx_handle_FFF0=0;

uint16_t In_handle_FEE7=0;
uint16_t tx_handle_FEE7=0;
uint16_t rx_handle_FEE7=0;


uint16_t tx_handle_CDD0=0;
uint16_t rx_handle_CDD0=0;

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

void SetBTName(unsigned char *Pbabuf)
{
	unsigned char mbuff[48];
	unsigned short i;
	mbuff[0] = 0xA5;
	mbuff[1] = 0x5A;
	mbuff[2] = 0x55;
	mbuff[3] = 0xAA;
	
	mbuff[4] = strlen(Pbabuf);
	if(mbuff[4] == 0)
	{
		mbuff[0] = 0xFF;
		mbuff[1] = 0xFF;
		mbuff[2] = 0xFF;
		mbuff[3] = 0xFF;
	}	
	i = 0;
	while(Pbabuf[i])
	{
		mbuff[i+5] = Pbabuf[i];
		i++;
	}
	
	enc_erase_flash_32byte((unsigned int)CON_BT_Name_arry);
	enc_write_flash((unsigned int)CON_BT_Name_arry, mbuff, 32);	
}

void ReadBTName(unsigned char *Pbabuf)
{
	enc_read_flash((unsigned int)CON_BT_Name_arry, Pbabuf,32);
}
	
void SetBT_KEY(unsigned char *Pbabuf)
{
	unsigned char mbuff[48];
	unsigned short i;
	mbuff[0] = 0xA5;
	mbuff[1] = 0x5A;
	mbuff[2] = 0x55;
	mbuff[3] = 0xAA;
	
	mbuff[4] = strlen(Pbabuf);
	if(mbuff[4] == 0)
	{
		mbuff[0] = 0xFF;
		mbuff[1] = 0xFF;
		mbuff[2] = 0xFF;
		mbuff[3] = 0xFF;
	}
		
	
	i = 0;
	while(Pbabuf[i])
	{
		mbuff[i+5] = Pbabuf[i];
		i++;
	}
		enc_erase_flash_32byte((unsigned int)CON_BT_KEY_arry);
		enc_write_flash((unsigned int)CON_BT_KEY_arry, mbuff, 32);	
}

void ReadBTKEY(unsigned char *Pbabuf)
{
	enc_read_flash((unsigned int)CON_BT_KEY_arry, Pbabuf,32);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
unsigned short g_ConnectFlag = 0;
uint8_t BT_KEY_arry[32];
void Power_off_test(void);
#ifdef BT_NAME_CHANGE		//BT name
uint8_t bt_name[15] = "PSF-20-";
#else
uint8_t bt_name[15] = "SR20-";
#endif
uint8_t BT_Name_arry[32];
uint8_t pin_code[5] = "0000";
unsigned char bt_addr[6] =  {0xBE,0xE0,0x0A,0x8C,0x47,0xC8};
/**
      * @FuncName  BTSysInit
      * @param     void
      * @retval    int - Boot initialization status (returns 0 or status code)
        * @LocalVar  uint8_t ble_adv_data[31] - BLE advertising and scan response payload structure
        *            uint16_t temp_handle      - Temporary service handle returned by BLE stack
        *            int UARTCMD               - Debug command variable holder
        *            uint8_t getbtstatus       - Bluetooth module hardware status query flag
        *            int i                     - Loop iterator index variable
        * @GlobalVar bt_name, bt_addr, chip_id, bt_cod, NvramData,
        *            tx_handle_FF00, rx_handle_FF00, flow_ctrl_handle_FF00,
        *            Wr_handle_4953, tx_handle_4953, rx_handle_4953, flow_ctrl_handle_4953,
        *            tx_handle_FFF0, rx_handle_FFF0, rx_handle_FEE7, In_handle_FEE7, tx_handle_FEE7
        * @Description Master Bluetooth Dual-Mode (BR/EDR Classic + BLE 4.2/5.0) system
*/
int BTSysInit(void)
{


    uint8_t ble_adv_data[31]={0x02,0x01,0x02,0x00};
	uint16_t temp_handle;

	int UARTCMD = 0;
	uint8_t getbtstatus = 0;
	int i = 0;


//	MyPrintf("bt ble demo\n");
	SysTick_Config(CPU_MHZ/100);
	
	BT_Init();
	enable_intr(INTR_BT);

	read_chipid(chip_id);
//	array2hex(chip_id,3,bt_name+5);
//	bt_name[14]='\0';
//	memcpy(bt_addr,chip_id,3);

#ifdef BT_NAME_CHANGE		
	array2hex(chip_id,3,bt_name+7);  //7 for BT name change to SR20- (i.e len = 5) to PSF-20- (i.e. Len = 7)
#else		
		array2hex(chip_id,3,bt_name+5);
#endif		
	bt_addr[0] = chip_id[0];
	bt_addr[1] = chip_id[1];
	bt_addr[2] = chip_id[2];
	bt_addr[3] = chip_id[3];
	bt_addr[4] = 0x47;
	bt_addr[5] = 0xC8;
		

#ifndef MY_DEBUG_ENABLE
	BT_SetBleName(bt_name,sizeof(bt_name)-1);
#else
	if(BT_SetBleName(bt_name,sizeof(bt_name)-1)==TRUE)
		MyPrintf("SetBleName_suc ble name:%s\n",bt_name);
	else
		MyPrintf("SetBleName_failed\n");
#endif

#ifndef MY_DEBUG_ENABLE
	BT_SetBtName(bt_name,sizeof(bt_name)-1);
#else
	if(BT_SetBtName(bt_name,sizeof(bt_name)-1) == TRUE)//bt与ble名字地址可以设置成一样
		MyPrintf("SetbtName_suc\n");
	else
		MyPrintf("SetbtName_fail\n");
#endif
	
#ifndef MY_DEBUG_ENABLE
	BT_SetBleAddr(bt_addr);
#else
	if(BT_SetBleAddr(bt_addr) == TRUE)
		MyPrintf("SetBleAddr_suc\n");
	else
		MyPrintf("SetBleAddr_fail\n");
#endif

#ifndef MY_DEBUG_ENABLE
	BT_SetBtAddr(bt_addr);
#else
	if(BT_SetBtAddr(bt_addr) == TRUE)
		MyPrintf("SetBtAddr_suc\n");
	else
		MyPrintf("SetBtAddr_fail\n");
#endif
			/*
			Mode 对应的配对模式
			0x00:pincode
			0x01:just work
			0x02:passkey
			0x03:confirm	
			*/	
	
	//---------------------------------------------------	

#ifndef MY_DEBUG_ENABLE
	BT_SetParingMode(0x01);
#else
	if(BT_SetParingMode(0x01) == TRUE)//设置配对模式为confirmkey
		MyPrintf("set confirmkey mode success\r\n");
	else
		MyPrintf("set confirmkey mode failed\r\n");	
#endif	
	//=====================================	
	
#ifndef MY_DEBUG_ENABLE
	BT_SetCOD(bt_cod);
#else
	if(BT_SetCOD(bt_cod) == TRUE) //设置COD
		MyPrintf("set COD sucess\n");
	else
		MyPrintf("set COD failed\n");
#endif

#ifndef MY_DEBUG_ENABLE
	BT_SetLpmMode(0x00);
#else
	if(BT_SetLpmMode(0x00) == TRUE) //设置低功耗模式
		MyPrintf("set lpm mode sucess\n");
	else
		MyPrintf("set lpm mode failed\n");
#endif

#ifndef MY_DEBUG_ENABLE
	BT_DeleteService();
#else
	if(BT_DeleteService() == TRUE) //  删除用户自定义服务
		MyPrintf("delete service sucess\n");
	else
		MyPrintf("delete service failed\n");
#endif

#ifndef MY_DEBUG_ENABLE
	temp_handle=BT_AddBleService(ble_service_uuid_lsps,sizeof(ble_service_uuid_lsps));
#else
	temp_handle=BT_AddBleService(ble_service_uuid_lsps,sizeof(ble_service_uuid_lsps));
	if( temp_handle!= 0)  //增加服务 返回handle无需保存
		MyPrintf("add service sucess,handle=%04x\n",temp_handle);
	else
		MyPrintf("add service failed,return=%04x\n",temp_handle);
#endif 
 //===============增加FF00服务 ======================================================================
    //增加FF00服务
#ifndef MY_DEBUG_ENABLE
	temp_handle=BT_AddBleService(ble_service_uuid_lsps,sizeof(ble_service_uuid_lsps));
#else
	temp_handle=BT_AddBleService(ble_service_uuid_lsps,sizeof(ble_service_uuid_lsps));
	if( temp_handle!= 0)  //增加服务 返回handle无需保存
		MyPrintf("add service sucess,handle=%04x\r\n",temp_handle);
	else
		MyPrintf("add service failed,return=%04x\r\n",temp_handle);
#endif

#ifndef MY_DEBUG_ENABLE
tx_handle_FF00=BT_AddBleCharacteristic(ble_Characteristic_uuid_lsps_tx,sizeof(ble_Characteristic_uuid_lsps_tx));
#else
	tx_handle_FF00=BT_AddBleCharacteristic(ble_Characteristic_uuid_lsps_tx,sizeof(ble_Characteristic_uuid_lsps_tx));
	if( tx_handle_FF00!= 0)  //增加服务特征  write返回的handle需要保存，发数据使用
		MyPrintf("add Characteristic tx sucess,handle=%04x\r\n",tx_handle_FF00);
	else
		MyPrintf("add Characteristic tx failed,return=%04x\r\n",tx_handle_FF00);
#endif

#ifndef MY_DEBUG_ENABLE
rx_handle_FF00=BT_AddBleCharacteristic(ble_Characteristic_uuid_lsps_rx,sizeof(ble_Characteristic_uuid_lsps_rx));
#else
	rx_handle_FF00=BT_AddBleCharacteristic(ble_Characteristic_uuid_lsps_rx,sizeof(ble_Characteristic_uuid_lsps_rx));
	if( rx_handle_FF00!= 0)
		MyPrintf("add Characteristic rx sucess;handle=%04x\r\n",rx_handle_FF00);
	else
		MyPrintf("add Characteristic rx failed,return=%04x\r\n",rx_handle_FF00);
#endif

#ifndef MY_DEBUG_ENABLE
flow_ctrl_handle_FF00=BT_AddBleCharacteristic(ble_Characteristic_uuid_flow_ctrl,sizeof(ble_Characteristic_uuid_flow_ctrl));
#else
	flow_ctrl_handle_FF00=BT_AddBleCharacteristic(ble_Characteristic_uuid_flow_ctrl,sizeof(ble_Characteristic_uuid_flow_ctrl));
	if( flow_ctrl_handle_FF00!= 0)
		MyPrintf("add Characteristic flow_ctrl sucess;handle=%04x\r\n",flow_ctrl_handle_FF00);
	else
		MyPrintf("add Characteristic flow_ctrl failed,return=%04x\r\n",flow_ctrl_handle_FF00);
#endif    
    //===============uuid, 49535343-FE78-4AE5-8FA9-9FAFD205E455  ======================================================================

#ifndef MY_DEBUG_ENABLE
temp_handle=BT_AddBleService(ble_service_uuid_test,sizeof(ble_service_uuid_test));
#else
    temp_handle=BT_AddBleService(ble_service_uuid_test,sizeof(ble_service_uuid_test));
	if( temp_handle!= 0)  //增加服务 返回handle无需保存
		MyPrintf("add service sucess,handle=%04x\r\n",temp_handle);
	else
		MyPrintf("add service failed,return=%04x\r\n",temp_handle);
#endif

#ifndef MY_DEBUG_ENABLE
Wr_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_Wr,sizeof(ble_Characteristic_uuid_test_Wr));
#else
	Wr_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_Wr,sizeof(ble_Characteristic_uuid_test_Wr));
	if( Wr_handle_4953!= 0)  //增加服务特征  write返回的handle需要保存，发数据使用
		MyPrintf("add Characteristic tx sucess,Wr_handle_4953=%04x\r\n",Wr_handle_4953);
	else
		MyPrintf("add Characteristic tx failed,Wr_handle_4953=%04x\r\n",Wr_handle_4953);
#endif

#ifndef MY_DEBUG_ENABLE
tx_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_tx,sizeof(ble_Characteristic_uuid_test_tx));
#else
	tx_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_tx,sizeof(ble_Characteristic_uuid_test_tx));
	if( tx_handle_4953!= 0)  //增加服务特征  write返回的handle需要保存，发数据使用
		MyPrintf("add Characteristic tx sucess,tx_handle_4953=%04x\r\n",tx_handle_4953);
	else
		MyPrintf("add Characteristic tx failed,tx_handle_4953=%04x\r\n",tx_handle_4953);
#endif

#ifndef MY_DEBUG_ENABLE
rx_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_rx,sizeof(ble_Characteristic_uuid_test_rx));
#else
	rx_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_rx,sizeof(ble_Characteristic_uuid_test_rx));
	if( rx_handle_4953!= 0)
		MyPrintf("add Characteristic rx sucess;rx_handle_4953=%04x\r\n",rx_handle_4953);
	else
		MyPrintf("add Characteristic rx failed,rx_handle_4953=%04x\r\n",rx_handle_4953);
#endif

#ifndef MY_DEBUG_ENABLE
flow_ctrl_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_ctrl,sizeof(ble_Characteristic_uuid_test_ctrl));
#else
	flow_ctrl_handle_4953=BT_AddBleCharacteristic(ble_Characteristic_uuid_test_ctrl,sizeof(ble_Characteristic_uuid_test_ctrl));
	if( flow_ctrl_handle_4953!= 0)
		MyPrintf("add Characteristic flow_ctrl sucess;flow_ctrl_handle_4953=%04x\r\n",flow_ctrl_handle_4953);
	else
		MyPrintf("add Characteristic flow_ctrl failed,flow_ctrl_handle_4953=%04x\r\n",flow_ctrl_handle_4953);	
#endif
//=============================FFF0========================================================
	
#ifndef MY_DEBUG_ENABLE
temp_handle=BT_AddBleService(ble_FFF0_service_uuid_lsps,sizeof(ble_FFF0_service_uuid_lsps));
#else
    temp_handle=BT_AddBleService(ble_FFF0_service_uuid_lsps,sizeof(ble_FFF0_service_uuid_lsps));
	if( temp_handle!= 0)  //增加服务 返回handle无需保存
		MyPrintf("add service sucess,handle=%04x\r\n",temp_handle);
	else
		MyPrintf("add service failed,return=%04x\r\n",temp_handle);
#endif

#ifndef MY_DEBUG_ENABLE
tx_handle_FFF0=BT_AddBleCharacteristic(ble_FFF0_Characteristic_uuid_lsps_tx,sizeof(ble_FFF0_Characteristic_uuid_lsps_tx));
#else
	tx_handle_FFF0=BT_AddBleCharacteristic(ble_FFF0_Characteristic_uuid_lsps_tx,sizeof(ble_FFF0_Characteristic_uuid_lsps_tx));
	if( tx_handle_FFF0!= 0)  //增加服务特征  write返回的handle需要保存，发数据使用
		MyPrintf("add Characteristic tx sucess,tx_handle_FFF0=%04x\r\n",tx_handle_FFF0);
	else
		MyPrintf("add Characteristic tx failed,tx_handle_FFF0=%04x\r\n",tx_handle_FFF0);
#endif

#ifndef MY_DEBUG_ENABLE
	rx_handle_FFF0=BT_AddBleCharacteristic(ble_FFF0_Characteristic_uuid_lsps_rx,sizeof(ble_FFF0_Characteristic_uuid_lsps_rx));
#else
	rx_handle_FFF0=BT_AddBleCharacteristic(ble_FFF0_Characteristic_uuid_lsps_rx,sizeof(ble_FFF0_Characteristic_uuid_lsps_rx));
	if( rx_handle_FFF0!= 0)
		MyPrintf("add Characteristic rx sucess;rx_handle_FFF0=%04x\r\n",rx_handle_FFF0);
	else
		MyPrintf("add Characteristic rx failed,rx_handle_FFF0=%04x\r\n",rx_handle_FFF0);
#endif	
	
	 //===============================FEE7======================================================
#ifndef MY_DEBUG_ENABLE
temp_handle=BT_AddBleService(ble_FEE7_service_uuid_lsps,sizeof(ble_FEE7_service_uuid_lsps));
#else
    temp_handle=BT_AddBleService(ble_FEE7_service_uuid_lsps,sizeof(ble_FEE7_service_uuid_lsps));
	if( temp_handle!= 0)  //增加服务 返回handle无需保存
		MyPrintf("add service sucess,handle=%04x\r\n",temp_handle);
	else
		MyPrintf("add service failed,return=%04x\r\n",temp_handle);
#endif

#ifndef MY_DEBUG_ENABLE
rx_handle_FEE7=BT_AddBleCharacteristic(ble_FEE7_Characteristic_uuid_lsps_rx,sizeof(ble_FEE7_Characteristic_uuid_lsps_rx));
#else
	rx_handle_FEE7=BT_AddBleCharacteristic(ble_FEE7_Characteristic_uuid_lsps_rx,sizeof(ble_FEE7_Characteristic_uuid_lsps_rx));
	if( rx_handle_FEE7!= 0)  //增加服务特征  write返回的handle需要保存，发数据使用
		MyPrintf("add Characteristic tx sucess,rx_handle_FEE7=%04x\r\n",rx_handle_FEE7);
	else
		MyPrintf("add Characteristic tx failed,rx_handle_FEE7=%04x\r\n",rx_handle_FEE7);
#endif

#ifndef MY_DEBUG_ENABLE
In_handle_FEE7=BT_AddBleCharacteristic(ble_FEE7_Characteristic_uuid_lsps_In,sizeof(ble_FEE7_Characteristic_uuid_lsps_In));
#else
	In_handle_FEE7=BT_AddBleCharacteristic(ble_FEE7_Characteristic_uuid_lsps_In,sizeof(ble_FEE7_Characteristic_uuid_lsps_In));
	if( In_handle_FEE7!= 0)
		MyPrintf("add Characteristic rx sucess;In_handle_FEE7=%04x\r\n",In_handle_FEE7);
	else
		MyPrintf("add Characteristic rx failed,In_handle_FEE7=%04x\r\n",In_handle_FEE7);
#endif

#ifndef MY_DEBUG_ENABLE
tx_handle_FEE7=BT_AddBleCharacteristic(ble_FEE7_Characteristic_uuid_lsps_tx,sizeof(ble_FEE7_Characteristic_uuid_lsps_tx));
#else
	tx_handle_FEE7=BT_AddBleCharacteristic(ble_FEE7_Characteristic_uuid_lsps_tx,sizeof(ble_FEE7_Characteristic_uuid_lsps_tx));
	if( tx_handle_FEE7!= 0)
		MyPrintf("add Characteristic flow_ctrl sucess;tx_handle_FEE7=%04x\r\n",tx_handle_FEE7);
	else
		MyPrintf("add Characteristic flow_ctrl failed,tx_handle_FEE7=%04x\r\n",tx_handle_FEE7);		
#endif	
	
	
//=====================================================================================

	genBleAdvData(bt_name,sizeof(bt_name)-1,bt_addr,ble_adv_data);
#ifndef MY_DEBUG_ENABLE
	BT_SetLEAdvData(ble_adv_data,sizeof(ble_adv_data));
#else	
	if(BT_SetLEAdvData(ble_adv_data,sizeof(ble_adv_data)) == TRUE)
		MyPrintf("BT_SetLEAdvData sucess\n");
	else
		MyPrintf("BT_SetLEAdvData failed\n");
#endif
	
#ifndef MY_DEBUG_ENABLE
	BT_SetLEScanData(ble_adv_data,sizeof(ble_adv_data));
#else
   if(BT_SetLEScanData(ble_adv_data,sizeof(ble_adv_data)) == TRUE)
		MyPrintf("BT_SetLEScanData sucess\n");
	else
		MyPrintf("BT_SetLEScanData failed\n");
#endif
	
	
#ifndef MY_DEBUG_ENABLE
		BT_SetVisibility(0x01,0x01,0x01);
#else	
	if(BT_SetVisibility(0x01,0x01,0x01) == TRUE)  //设置可发现
		MyPrintf("SetVisibility sucess\n");
	else
		MyPrintf("SetVisibility failed\n");
#endif	
	
#ifndef MY_DEBUG_ENABLE
	BT_ConfigMultiConnect(0x01);
#else	
    if(BT_ConfigMultiConnect(0x01) == TRUE)       //  设置是否开启多连接     0x00：单链接模式  0x01:多连接模式      默认为多连接模式
        MyPrintf("MultiMode Set sucess\n");
    else
        MyPrintf("MultiMode Set failed\n");
#endif
	
#ifndef MY_DEBUG_ENABLE    
//	MyPrintf("bt version=%x\n",BT_GetVersion());
#endif

qspi_flash_read(IFLASH_NVRAM_ADDR,NvramData,NVRAM_LEN);

//	MyPrintf("before set nvram:\n");
//					while(i<NVRAM_LEN)
//						MyPrintf("0x%02X ",NvramData[i++]);
//					MyPrintf("\n");

if(NvramData[34*0] > 0x05||NvramData[34*1] > 0x05||NvramData[34*2] > 0x05||NvramData[34*3] > 0x05||NvramData[34*4] > 0x05)
	{
		memset(NvramData,0x00,NVRAM_LEN);
	}
	i=0;	
#ifndef MY_DEBUG_ENABLE
	BT_SetNVRAM(NvramData,NVRAM_LEN);
#else	
	if(BT_SetNVRAM(NvramData,NVRAM_LEN) == TRUE)
	{
		MyPrintf("set nvram success:\n");
		while(i<NVRAM_LEN)
			MyPrintf("0x%02X ",NvramData[i++]);
		MyPrintf("\n");
	}
	else
		MyPrintf("set nvram failed\n");
#endif
}

/**
  * @brief  deal with data from bt core event
  * @param  None
  * @retval None
  */
extern unsigned short BT_IRQ_ReceiveNBytes(unsigned char *mbuff,unsigned short mlen);

unsigned short  g_BLE_RxProhandle=0xFFFF;
unsigned short  g_bt_handle,g_spp_handle;
void PrintBle_SPP_Send(unsigned char * mBuf,unsigned char mLen)
{
	unsigned char SendBuffer[64];
	unsigned char i,k;
	if( g_BLE_RxProhandle == 0xFFFF)
	{
		return;
	}	
	if(g_BLE_RxProhandle)
	{
		SendBuffer[0] = g_BLE_RxProhandle;
		SendBuffer[1] = g_BLE_RxProhandle>>8;
		
		i = 2;
		for(k=0;k<mLen;k++)
		{
			SendBuffer[i] = mBuf[k];
			i++;
		}
		BT_SendBleData(SendBuffer,mLen+2);
	}
	else
	{
		SendBuffer[0] = g_spp_handle;
		for(i=0;i<mLen;i++)
		{
			SendBuffer[i+1] = mBuf[i];
		}
		BT_SendSppData(SendBuffer,mLen+1);
	}
}

unsigned short PrintBleFCFlag=0;
unsigned short PrintSPPFCFlag=0;
unsigned short PrintBleFCOK=0;
void PrintBle_FC_Send(void)
{
	uint8_t bleData_send[MAX_BLUETOOTH_DATA_LEN]={0};
	if(g_BLE_RxProhandle == tx_handle_FF00)
	{	
	if(PrintBleFCOK == 0xA510)
	{
		if(BT_SendFineok() == FALSE)
		{
			PrintBleFCFlag++;
		}
		PrintBleFCOK = 0;
	}
	
	
	if(PrintBleFCFlag == 0)
	{
		return;
	}
	PrintBleFCFlag--;

			bleData_send[0]=(flow_ctrl_handle_FF00&0xff);
			bleData_send[1]=((flow_ctrl_handle_FF00>>8)&0xff);
			bleData_send[2]= 0x01;
			bleData_send[3]= 0x01;
		PrintBleFCOK = 0xA510;
		BT_SendBleData_one(bleData_send,4);
//			if(BT_SendBleData(bleData_send,4) == TRUE)
//			{
//				//发送成功
//				PrintBleFCFlag--;
//			}
	}

}

void PrintSPP_FC_Send(void)
{
	uint8_t bleData_send[MAX_BLUETOOTH_DATA_LEN]={0};
	if(PrintSPPFCFlag == 0)
	{
		return;
	}
	PrintSPPFCFlag--;
	if(g_BLE_RxProhandle == 0)
	{
		BT_SetSppFlowcontrol(g_bt_handle,1) ;
	}

}

extern unsigned int LID_Detection_Flag;

void BT_Progress(void)
{
		uint8_t btData[MAX_BLUETOOTH_DATA_LEN];
	int eventCmd= BT_GetEventOpcode();
	switch(eventCmd)
	{
		case BT_DATA:
			{
				uint8_t i = 0;
				//uint8_t btData[MAX_BLUETOOTH_DATA_LEN];
				int BtDataLen=0;
				BtDataLen=BT_ReadBTData(btData);
				//MyPrintf("BLEN:%d\r\n",BtDataLen);
				if(BtDataLen>0)
				{
					//MyPrintf("Tmp");
//					for(int i=0;i<BtDataLen;i++)	
//					{
//						MyPrintf("%02x ",btData[i]);
//					}
//					MyPrintf("\r\n");
#ifndef BT_CMD_REQ				
				//Length = BT_ReadBTData(btData);
				 // BT_NACK_Response_Media_And_Size(mediaType,label_length_mm); 
					MyPrintf("L:%d,SL:%d\r\n",BtDataLen,strlen(Get_Label_Size_cmd));
				if (((BtDataLen-1) == strlen(Get_Label_Size_cmd)) && (MyMemCmp(&btData[1], Get_Label_Size_cmd, strlen(Get_Label_Size_cmd)) == 0))
				{
					  MyPrintf("T1_LH:%d\r\n",label_length_mm);
						Get_Label_Size_BT_Response(label_length_mm);

				}
#endif
#ifdef LID_OPEN_MEDIA_ABSENT_AVOID_GARBAGE_PRINT				
					if(LID_Detection_Flag == 1)
					{
						MyPrintf("BL:%d ",BtDataLen);
							for(int i=0;i<BtDataLen;i++)	
							{
										MyPrintf("*114 ");
								 btData[i] = 0;
							}
							BtDataLen = 0;
							BT_IRQ_ReceiveNBytes(&btData[1],BtDataLen);
							#if (MEM_SPP_FLOWCTRL_FLAG_VALUE==0x01)
							
							if(BT_SetSppFlowcontrol(btData[0],1) != TRUE)	MyPrintf("set Flowcontrol failed\n");
							#endif
							LID_Detection_Flag = 0;

					}
					else
					{
						MyPrintf("*NN\r\n");
						g_bt_handle = btData[0];
						g_BLE_RxProhandle = 0;
						if(BT_IRQ_ReceiveNBytes(&btData[1],BtDataLen-1) == 0)
						{
							#if (MEM_SPP_FLOWCTRL_FLAG_VALUE==0x01)
							
							if(BT_SetSppFlowcontrol(btData[0],1) != TRUE)	MyPrintf("set Flowcontrol failed\n");
							#endif
						}
						else
						{
							PrintSPPFCFlag++;
						}
					}
#else					
		      
					  g_bt_handle = btData[0];
						g_BLE_RxProhandle = 0;
						if(BT_IRQ_ReceiveNBytes(&btData[1],BtDataLen-1) == 0)
						{
							#if (MEM_SPP_FLOWCTRL_FLAG_VALUE==0x01)
							
							if(BT_SetSppFlowcontrol(btData[0],1) != TRUE)	MyPrintf("set Flowcontrol failed\n");
							#endif
						}
						else
						{
							PrintSPPFCFlag++;
						}			
#endif					
				}
				else	MyPrintf("read bt data error\r\n");
			}
		break;
		
		case BLE_DATA:
			{
				uint8_t bleData_rec[MAX_BLUETOOTH_DATA_LEN]={0};
                uint8_t bleData_send[MAX_BLUETOOTH_DATA_LEN]={0};
				int BleDataLen_rec=0;
                int BleDataLen_send=0;
				BleDataLen_rec=BT_ReadBTData(bleData_rec);
	//							MyPrintf("bleData_rec---%02X-%02X\r\n",bleData_rec[0],bleData_rec[1]);
				if(BleDataLen_rec>0)
				{
						if ((flow_ctrl_handle_FF00+1) == (bleData_rec[1]*0x100+bleData_rec[0]))
						{
								//send package num
								//add handle
								bleData_send[0]=(flow_ctrl_handle_FF00&0xff);
								bleData_send[1]=((flow_ctrl_handle_FF00>>8)&0xff);
								bleData_send[2]= 0x01;
								bleData_send[3]= 0x07;
								BleDataLen_send = 4;
								BT_SendBleData(bleData_send,BleDataLen_send);
//								MyPrintf("1-send %d bytes ble data[%02x %02x]:\r\n",BleDataLen_send-2,bleData_send[0],bleData_send[1]);
//                        for(int i=0;i<(BleDataLen_send-2);i++)	MyPrintf("%02x ",bleData_send[2+i]);
//                        MyPrintf("\r\n");
								
								//send support mtu
								bleData_send[2]= 0x02;
								bleData_send[3]= 0xF0;
								bleData_send[4]= 0x00;
								BleDataLen_send = 5;
								BT_SendBleData(bleData_send,BleDataLen_send);
//								MyPrintf("2-send %d bytes ble data[%02x %02x]:\r\n",BleDataLen_send-2,bleData_send[0],bleData_send[1]);
//                        for(int i=0;i<(BleDataLen_send-2);i++)	MyPrintf("%02x ",bleData_send[2+i]);
//                        MyPrintf("\r\n");
#ifdef MY_DEBUG_ENABLE
								MyPrintf("FC1-\r\n");
#endif								
						}
						
						if (rx_handle_FF00 == (bleData_rec[1]*0x100+bleData_rec[0]))
						{
							g_BLE_RxProhandle = tx_handle_FF00;
//								bleData_send[0]=(flow_ctrl_handle_FF00&0xff);
//								bleData_send[1]=((flow_ctrl_handle_FF00>>8)&0xff);
//								bleData_send[2]= 0x01;
//								bleData_send[3]= 0x01;
//								BleDataLen_send = 4;
//								BT_SendBleData(bleData_send,BleDataLen_send);
#ifdef MY_DEBUG_ENABLE
							MyPrintf("RX1-\r\n");
#endif							
							if(BT_IRQ_ReceiveNBytes(&bleData_rec[2],BleDataLen_rec-2))
							{
									bleData_send[0]=(flow_ctrl_handle_FF00&0xff);
									bleData_send[1]=((flow_ctrl_handle_FF00>>8)&0xff);
									bleData_send[2]= 0x01;
									bleData_send[3]= 0x01;
									BleDataLen_send = 4;
									BT_SendBleData_one(bleData_send,BleDataLen_send);
									//PrintBleFCFlag = 0;

								
							}
							else
							{
								PrintBleFCFlag++;
							}
						//	UART_SendBuf(UART0,&bleData_rec[2],BleDataLen_rec-2);
						}
            
						if (rx_handle_4953 == (bleData_rec[1]*0x100+bleData_rec[0]))
						{
							g_BLE_RxProhandle = tx_handle_4953;
#ifdef MY_DEBUG_ENABLE							
							MyPrintf("rx_handle_4953-\r\n");
#endif							
							BT_IRQ_ReceiveNBytes(&bleData_rec[2],BleDataLen_rec-2);
						//	UART_SendBuf(UART0,&bleData_rec[2],BleDataLen_rec-2);
						}
						if (Wr_handle_4953 == (bleData_rec[1]*0x100+bleData_rec[0]))
						{
							g_BLE_RxProhandle = tx_handle_4953;
//							gsum += (BleDataLen_rec-2);
//							MyPrintf("rx_handle_4953-%d\r\n",gsum);
							BT_IRQ_ReceiveNBytes(&bleData_rec[2],BleDataLen_rec-2);
						//	UART_SendBuf(UART0,&bleData_rec[2],BleDataLen_rec-2);
						}
						if (rx_handle_FFF0 == (bleData_rec[1]*0x100+bleData_rec[0]))
						{
							g_BLE_RxProhandle = tx_handle_FFF0;
#ifdef MY_DEBUG_ENABLE							
							MyPrintf("rx_handle_FFF0-\r\n");
#endif							
							BT_IRQ_ReceiveNBytes(&bleData_rec[2],BleDataLen_rec-2);
						//	UART_SendBuf(UART0,&bleData_rec[2],BleDataLen_rec-2);
						}
						if (rx_handle_FEE7 == (bleData_rec[1]*0x100+bleData_rec[0]))
						{
							g_BLE_RxProhandle = tx_handle_FEE7;
#ifdef MY_DEBUG_ENABLE							
							MyPrintf("rx_handle_FEE7-\r\n");
#endif							
							BT_IRQ_ReceiveNBytes(&bleData_rec[2],BleDataLen_rec-2);
						//	UART_SendBuf(UART0,&bleData_rec[2],BleDataLen_rec-2);
						}
	

						
//					MyPrintf("4-received %d bytes ble data[handle:%02x %02x]:\r\n",BleDataLen_rec-2,bleData_rec[0],bleData_rec[1]);
//					for(int i=0;i<(BleDataLen_rec-2);i++)	MyPrintf("%02x ",bleData_rec[2+i]);
//					MyPrintf("\r\n");
				}
				else	MyPrintf("read ble data error\r\r\n");
			}		
			
		break;

				case BT_CONNECTED:
					g_Bt_Connect = 0xA1;
//            MyPrintf("bt connected\n"); 
//            {        
//                uint8_t i = 0;
//                uint8_t btData[MAX_BLUETOOTH_DATA_LEN];
                int BtDataLen=0;
							//MyPrintf("BLen:%d\r\n",BtDataLen);
              BtDataLen=BT_ReadBTData(btData);
				   //   MyPrintf("BLen:%d\r\n",BtDataLen);
//							for(int i=0;i<BtDataLen;i++)	
//							{
//								MyPrintf("%02x ",btData[i]);
//							}
//							MyPrintf("\r\n");
							g_spp_handle = btData[0];
#ifndef BT_CMD_REQ				
				//Length = BT_ReadBTData(btData);
				 // BT_NACK_Response_Media_And_Size(mediaType,label_length_mm); 
				if ((BtDataLen == strlen(Get_Label_Size_cmd)) && (MyMemCmp(btData, Get_Label_Size_cmd, strlen(Get_Label_Size_cmd)) == 0))
				{
					  MyPrintf("T1_LH:%d\r\n",label_length_mm);
						Get_Label_Size_BT_Response(label_length_mm);

				}
#endif				
//                if(BtDataLen>0)
//                {
//                    MyPrintf("received %d bytes bt data:\n",BtDataLen);
//                    for(int i=1;i<BtDataLen;i++)	MyPrintf("0x%02x ",btData[i]);
//                    MyPrintf("\n");
//                    MyPrintf("byte1==>connect device handle\n");
//                    MyPrintf("0x%02x\n ",btData[0]);
//                    MyPrintf("byte2-%d==>connect device mac addr\n",BtDataLen-1);
//                    for(int i=1;i<BtDataLen;i++)	MyPrintf("0x%02x ",btData[i]);
//                    MyPrintf("\n");
//                }
//                else	MyPrintf("read bt data error\r\n");
 //           }			
		break;

		case BLE_CONNECTED:
					g_Bt_Connect = 0xA1;
#ifdef MY_DEBUG_ENABLE		
            MyPrintf("ble connected\n");
#endif		
		break;

		case BT_DISCONNECTED:
			g_Bt_Connect = 0;
#ifdef MY_DEBUG_ENABLE		
			MyPrintf("bt disconnected\n");
#endif            
					{
                uint8_t i = 0;
                uint8_t btData[MAX_BLUETOOTH_DATA_LEN];
                int BtDataLen=0;
                BtDataLen=BT_ReadBTData(btData);
#ifdef MY_DEBUG_ENABLE							
                if(BtDataLen>0)
                {
                    MyPrintf("received %d bytes bt data:\n",BtDataLen);
                   for(int i=1;i<BtDataLen;i++)	MyPrintf("0x%02x ",btData[i]);
                    MyPrintf("\n");
                    
                    MyPrintf("byte==>disconnect device handle\n");
                    MyPrintf("0x%02x\n ",btData[0]);                   
                }
                else	MyPrintf("read bt data error\r\n");
#endif            
						}
		break;

		case BLE_DISCONNECTED:
			g_Bt_Connect = 0;
#ifdef MY_DEBUG_ENABLE		
			MyPrintf("ble disconnected\n");
#endif		
		break;

		case NVRAM_DATA:
			{
				uint8_t NvramData[MAX_BLUETOOTH_DATA_LEN];
				int NvramDataLen=0;
				NvramDataLen=BT_ReadBTData(NvramData);				
				if(NvramDataLen>0)                                        //This debug is set for check NV RAM data
				{
#ifdef MY_DEBUG_ENABLE					
					MyPrintf("receive %dbytes Nvramdata:",NvramDataLen);
					for(int i=0;i<NvramDataLen;i++)
					{
						MyPrintf("%02X ",NvramData[i]);
					}
					MyPrintf("\n");
					MyPrintf("go ReserveNV :\n");
#endif					
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
#ifdef MY_DEBUG_ENABLE				
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
#endif
				if(BT_ConfirmGkey(0) == TRUE)//BT_ConfirmGkey参数为0则继续配对，1则取消配对。
					MyPrintf("set confirmkey  success");
				else
					MyPrintf("set confirmkey  failed\n");
		}
		break;

		case PASSKEY:
#ifdef MY_DEBUG_ENABLE			
			MyPrintf("request Passkey");
#endif		
		break;

		case PAIRING_COMPLETED:
			{
				uint8_t PairingData[MAX_BLUETOOTH_DATA_LEN];
				int PairingDataLen=0;
				PairingDataLen=BT_ReadBTData(PairingData);
#ifdef MY_DEBUG_ENABLE				
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
#endif				
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
#ifdef MY_DEBUG_ENABLE			
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
#endif					
				}
				else	MyPrintf("read PAIRING_STATE data error\r\n");
			}
		break;

		case -1:
			//no event
		break;

		default:
		{
#ifdef MY_DEBUG_ENABLE				
			MyPrintf("unknown event:%02x\r\n",eventCmd);
#endif			
			uint8_t unknownData[MAX_BLUETOOTH_DATA_LEN];
			int unknownDataLen=0;
			unknownDataLen=BT_ReadBTData(unknownData);
#ifdef MY_DEBUG_ENABLE				
			if(unknownDataLen>0)
			{
				MyPrintf("datalen:%d\r\n",unknownDataLen);
				for(int i=0;i<unknownDataLen;i++)
				{
					MyPrintf("%02X ",unknownData[i]);
				}
				MyPrintf("\n");
			}
#endif			
		}
		break;
	}
}

uint8_t   ReserveNV(uint8_t* nvram)
{
#ifdef MY_DEBUG_ENABLE	
	MyPrintf("new nvram data,updata to flash\r\n");
#endif	
	qspi_flash_sectorerase(IFLASH_NVRAM_ADDR);
	return qspi_flash_write(IFLASH_NVRAM_ADDR,nvram,NVRAM_LEN);
}


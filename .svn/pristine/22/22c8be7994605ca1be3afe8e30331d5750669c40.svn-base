/************************ (C) COPYRIGHT YICHIP *************************
 * File Name            : usbd_printer_vcp.c
 * Author               : YICHIP
 * Version              : V1.0.0
 * Date                 : 21-May-2019
 * Description          : USB-OTG Core layer.
 *****************************************************************************/


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #pragma     data_alignment = 4
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_printer_vcp.h"
#include "usb_conf.h"
#include "usbd_printer_core.h"
#include "Misc.h"
#include "yc_timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LINE_CODING linecoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};


UART_InitTypeDef USART_InitStructure;

/* These are external variables imported from PRINTER core to be used for IN
   transfer management. */

#ifdef USER_SPECIFIED_DATA_SOURCE
    extern uint8_t  *APP_Rx_Buffer;
    uint8_t COM_Rx_data_buf[2048];
#else
    //extern uint8_t  APP_Rx_Buffer [];
    extern struct APP_DATA_STRUCT_DEF APP_Gdata_param;
#endif                            /* Write PRINTER received data in this buffer.
These data will be sent over USB IN endpoint
in the PRINTER core functions. */
//extern volatile uint32_t APP_Rx_ptr_in;
/* Increment this pointer or roll it back to
   start address when writing received data
   in the buffer APP_Rx_Buffer. */

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t *Buf, uint32_t Len);
//static uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx(uint8_t *Buf, uint32_t Len);

static uint16_t VCP_COMConfig(uint8_t Conf);

PRINTER_IF_Prop_TypeDef VCP_fops =
{
    VCP_Init,
    VCP_DeInit,
    VCP_Ctrl,
    VCP_DataTx,
    VCP_DataRx
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  VCP_Init
  *         Initializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Init(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;

    /**/
#ifdef USER_SPECIFIED_DATA_SOURCE
    APP_Rx_Buffer = COM_Rx_data_buf;
#else
    APP_Gdata_param.COM_config_cmp = 0;
#endif
    memset(&APP_Gdata_param, 0, sizeof(APP_Gdata_param));
    /* EVAL_COM1 default configuration */
    /* EVAL_COM1 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - Parity Odd
        - Hardware flow control disabled
        - Receive and transmit enabled
    */
//    USART_InitStructure.UART_BaudRate = 115200;
//    USART_InitStructure.UART_WordLength = UART_WordLength_8b;
//    USART_InitStructure.UART_StopBits = UART_StopBits_1;
//    USART_InitStructure.UART_Parity = UART_Parity_Odd;
//    UART_Init(UART1, &USART_InitStructure);

//    /* Enable the USART Receive interrupt */
//    UART_ITConfig(UART1, UART_IT_RX_RECVD, ENABLE);

//    /* Enable USART Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    return USBD_OK;
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_DeInit(void)
{

    return USBD_OK;
}


/**
  * @brief  VCP_Ctrl
  *         Manage the PRINTER class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result USBD_Status
  */
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t *Buf, uint32_t Len)
{
    switch (Cmd)
    {
    case SEND_ENCAPSULATED_COMMAND:
        /*get PRINTER class GET DEVICE ID */

        break;

    case GET_ENCAPSULATED_RESPONSE:
        /* Not  needed for this driver */
        break;

    case SET_COMM_FEATURE:
        /* Not  needed for this driver */
        break;

    case GET_COMM_FEATURE:
        /* Not  needed for this driver */
        break;

    case CLEAR_COMM_FEATURE:
        /* Not  needed for this driver */
        break;

    case SET_LINE_CODING:
        linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
        linecoding.format = Buf[4];
        linecoding.paritytype = Buf[5];
        linecoding.datatype = Buf[6];
        APP_Gdata_param.COM_config_cmp = 1;
        MyPrintf("cmp=%X\n", APP_Gdata_param.COM_config_cmp);
        MyPrintf("SET_LINE_CODING \n\r");
        /* Set the new configuration */
        VCP_COMConfig(OTHER_CONFIG);
        break;

    case GET_LINE_CODING:
        Buf[0] = (uint8_t)(linecoding.bitrate);
        Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
        Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
        Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
        Buf[4] = linecoding.format;
        Buf[5] = linecoding.paritytype;
        Buf[6] = linecoding.datatype;
        break;

    case SET_CONTROL_LINE_STATE:
        /* Not  needed for this driver */
        break;

    case SEND_BREAK:
        /* Not  needed for this driver */
        break;

    default:
        break;
    }

    return USBD_OK;
}

int isUsbBufFull()
{
    int ret = 0;
//  NVIC_DisableIRQ(USB_IRQn);
    ret = APP_Gdata_param.rx_structure.Rx_counter >= PRINTER_APP_RX_DATA_SIZE;
//  NVIC_EnableIRQ(USB_IRQn);
    return ret;
}
/**
  * @brief  VCP_DataTx
  *         PRINTER received data to be send over USB IN endpoint are managed in
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
uint16_t VCP_DataTx(uint8_t *Buf, uint32_t Len)
{
#ifdef USER_SPECIFIED_DATA_SOURCE
    uint32_t i = 0;
    for (i = 0; i < Len; i++)
    {
        *(APP_Rx_Buffer + APP_Rx_ptr_in) = *Buf++;
        APP_Rx_ptr_in++;
    }
#else
    uint32_t i = 0;
    if (linecoding.datatype == 7)
    {
        for (i = 0; i < Len; i++)
        {

            APP_Gdata_param.rx_structure.APP_Rx_Buffer[APP_Gdata_param.rx_structure.APP_Rx_ptr_in++] = (*Buf++) & 0x7F;
            APP_Gdata_param.rx_structure.Rx_counter++;
            /* To avoid buffer overflow */
            if (APP_Gdata_param.rx_structure.APP_Rx_ptr_in == PRINTER_APP_RX_DATA_SIZE)
            {
                APP_Gdata_param.rx_structure.APP_Rx_ptr_in = 0;
            }
        }
    }
    else if (linecoding.datatype == 8)
    {
        for (i = 0; i < Len; i++)
        {
            /* Detect whether the buffer overflow */
            APP_Gdata_param.rx_structure.APP_Rx_Buffer[APP_Gdata_param.rx_structure.APP_Rx_ptr_in++] = *Buf++;
            APP_Gdata_param.rx_structure.Rx_counter++;
            /* To avoid buffer overflow */
            if (APP_Gdata_param.rx_structure.APP_Rx_ptr_in == PRINTER_APP_RX_DATA_SIZE)
            {
                APP_Gdata_param.rx_structure.APP_Rx_ptr_in = 0;
            }
        }
    }
#endif
    return USBD_OK;
}

/**
  * @brief  Get_TxBuf_length
  *         Get the length of the remaining data to be transmitted
  * @param  NONE
  * @retval Result receive data length
  */
uint32_t VCP_GetTxBuflen(void)
{
    uint32_t ret = 0x00;
    if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out < APP_Gdata_param.rx_structure.APP_Rx_ptr_in)
        ret = APP_Gdata_param.rx_structure.APP_Rx_ptr_in - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
    else
        ret = PRINTER_APP_RX_DATA_SIZE + APP_Gdata_param.rx_structure.APP_Rx_ptr_in \
              - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
    return ret;
}

/**
  * @brief  Get_RxBuf_rsaddr
  *         Get reading receive data starting position.
  * @param  NONE
  * @retval Result received data is read starting position
  */
uint32_t VCP_GetTxBufrsaddr(void)
{
    return (APP_Gdata_param.rx_structure.APP_Rx_ptr_out);
}

/**
  * @brief  Get_RxData
  *         Get receive data by byte
  * @param  NONE
  * @retval Result receive data
  */
extern volatile uint8_t usbFlowCtrl;
extern volatile unsigned char APP_Tx_ptr_out_count;
uint8_t VCP_GetRxChar(void)
{
    uint8_t ret;
    ret = (APP_Gdata_param.tx_structure.APP_Tx_Buffer[APP_Gdata_param.tx_structure.APP_Tx_ptr_out++]);
    if (APP_Gdata_param.tx_structure.APP_Tx_ptr_out >= PRINTER_APP_TX_DATA_SIZE)
    {
        APP_Gdata_param.tx_structure.APP_Tx_ptr_out = 0;
        APP_Tx_ptr_out_count++;

    }
#if USB_FLOWCTRL
    if ((VCP_GetRxBuflen() + PRINTER_DATA_OUT_PACKET_SIZE) < PRINTER_APP_TX_DATA_SIZE)
    {
        if (usbFlowCtrl == 1)
        {
            usbFlowCtrl = 0;
//            USB_OTG_WRITE_REG8(CORE_USB_INT_MASK(0), 0xf0);
            NVIC_EnableIRQ(USB_IRQn);
        }
    }
#endif
    return ret;
}

/**
  * @brief  Get_RxBuf_length
  *         Get receive data length
  * @param  NONE
  * @retval Result receive data length
  */
extern volatile unsigned char APP_Tx_ptr_in_count;
uint32_t VCP_GetRxBuflen(void)
{
    if (APP_Tx_ptr_in_count >= APP_Tx_ptr_out_count)
    {
        return ((APP_Tx_ptr_in_count - APP_Tx_ptr_out_count) * APP_TX_DATA_SIZE + APP_Gdata_param.tx_structure.APP_Tx_ptr_in - APP_Gdata_param.tx_structure.APP_Tx_ptr_out);
    }
    else
    {
        return ((256 - APP_Tx_ptr_out_count - APP_Tx_ptr_in_count) * APP_TX_DATA_SIZE + APP_Gdata_param.tx_structure.APP_Tx_ptr_in - APP_Gdata_param.tx_structure.APP_Tx_ptr_out);
    }
}

/**
  * @brief  Get_RxBuf_rsaddr
  *         Get reading receive data starting position.
  * @param  NONE
  * @retval Result received data is read starting position
  */
int8_t *VCP_GetRxBufrsaddr(void)
{
    return (int8_t *)(&(APP_Gdata_param.tx_structure.APP_Tx_Buffer[APP_Gdata_param.tx_structure.APP_Tx_ptr_out]));
}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over PRINTER interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on PRINTER interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
volatile uint8_t out_packet_len = 0;
static uint16_t VCP_DataRx(uint8_t *Buf, uint32_t Len)
{
    uint32_t i;


    for (i = 0; i < Len; i++)
    {

        APP_Gdata_param.tx_structure.APP_Tx_Buffer[APP_Gdata_param.tx_structure.APP_Tx_ptr_in++] = Buf[i];
        if (APP_Gdata_param.tx_structure.APP_Tx_ptr_in >= PRINTER_APP_TX_DATA_SIZE)
        {
            APP_Gdata_param.tx_structure.APP_Tx_ptr_in = 0;
            APP_Tx_ptr_in_count++;
        }

        APP_Gdata_param.tx_structure.Tx_counter++;
    }
#if USB_FLOWCTRL
    if ((VCP_GetRxBuflen() + PRINTER_DATA_OUT_PACKET_SIZE) >= PRINTER_APP_TX_DATA_SIZE)
    {
        if (usbFlowCtrl == 0)
        {
            usbFlowCtrl = 1;
//            NVIC_DisableIRQ(USB_IRQn);
        }
    }
#endif
    return USBD_OK;
}



/**
  * @brief  VCP_COMConfig
  *         Configure the COM Port with default values or values received from host.
  * @param  Conf: can be DEFAULT_CONFIG to set the default configuration or OTHER_CONFIG
  *         to set a configuration received from the host.
  * @retval None.
  */
static uint16_t VCP_COMConfig(uint8_t Conf)
{

    return USBD_OK;
}

/**
  * @brief  VCP_ReadNextDataLen
  *
  * @param  None.
  * @retval None.
  */

void VCP_ReadNextDataLen(void)
{

    out_packet_len = 0;
    for (int i = 0; i < 500; i++)
    {
        if (USB_OTG_READ_REG8(CORE_USB_STATUS) & USB_STATUS_OUT(1))
        {
            out_packet_len = USB_OTG_READ_REG8(CORE_USB_EP_LEN(1));
            break;
        }
    }
    USB_OTG_WRITE_REG8(CORE_USB_STATUS, USB_STATUS_NAK);
    for (int i = 0; i < 5000; i++)
    {
        if (USB_OTG_READ_REG8(CORE_USB_STATUS) & USB_STATUS_NAK)
        {
            USB_OTG_WRITE_REG8(CORE_USB_STATUS, USB_STATUS_NAK);
            break;
        }
    }

//    USB_OTG_WRITE_REG8(CORE_USB_INT_MASK(0), 0xf2);
    NVIC_DisableIRQ(USB_IRQn);

}

/**
  * @brief  VCP_ReadNextDataLen
  *
  * @param  None.
  * @retval None.
  */
extern volatile uint8_t data_len;
uint8_t ReceiveDataLen = 0;
uint8_t VCP_GetReceiveDataLen(void *pdev, uint8_t epnum)
{
    ReceiveDataLen = ((USB_OTG_CORE_HANDLE *)pdev)->dev.out_ep[epnum].xfer_count;

    if ((out_packet_len != 64) && (out_packet_len != 0))
    {
        ReceiveDataLen  = out_packet_len;
        out_packet_len = 0;
    }
    return ReceiveDataLen;
}

/************************ (C) COPYRIGHT Yichip Microelectronics *****END OF FILE****/

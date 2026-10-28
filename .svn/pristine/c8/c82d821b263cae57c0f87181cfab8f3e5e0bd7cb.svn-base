
/* Includes ------------------------------------------------------------------*/
#include "usbd_printer_core.h"
#include "usbd_printer_vcp.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbd_usr.h"
#include "yc_timer.h"
#include "misc.h"
/** @defgroup usbd_printer
  * @brief usbd core module
  * @{
  */

/** @defgroup usbd_printer_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup usbd_printer_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup usbd_printer_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup usbd_printer_Private_FunctionPrototypes
  * @{
  */

/*********************************************
   PRINTER Device library callbacks
 *********************************************/
static uint8_t  usbd_printer_Init(void  *pdev, uint8_t cfgidx);
static uint8_t  usbd_printer_DeInit(void  *pdev, uint8_t cfgidx);
static uint8_t  usbd_printer_Setup(void  *pdev, USB_SETUP_REQ *req);
static uint8_t  usbd_printer_EP0_RxReady(void *pdev);
static uint8_t  usbd_printer_DataIn(void *pdev, uint8_t epnum);
static uint8_t  usbd_printer_DataOut(void *pdev, uint8_t epnum);
static uint8_t  usbd_printer_SOF(void *pdev);

/*********************************************
   PRINTER specific management functions
 *********************************************/
static void Handle_USBAsynchXfer(void *pdev);
static uint8_t  *USBD_printer_GetCfgDesc(uint8_t speed, uint16_t *length);
#ifdef USE_USB_OTG_HS
    static uint8_t  *USBD_printer_GetOtherCfgDesc(uint8_t speed, uint16_t *length);
#endif
/**
  * @}
  */

/** @defgroup usbd_printer_Private_Variables
  * @{
  */
extern PRINTER_IF_Prop_TypeDef  APP_FOPS;
extern uint8_t USBD_DeviceDesc   [USB_SIZ_DEVICE_DESC];

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t usbd_printer_CfgDesc  [USB_PRINTER_CONFIG_DESC_SIZ] __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t usbd_printer_OtherCfgDesc  [USB_PRINTER_CONFIG_DESC_SIZ] __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN static uint32_t  usbd_printer_AltSet  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USB_Rx_Buffer   [PRINTER_DATA_MAX_PACKET_SIZE] __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

#ifdef USER_SPECIFIED_DATA_SOURCE
    uint8_t *APP_Rx_Buffer = NULL;
#else
    //__ALIGN_BEGIN uint8_t APP_Rx_Buffer   [APP_RX_DATA_SIZE] __ALIGN_END ;
    struct APP_DATA_STRUCT_DEF APP_Gdata_param;
#endif

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t CmdBuff[PRINTER_CMD_PACKET_SZE] __ALIGN_END ;

//volatile uint32_t APP_Rx_ptr_in  = 0;
//volatile uint32_t APP_Rx_ptr_out = 0;
uint32_t APP_Rx_length  = 0;

uint8_t  USB_Tx_State = 0;

static uint32_t printerCmd = 0xFF;
static uint32_t printerLen = 0;

/* PRINTER interface class callbacks structure */
USBD_Class_cb_TypeDef  USBD_PRINTER_cb =
{
    usbd_printer_Init,
    usbd_printer_DeInit,
    usbd_printer_Setup,
    NULL,                 /* EP0_TxSent, */
    usbd_printer_EP0_RxReady,
    usbd_printer_DataIn,
    usbd_printer_DataOut,
    usbd_printer_SOF,
    NULL,
    NULL,
    USBD_printer_GetCfgDesc,
#ifdef USE_USB_OTG_HS
    USBD_printer_GetOtherCfgDesc, /* use same cobfig as per FS */
#endif /* USE_USB_OTG_HS  */
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB PRINTER device Configuration Descriptor */
__ALIGN_BEGIN uint8_t usbd_printer_CfgDesc[USB_PRINTER_CONFIG_DESC_SIZ]  __ALIGN_END =
{
    /*Configuation Descriptor*/
    0x09,   /* bLength: Configuation Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    USB_PRINTER_CONFIG_DESC_SIZ,       /* wTotalLength:no of returned bytes */
    0x00,

    0x01,   /* bNumInterfaces: 1 interface */

    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xc0,   /* bmAttributes: self powered */
    0x32,   /* MaxPower 100 mA */
    /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: One endpoints used */

    0x07,  //basic class for printer
    0x01,  //printer calll device

    0x02, // bi-directional interface.
    0x04,   /* iInterface: */

    /*Endpoint 3 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    PRINTER_OUT_EP,   /* bEndpointAddress: (OUT3) */
    0x02,   /* bmAttributes: Bulk */
    0x40,             /* wMaxPacketSize: */
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */
    /*Endpoint 1 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    PRINTER_IN_EP,   /* bEndpointAddress: (IN1) */
    0x02,   /* bmAttributes: Bulk */
    0x40,             /* wMaxPacketSize: */
    0x00,
    0x00    /* bInterval */
};

/* USB PRINTER device id Descriptor */
__ALIGN_BEGIN uint8_t print_id_descriptor[] __ALIGN_END =
{
    0x00,
    0x00,
    'p', 'r', 'i', 'n', 't', 'e', 'r', ';',
};

#ifdef USE_USB_OTG_HS
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined ( __ICCARM__ ) /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t usbd_printer_OtherCfgDesc[USB_PRINTER_CONFIG_DESC_SIZ]  __ALIGN_END =
{
};
#endif /* USE_USB_OTG_HS  */

/**
  * @}
  */

/** @defgroup usbd_printer_Private_Functions
  * @{
  */

/**
  * @brief  usbd_printer_Init
  *         Initilaize the PRINTER interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  usbd_printer_Init(void  *pdev,
                                  uint8_t cfgidx)
{
//  uint8_t *pbuf;

    /* Open EP IN */
    DCD_EP_Open(pdev,
                PRINTER_IN_EP,
                PRINTER_DATA_IN_PACKET_SIZE,
                USB_OTG_EP_BULK);

    /* Open EP OUT */
    DCD_EP_Open(pdev,
                PRINTER_OUT_EP,
                PRINTER_DATA_OUT_PACKET_SIZE,
                USB_OTG_EP_BULK);

    /* Open Command IN EP */
    DCD_EP_Open(pdev,
                PRINTER_CMD_EP,
                PRINTER_CMD_PACKET_SZE,
                USB_OTG_EP_INT);

//  pbuf = (uint8_t *)USBD_DeviceDesc;
//  pbuf[4] = DEVICE_CLASS_PRINTER;
//  pbuf[5] = DEVICE_SUBCLASS_PRINTER;

    /* Initialize the Interface physical components */
    APP_FOPS.pIf_Init();

    /* Prepare Out endpoint to receive next packet */
    DCD_EP_PrepareRx(pdev,
                     PRINTER_OUT_EP,
                     (uint8_t *)(USB_Rx_Buffer),
                     PRINTER_DATA_OUT_PACKET_SIZE);

    return USBD_OK;
}

/**
  * @brief  usbd_printer_Init
  *         DeInitialize the PRINTER layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  usbd_printer_DeInit(void  *pdev,
                                    uint8_t cfgidx)
{
    /* Open EP IN */
    DCD_EP_Close(pdev,
                 PRINTER_IN_EP);

    /* Open EP OUT */
    DCD_EP_Close(pdev,
                 PRINTER_OUT_EP);

    /* Open Command IN EP */
    DCD_EP_Close(pdev,
                 PRINTER_CMD_EP);

    /* Restore default state of the Interface physical components */
    APP_FOPS.pIf_DeInit();

    return USBD_OK;
}

/**
  * @brief  usbd_printer_Setup
  *         Handle the PRINTER specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  usbd_printer_Setup(void  *pdev,
                                   USB_SETUP_REQ *req)
{
    uint16_t len = USB_PRINTER_DESC_SIZ;
    uint8_t  *pbuf = usbd_printer_CfgDesc + 9;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    /* PRINTER Class Requests -------------------------------*/
    case USB_REQ_TYPE_CLASS :
        /* Check if the request is a data setup packet */
        if (req->wLength)
        {
            /* Check if the request is Device-to-Host */
            if (req->bmRequest & 0x80)
            {
                /* Get the data to be sent to Host from interface layer */
                //APP_FOPS.pIf_Ctrl(req->bRequest, CmdBuff, req->wLength);

                /* Send the data to the host */
                print_id_descriptor[0] = sizeof(print_id_descriptor) >> 8;
                print_id_descriptor[1] = sizeof(print_id_descriptor);
                USBD_CtlSendData(pdev,
                                 print_id_descriptor,
                                 sizeof(print_id_descriptor));
            }
            else /* Host-to-Device requeset */
            {
                /* Set the value of the current command to be processed */
                printerCmd = req->bRequest;
                printerLen = req->wLength;

                /* Prepare the reception of the buffer over EP0
                Next step: the received data will be managed in usbd_printer_EP0_TxSent()
                function. */
                //     MyPrintf("printer class \n\r");
                USBD_CtlPrepareRx(pdev,
                                  CmdBuff,
                                  req->wLength);
                APP_FOPS.pIf_Ctrl(req->bRequest, CmdBuff, req->wLength);
                if (req->wLength != 0)
                {
                    USBD_CtlSendStatus(pdev);
                }
            }
        }
        else /* No Data request */
        {
            /* Transfer the command to the interface layer */
            APP_FOPS.pIf_Ctrl(req->bRequest, NULL, 0);
        }

        return USBD_OK;

    default:
        USBD_CtlError(pdev, req);
        return USBD_FAIL;



    /* Standard Requests -------------------------------*/
    case USB_REQ_TYPE_STANDARD:
        switch (req->bRequest)
        {
        case USB_REQ_GET_DESCRIPTOR:
            if ((req->wValue >> 8) == PRINTER_DESCRIPTOR_TYPE)
            {
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
                pbuf = usbd_printer_Desc;
#else
                pbuf = usbd_printer_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
#endif
                len = MIN(USB_PRINTER_DESC_SIZ, req->wLength);
            }

            USBD_CtlSendData(pdev,
                             pbuf,
                             len);
            break;

        case USB_REQ_GET_INTERFACE :
            USBD_CtlSendData(pdev,
                             (uint8_t *)&usbd_printer_AltSet,
                             1);
            break;

        case USB_REQ_SET_INTERFACE :
            if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
            {
                usbd_printer_AltSet = (uint8_t)(req->wValue);
            }
            else
            {
                /* Call the error management function (command will be nacked */
                USBD_CtlError(pdev, req);
            }
            break;
        }
    }
    return USBD_OK;
}

/**
  * @brief  usbd_printer_EP0_RxReady
  *         Data received on control endpoint
  * @param  pdev: device device instance
  * @retval status
  */
static uint8_t  usbd_printer_EP0_RxReady(void  *pdev)
{
    if (printerCmd != NO_CMD)
    {
        /* Process the data */
        APP_FOPS.pIf_Ctrl(printerCmd, CmdBuff, printerLen);

        /* Reset the command variable to default value */
        printerCmd = NO_CMD;
    }

    return USBD_OK;
}

/**
  * @brief  usbd_audio_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_printer_DataIn(void *pdev, uint8_t epnum)
{
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length = 0;

    if (USB_Tx_State == 1)
    {
        if (APP_Rx_length == 0)
        {
            USB_Tx_State = 0;
        }
        else
        {
            if (APP_Rx_length > PRINTER_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_ptr = APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
                if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out + PRINTER_DATA_IN_PACKET_SIZE >= APP_RX_DATA_SIZE)
                {
                    USB_Tx_length = APP_RX_DATA_SIZE - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
                    APP_Gdata_param.rx_structure.APP_Rx_ptr_out = 0;
                    APP_Rx_length -= USB_Tx_length;
                    APP_Gdata_param.rx_structure.Rx_counter -= USB_Tx_length;
                }
                else
                {
                    USB_Tx_length = PRINTER_DATA_IN_PACKET_SIZE;
                    APP_Gdata_param.rx_structure.APP_Rx_ptr_out += PRINTER_DATA_IN_PACKET_SIZE;
                    APP_Rx_length -= PRINTER_DATA_IN_PACKET_SIZE;
                    APP_Gdata_param.rx_structure.Rx_counter -= PRINTER_DATA_IN_PACKET_SIZE;
                }
            }
            else
            {
                USB_Tx_ptr = APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
                if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out + APP_Rx_length >= APP_RX_DATA_SIZE)
                {
                    USB_Tx_length = APP_RX_DATA_SIZE - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
                    APP_Gdata_param.rx_structure.APP_Rx_ptr_out = 0;
                    APP_Rx_length -= USB_Tx_length;
                    APP_Gdata_param.rx_structure.Rx_counter -= USB_Tx_length;
                }
                else
                {
                    USB_Tx_length = APP_Rx_length;
                    APP_Gdata_param.rx_structure.APP_Rx_ptr_out += APP_Rx_length;
                    APP_Gdata_param.rx_structure.Rx_counter -= APP_Rx_length;
                    APP_Rx_length = 0;
                }
            }
        }
        /* Prepare the available data buffer to be sent on IN endpoint */

        DCD_EP_Tx(pdev,
                  PRINTER_IN_EP,
                  (uint8_t *)&APP_Gdata_param.rx_structure.APP_Rx_Buffer[USB_Tx_ptr],
                  USB_Tx_length);
    }

    return USBD_OK;
}

/**
  * @brief  usbd_printer_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
extern uint8_t usbFlowCtrl;
extern uint32_t total_len;
extern volatile uint8_t out_packet_len;
static uint8_t  usbd_printer_DataOut(void *pdev, uint8_t epnum)
{
    uint16_t USB_Rx_Cnt;

    /* Get the received data buffer and update the counter */

    /* USB data will be immediately processed, this allow next USB traffic being
       NAKed till the end of the application Xfer */
    /* Prepare Out endpoint to receive next packet */

    DCD_EP_PrepareRx(pdev,
                     PRINTER_OUT_EP,
                     (uint8_t *)(USB_Rx_Buffer),
                     PRINTER_DATA_OUT_PACKET_SIZE);

#if USB_FLOWCTRL

    USB_Rx_Cnt = VCP_GetReceiveDataLen(pdev, epnum);

    APP_FOPS.pIf_DataRx(USB_Rx_Buffer, USB_Rx_Cnt);
#else
    USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE *)pdev)->dev.out_ep[epnum].xfer_count;
    total_len = total_len + USB_Rx_Cnt;
    MyPrintf("total_len = %d USB_Rx_Cnt = %d \n", total_len, USB_Rx_Cnt);
#endif

    return USBD_OK;
}

/**
  * @brief  usbd_audio_SOF
  *         Start Of Frame event management
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_printer_SOF(void *pdev)
{
    static uint32_t FrameCount = 0;

    if (FrameCount++ == PRINTER_IN_FRAME_INTERVAL)
    {
        /* Reset the frame counter */
        FrameCount = 0;

        /* Check the data to be sent through IN pipe */
        Handle_USBAsynchXfer(pdev);
    }

    return USBD_OK;
}

/**
  * @brief  Handle_USBAsynchXfer
  *         Send data to USB
  * @param  pdev: instance
  * @retval None
  */
static void Handle_USBAsynchXfer(void *pdev)
{
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length;

    if (USB_Tx_State != 1)
    {
        if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out == PRINTER_APP_RX_DATA_SIZE)
        {
            APP_Gdata_param.rx_structure.APP_Rx_ptr_out = 0;
        }
#if 1
        if (!APP_Gdata_param.rx_structure.Rx_counter)
        {
            USB_Tx_State = 0;
            return;
        }
        APP_Rx_length = APP_Gdata_param.rx_structure.Rx_counter;
#else
        /* */
        if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out == \
                APP_Gdata_param.rx_structure.APP_Rx_ptr_in)
        {
            USB_Tx_State = 0;
            return;
        }

        if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out > APP_Gdata_param.rx_structure.APP_Rx_ptr_in) /* rollback */
        {
            APP_Rx_length = APP_RX_DATA_SIZE - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
        }
        else
        {
            APP_Rx_length = APP_Gdata_param.rx_structure.APP_Rx_ptr_in - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
        }
#endif
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        APP_Rx_length &= ~0x03;
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

        if (APP_Rx_length > PRINTER_DATA_IN_PACKET_SIZE)
        {
            USB_Tx_ptr = APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
            if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out + PRINTER_DATA_IN_PACKET_SIZE >= PRINTER_APP_RX_DATA_SIZE)
            {
                USB_Tx_length = APP_RX_DATA_SIZE - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
                APP_Gdata_param.rx_structure.APP_Rx_ptr_out = 0;
                APP_Rx_length -= USB_Tx_length;
                APP_Gdata_param.rx_structure.Rx_counter = APP_Rx_length;
            }
            else
            {
                USB_Tx_length = PRINTER_DATA_IN_PACKET_SIZE;
                APP_Gdata_param.rx_structure.APP_Rx_ptr_out += PRINTER_DATA_IN_PACKET_SIZE;
                APP_Rx_length -= PRINTER_DATA_IN_PACKET_SIZE;
                APP_Gdata_param.rx_structure.Rx_counter = APP_Rx_length;
            }
        }
        else
        {
            USB_Tx_ptr = APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
            if (APP_Gdata_param.rx_structure.APP_Rx_ptr_out + APP_Rx_length >= PRINTER_APP_RX_DATA_SIZE)
            {
                USB_Tx_length = APP_RX_DATA_SIZE - APP_Gdata_param.rx_structure.APP_Rx_ptr_out;
                APP_Gdata_param.rx_structure.APP_Rx_ptr_out = 0;
                APP_Rx_length -= USB_Tx_length;
                APP_Gdata_param.rx_structure.Rx_counter = APP_Rx_length;
            }
            else
            {
                USB_Tx_length = APP_Rx_length;
                APP_Gdata_param.rx_structure.APP_Rx_ptr_out += APP_Rx_length;
                APP_Rx_length = 0;
                APP_Gdata_param.rx_structure.Rx_counter = APP_Rx_length;
            }
        }
        USB_Tx_State = 1;

        DCD_EP_Tx(pdev,
                  PRINTER_IN_EP,
                  (uint8_t *)&APP_Gdata_param.rx_structure.APP_Rx_Buffer[USB_Tx_ptr],
                  USB_Tx_length);
    }
}

/**
  * @brief  USBD_printer_GetCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_printer_GetCfgDesc(uint8_t speed, uint16_t *length)
{
    *length = sizeof(usbd_printer_CfgDesc);
    return usbd_printer_CfgDesc;
}

/**
  * @brief  USBD_printer_GetCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
#ifdef USE_USB_OTG_HS
static uint8_t  *USBD_printer_GetOtherCfgDesc(uint8_t speed, uint16_t *length)
{
    *length = sizeof(usbd_printer_OtherCfgDesc);
    return usbd_printer_OtherCfgDesc;
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Yichip Microelectronics *****END OF FILE****/

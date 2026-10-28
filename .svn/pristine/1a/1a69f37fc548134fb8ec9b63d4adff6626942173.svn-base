/************************ (C) COPYRIGHT YICHIP *************************
 * File Name            : usb_core.c
 * Author               : YICHIP
 * Version              : V1.0.0
 * Date                 : 21-May-2019
 * Description          : USB-OTG Core layer.
 *****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "usb_dcd_int.h"
#include "usbd_printer_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "misc.h"
#include "usbd_printer_vcp.h"
#include "yc_timer.h"
#include "usb_defines.h"

USB_OTG_CORE_HANDLE  USB_OTG_dev;
volatile unsigned char APP_Tx_ptr_in_count = 0, APP_Tx_ptr_out_count = 0;
uint32_t total_len;
void usb_reg_init()
{
    USB_OTG_WRITE_REG8(CORE_USB_CONFIG, 0x00);
    my_delay_ms(1);
    USB_OTG_WRITE_REG8(CORE_USB_TRIG, 0xc0);
    USB_OTG_WRITE_REG8(CORE_USB_CONFIG, 0x3c);
    USB_OTG_WRITE_REG8(CORE_USB_STATUS, 0xFF);
    USB_OTG_WRITE_REG8(CORE_USB_FIFO_EMPTY, 0xFF);
    USB_OTG_WRITE_REG8(CORE_USB_ADDR, 0x00);
    USB_OTG_WRITE_REG8(CORE_USB_INT_MASK(0), 0x70);
    USB_OTG_WRITE_REG8(CORE_USB_INT_MASK(1), 0xf0);
    USB_OTG_WRITE_REG8(CORE_USB_INT_MASK(2), 0xff);


}

uint8_t Rxbuf[1024 * 2] = {0}, send_data_flag = 0;
int totalReceiveLen = 0;
void usb_main(void)
{

#if ((VERSIONS == EXIST_BT) || (VERSIONS == NO_BT))
    enable_clock(CLKCLS_BT);
    BT_CLKPLL_EN = 0xff;
    SYSCTRL_HCLK_CON |= 1 << 11;
#elif (VERSIONS == NO_XTAL)

#endif
    enable_clock(CLKCLS_USB);
    usb_reg_init();

    memset(&USB_OTG_dev, 0, sizeof(USB_OTG_dev));

    USBD_Init(&USB_OTG_dev,
              USB_OTG_FS_CORE_ID,
              &USR_desc,
              &USBD_PRINTER_cb,
              &USRD_cb);
    enable_intr(INTR_USB);
    while (1)
    {
        int rxlen = 0;
#if USB_FLOWCTRL
        rxlen = VCP_GetRxBuflen();
        if (rxlen > 0)
        {
            totalReceiveLen += rxlen;
            MyPrintf("totalReceiveLen = %d rxlen = %d \n", totalReceiveLen, rxlen);
            for (int i = 0; i < rxlen; i++)
            {
                Rxbuf[i] = VCP_GetRxChar();
            }
        }
#endif

    }
}



void USB_IRQHandler(void)
{

    USBD_OTG_ISR_Handler(&USB_OTG_dev);

}

/************************ (C) COPYRIGHT Yichip Microelectronics *****END OF FILE****/

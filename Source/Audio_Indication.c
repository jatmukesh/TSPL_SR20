


/**
 * @file    audio_indication.c
 * @brief   Application-level driver for the SR20 Audio Indication.
 */

#include "Audio_Indication.h"
#include "yc_timer.h"  
#include "yc_systick.h"
#include "PDL.h"
#include "macros.h"
#include "PrinterParType.h"


extern _PDL PRINTER_PDL;
extern void  Delay1ms(unsigned short mdelay);

#ifdef AUDIO_BEEP_SEQUENCE
// 3. Global Context Variable Declaration
//static BuzzerManager_t g_BuzzerCtx = {BUZZ_STATE_IDLE, 0, 0, 0, 0, 0, 0};
//static unsigned char g_BuzzerRecursionFlag = 0;
/**
 * @brief  Initializes or resets the buzzer state machine based on sensor inputs.
 * @param  is_paper_out  Current status of the paper sensor (1 = Out, 0 = Present)
 * @param  is_lid_open   Current status of the lid sensor (1 = Open, 0 = Closed)
 */

//void Process_Buzzer_State_Machine(void);
void BuzzerManager_UpdateUI(void);
#endif

#ifdef SYSTEM_SOUND_SETTING
unsigned char g_System_Sound_Flag = 1;
#endif

extern unsigned int g_sysTime;
extern unsigned short	m_SaveConPar[10];
extern PrintrConfig	s_PrintrConfig;
/**
* @FuncName  Audio_Beep_Custom
* @param     unsigned char count   - Number of beep pulses to generate (capped at
BEEP_MAX_COUNT = 20)
*            unsigned int on_ms    - Active buzzer ON duration per pulse in milliseconds
*            unsigned int off_ms   - Silent buzzer OFF duration between pulses in
milliseconds
* @retval    void
* @LocalVar  unsigned char i - Loop iteration counter tracking completed beep pulses
* @GlobalVar PRINTER_PDL
* @Description Generates a configurable sequence of  beep pulses using  delays.
					to refresh the hardware watchdog timer.
*/
void Audio_Beep_Custom(unsigned char count, unsigned int on_ms, unsigned int off_ms) {
    unsigned char i;

    // Safety check: Prevent infinite loops or extreme blocking
    if (count > BEEP_MAX_COUNT) count = BEEP_MAX_COUNT;

    for (i = 0; i < count; i++) {
        PRINTER_PDL.SYS._BEEP_EN();   // Turn buzzer ON
        Delay1ms(on_ms);              // Wait (Blocking)
        
        PRINTER_PDL.SYS._BEEP_DIS();  // Turn buzzer OFF
        
        // Don't delay after the very last beep
        if (i < (count - 1)) {
            Delay1ms(off_ms);         // Wait (Blocking)
        }
        
        // Feed the watchdog so the printer doesn't reset during long beeps
        PRINTER_PDL.SYS._WDG_ReloadCounter();
    }
}




















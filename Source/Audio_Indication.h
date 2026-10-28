#ifndef AUDIO_INDICATION_H
#define AUDIO_INDICATION_H




#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


// Safety Constraint: Maximum allowed beeps in a blocking function
#define BEEP_MAX_COUNT         20

// Media (Paper/Label roll ) Error Notification 
#define MEDIA_ABSENT_BEEP_ON_TIME_MS			100
#define MEDIA_ABSENT_BEEP_OFF_TIME_MS			100
#define MEDIA_BEEP_COUNT									3
#define MEDIA_BEEP_INTERVAL_MS						10000 //30 secs	


// Cover / LID  Error  (Lid Open)
#define	LID_OPEN_BEEP_ON_TIME_MS						50
#define	LID_OPEN_BEEP_OFF_TIME_MS						50
#define LID_BEEP_COUNT											3
#define LID_OPEN_BEEP_INTERVAL_MS						10000


#define MENU_PAR_INDEX_REMINDER    3

// 1. State  Declaration
typedef enum {
    BUZZ_STATE_IDLE = 0,         		//Buzzer is inactive  , waiting for media/Lid absent detection
    BUZZ_STATE_CALC_INTERVAL,    		//Calculating the time gap between reminder sequences
    BUZZ_STATE_PLAY_MEDIA,       		//Playing the "Media Out" beep sequence
    BUZZ_STATE_GAP_WAIT,         		//wait between overlapping error conditions
    BUZZ_STATE_PLAY_LID,         		//Playing the "Lid Open" beep sequence
    BUZZ_STATE_INTERVAL_WAIT,    		//wait until the next reminder sequence
    BUZZ_STATE_EXHAUSTED         		//Target reminders reached; buzzer silenced until reset
} BuzzerState_t;
// 2. Context Struct Declaration
typedef struct {
    BuzzerState_t current_state;
    uint32_t      next_action_timestamp; 
    uint32_t      interval_ms;           
    uint8_t       reminders_played;      
    uint8_t       target_reminders;      
    uint8_t       prev_paper_status;     
    uint8_t       prev_lid_status;
} BuzzerManager_t;

static BuzzerManager_t g_BuzzerCtx = {BUZZ_STATE_IDLE, 0, 0, 0, 0, 0, 0};
extern unsigned char g_KeySoundEnable; // Global toggle state
void Process_Buzzer_State_Machine(void);
void BuzzerManager_Init(uint8_t is_paper_out, uint8_t is_lid_open);















void Audio_Beep_Custom(unsigned char count, unsigned int on_ms, unsigned int off_ms);
void Audio_Key_Click(void);






#endif // AUDIO_INDICATION_H
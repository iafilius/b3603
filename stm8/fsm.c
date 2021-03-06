
// Finite State Enginer for menu structure

#include <stdint.h>
//#include "stm8s.h"
#include "display.h"
#include "buttons.h"
#include "config.h"
#include "outputs.h"
#include "fsm.h"

uint8_t	fsm_state=FSM_SHOWVIN_TXT;	// default start to show VIN... for now
extern state_t state;
extern cfg_output_t cfg_output;
extern cfg_system_t cfg_system;

uint16_t proposed_new_vset;	// new vset to be confimed  <ok>
uint16_t proposed_new_cset;	// new cset to be confirmed <ok>
uint8_t proposed_new_output;	// new value of output power  to be confirmed <ok>


void process_fsm_state(uint8_t button) 
{
static uint16_t i=0;	// 0 => turns immidiate to ON
static uint8_t blink=OFF;	// turns immediate to ON as counter i is 0, volotile to prevent compiler warning and non funcitoning code
static uint8_t repeated=MAXREPEAT;	// volatile for compiler warning/non working code
uint8_t local_fsm_state=fsm_state;	// to prevent compiler confusion
// all menu state/actions and transistions are done here

switch (local_fsm_state) {
    case FSM_SHOWVIN_TXT:

            if (button == BUTTON_SET) {
                fsm_state=FSM_SHOWVOUT_TXT;
                break;
                }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWVIN_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    if(blink == OFF) {
                        display_show_raw_digits((0x3E<<1), (0x00<<1), (0x10<<1), (0x15<<1));                
                        blink=ON;
                    } else {
                        display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));                
                        blink=OFF;
                    }
                i=MAXBLINKDELAY;
                repeated--;	// one Complete blink event
                }
                    
            
            // state transitions
        break;
    case FSM_SHOWVIN_ACTION:

            if (button == BUTTON_SET) {
                fsm_state=FSM_SHOWVOUT_TXT;
                break;
                }
    
            display_show_uint16(state.vin);

            
            // state transitions
        break;
    case FSM_SHOWVOUT_TXT:
    
            if (button == BUTTON_SET) {
                fsm_state=FSM_SHOWIOUT_TXT;
                break;
                }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWVOUT_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    if(blink == OFF) {
                        display_show_raw_digits((0x3E<<1), (0x1D<<1), (0x1C<<1), (0x0F<<1));     //Uout           
                        blink=ON;
                    } else {
                        display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));   // blank             
                        blink=OFF;
                    }
                i=MAXBLINKDELAY;
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_SHOWVOUT_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                case BUTTON_DOWN:
                    fsm_state=FSM_SHOWVOUT_SET_TXT;
                    break;
                case BUTTON_OK:
                    fsm_state=FSM_OUTPUT_ENABLE_TXT;
                    break;

            }
/*            if (button == BUTTON_SET) {
                fsm_state=FSM_SHOWIOUT_TXT;
                break;
            }

*/
            display_show_uint16(state.vout);
    

        break;



    case FSM_SHOWVOUT_SET_TXT:
    

            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                    proposed_new_vset=cfg_output.vset;
                    proposed_new_vset+=10;
                    fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;
                    break;
                case BUTTON_DOWN:
                    proposed_new_vset=cfg_output.vset;
                    proposed_new_vset-=10;
                    fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;
                    break;
                case BUTTON_OK:
                    fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;                    
                    break;

            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWVOUT_SET_ACTION;
                proposed_new_vset=cfg_output.vset;
                break;
            }
            
    
                if (i--==0) {
                    if(blink == OFF) {
                        display_show_raw_digits((0x3E<<1), (0x1D<<1), (0x1C<<1), (0x0F<<1));         // Set       
                        blink=ON;
                    } else {
                        display_show_raw_digits( (0x5B<<1), (0x4F<<1), (0x0F<<1), (0x00<<1));        //Uout
                        blink=OFF;
                    }
                i=MAXBLINKDELAY*4;	// MAXBLINKDELAY seems to fast here for unclear reasons , 
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_SHOWVOUT_SET_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                    proposed_new_vset+=10;
                    fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;
                    break;
                case BUTTON_DOWN:
                    proposed_new_vset-=10;
                    fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;
                    break;
                case BUTTON_OK:
                    fsm_state=FSM_SHOWVOUT_TXT;	// return to display actual value mode
                    
                    break;

            }
            display_show_uint16(cfg_output.vset);	// actual desired value
        break;
        
        

///---

    case FSM_SHOWVOUT_SET_NEW_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                    proposed_new_vset+=10;
                    fsm_state=FSM_SHOWVOUT_SET_NEW_ACTION;
                    break;
                case BUTTON_DOWN:
                    proposed_new_vset-=10;
                    fsm_state=FSM_SHOWVOUT_SET_NEW_ACTION;
                    break;
                case BUTTON_OK:
                    fsm_state=FSM_SHOWVOUT_SET_NEW_ACTION;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWVOUT_SET_NEW_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    switch (blink) {
                        case 3:
                            display_show_raw_digits((0x3E<<1), (0x1D<<1), (0x1C<<1), (0x0F<<1)); // Uout               
                            blink--;
                            break;
                        case 2:
                            display_show_raw_digits( (0x5B<<1), (0x4F<<1), (0x0F<<1), (0x00<<1)); // Set               
                            blink--;
                            break;
                        case 1:
                            display_show_raw_digits( (0x76<<1), (0x4F<<1), (0x1E<<1), (0x3C<<1)); //NEW               
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));                
                            blink=3;
                            break;
                        default:
                            blink=3;
                            break;
                    }
    
                i=MAXBLINKDELAY*4;	// MAXBLINKDELAY seems to fast here for unclear reasons , 
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_SHOWVOUT_SET_NEW_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                    proposed_new_vset+=10;
                    //fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;
                    break;
                case BUTTON_DOWN:
                    proposed_new_vset-=10;
                    //fsm_state=FSM_SHOWVOUT_SET_NEW_TXT;
                    break;
                case BUTTON_OK:
                    cfg_output.vset = proposed_new_vset;
                    output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_SHOWVOUT_SET_TXT;	// return to showing set value only 
                    
                    break;

            }
            
            // always blink until new value is set
                if (i--==0) {
                    switch (blink) {
//                        case 4:
//                        case 3:
                        case 1:
                            display_show_uint16(proposed_new_vset);
                            blink--;
                            break;
/*                        case 1:
                            display_show_raw_digits( (0x76<<1), (0x4F<<1), (0x1E<<1), (0x3C<<1));   	// New             
                            blink--;
                            break;
*/                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));	// Clear
                            blink=1;
                            break;
                        default:
                            blink=1;
                            break;
                
                        }
                i=MAXBLINKDELAY;
                }
                    
                
        break;
///////-----------        

    case FSM_OUTPUT_ENABLE_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                    //proposed_new_vset+=10;
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;
                    break;
                case BUTTON_DOWN:
                    //proposed_new_vset-=10;
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;
                    break;
                case BUTTON_OK:
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_OUTPUT_ENABLE_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    switch (blink) {
                        case 1:
                            display_show_raw_digits((0x67<<1), (0x7E<<1), (0x1E<<1), (0x3C<<1)); // POW
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));                
                            blink=1;
                            break;
                        default:
                            blink=1;
                            break;
                    }
    
                i=MAXBLINKDELAY*4;	// MAXBLINKDELAY seems to fast here for unclear reasons , 
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_OUTPUT_ENABLE_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                    //proposed_new_vset+=10;
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;
                    proposed_new_output=cfg_system.output;	// init
                    break;
                case BUTTON_DOWN:
                    //proposed_new_vset-=10;
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;
                    proposed_new_output=cfg_system.output;	// init
                    break;
                case BUTTON_OK:
                    //cfg_output.vset = proposed_new_vset;
                    //output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;	// return to showing set value only 
                    proposed_new_output=cfg_system.output;	// init
                    break;

            }
            
           //display_show_uint16(proposed_new_output);
           display_show_decimal_digits(0xFF,0,0xFF,0,0xFF,0,proposed_new_output,0);
                
        break;
        

//// -------------
    case FSM_OUTPUT_ENABLE_SET_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:		// do not rush to change value here
                case BUTTON_DOWN:	// do not rush to change value here
                case BUTTON_OK:
                    fsm_state=FSM_OUTPUT_ENABLE_SET_ACTION;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_OUTPUT_ENABLE_SET_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    switch (blink) {
                        case 3:
                            display_show_raw_digits((0x67<<1), (0x7E<<1), (0x1E<<1), (0x3C<<1)); // POW               
                            blink--;
                            break;
                        case 2:
                            display_show_raw_digits( (0x5B<<1), (0x4F<<1), (0x0F<<1), (0x00<<1)); // Set               
                            blink--;
                            break;
                        case 1:
                            display_show_raw_digits( (0x76<<1), (0x4F<<1), (0x1E<<1), (0x3C<<1)); //NEW               
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));                
                            blink=3;
                            break;
                        default:
                            blink=3;
                            break;
                    }
    
                i=MAXBLINKDELAY*4;	// MAXBLINKDELAY seems to fast here for unclear reasons , 
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_OUTPUT_ENABLE_SET_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWIOUT_TXT;
                    break;
                case BUTTON_UP:
                case BUTTON_DOWN:
                    if(proposed_new_output==0) {
                        proposed_new_output=1;
                    } else {
                        proposed_new_output=0;
                    }
                    break;

                case BUTTON_OK:
                    cfg_system.output = proposed_new_output;
                    output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_OUTPUT_ENABLE_TXT;	// return to showing set value only 
                    
                    break;

            }
            
            // always blink until new value is set
                if (i--==0) {
                    switch (blink) {
                        case 1:
                            display_show_decimal_digits(0xFF,0,0xFF,0,0xFF,0,proposed_new_output,0);
                            //display_show_uint16(proposed_new_output);
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));	// Clear
                            blink=1;
                            break;
                        default:
                            blink=1;
                            break;
                
                        }
                i=MAXBLINKDELAY;
                }
                    
                
        break;
        


        
        
        
///////-----------        
////-----
        
    case FSM_SHOWIOUT_TXT:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                case BUTTON_DOWN:
                    fsm_state=FSM_SHOWIOUT_SET_TXT;
                    break;

                case BUTTON_OK:
                    fsm_state=FSM_OUTPUT_ENABLE_TXT;	// power on/off menu
                    
                    break;

            }
            




            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWIOUT_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    if(blink == OFF) {
                        display_show_raw_digits((0x06<<1), (0x1D<<1), (0x1C<<1), (0x0F<<1));     // Iout           
                        blink=ON;
                    } else {
                        display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));   // blank             
                        blink=OFF;
                    }
                i=MAXBLINKDELAY;
                repeated--;	// one Complete blink event
                }
                    

        break;
    case FSM_SHOWIOUT_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                case BUTTON_DOWN:
                    fsm_state=FSM_SHOWIOUT_SET_TXT;
                    break;

                case BUTTON_OK:
                    fsm_state=FSM_OUTPUT_ENABLE_TXT;	// power on/off menu
                    
                    break;

            }
            

            display_show_uint16(state.cout);

        break;
    

//<HIER> FSM_SHOWIOUT_SET_[TXT/ACTION ] en FSM_SHOWIOUT_SET_NEW_[TXT/ACTION]
///////-----------        

    case FSM_SHOWIOUT_SET_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                    //proposed_new_vset+=10;
                    fsm_state=FSM_SHOWIOUT_SET_NEW_TXT;
                    break;
                case BUTTON_DOWN:
                    //proposed_new_vset-=10;
                    fsm_state=FSM_SHOWIOUT_SET_NEW_TXT;
                    break;
                case BUTTON_OK:
                    fsm_state=FSM_SHOWIOUT_SET_NEW_TXT;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWIOUT_SET_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    switch (blink) {
                        case 2:
                            display_show_raw_digits((0x06<<1), (0x1D<<1), (0x1C<<1), (0x0F<<1));	// Iout
                            blink--;
                            break;
                        case 1:
                            display_show_raw_digits((0x5B<<1), (0x4F<<1), (0x0F<<1), (0x00<<1));	// SEt
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));                
                            blink=2;
                            break;
                        default:
                            blink=2;
                            break;
                    }
    
                i=MAXBLINKDELAY*4;	// MAXBLINKDELAY seems to fast here for unclear reasons , 
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_SHOWIOUT_SET_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                    //proposed_new_vset+=10;
                    fsm_state=FSM_SHOWIOUT_SET_NEW_TXT;
                    proposed_new_cset=cfg_output.cset;	// init
                    break;
                case BUTTON_DOWN:
                    //proposed_new_vset-=10;
                    fsm_state=FSM_SHOWIOUT_SET_NEW_TXT;
                    proposed_new_cset=cfg_output.cset;	// init
                    break;
                case BUTTON_OK:
                    //cfg_output.vset = proposed_new_vset;
                    //output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_SHOWIOUT_SET_NEW_TXT;	// return to showing set value only 
                    proposed_new_cset=cfg_output.cset;	// init
                    break;

            }
            
           //display_show_uint16(proposed_new_output);
           display_show_uint16(proposed_new_cset);
                
        break;
        

//// -------------
    case FSM_SHOWIOUT_SET_NEW_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:		// do not rush to change value here
                case BUTTON_DOWN:	// do not rush to change value here
                case BUTTON_OK:
                    fsm_state=FSM_SHOWIOUT_SET_NEW_ACTION;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=FSM_SHOWIOUT_SET_NEW_ACTION;
                break;
            }
            
    
                if (i--==0) {
                    switch (blink) {
                        case 3:
                            display_show_raw_digits((0x06<<1), (0x1D<<1), (0x1C<<1), (0x0F<<1)); // Iout               
                            blink--;
                            break;
                        case 2:
                            display_show_raw_digits( (0x5B<<1), (0x4F<<1), (0x0F<<1), (0x00<<1)); // Set               
                            blink--;
                            break;
                        case 1:
                            display_show_raw_digits( (0x76<<1), (0x4F<<1), (0x1E<<1), (0x3C<<1)); //NEW               
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));                
                            blink=3;
                            break;
                        default:
                            blink=3;
                            break;
                    }
    
                i=MAXBLINKDELAY*4;	// MAXBLINKDELAY seems to fast here for unclear reasons , 
                repeated--;	// one Complete blink event
                }
                    
    
            break;
    

        break;
    case FSM_SHOWIOUT_SET_NEW_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                    proposed_new_cset+=10;
                    break;
                case BUTTON_DOWN:
                    proposed_new_cset-=10;
                    break;

                case BUTTON_OK:
                    //cfg_system.output = proposed_new_output;
                    cfg_output.cset=proposed_new_cset;
                    output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_SHOWIOUT_SET_TXT;	// return to showing set value only 
                    
                    break;

            }
            
            // always blink until new value is set
                if (i--==0) {
                    switch (blink) {
                        case 1:
                            //display_show_decimal_digits(0xFF,0,0xFF,0,0xFF,0,proposed_new_output,0);
                            display_show_uint16(proposed_new_cset);
                            blink--;
                            break;
                        case 0:
                            display_show_raw_digits( (0x00<<1), (0x00<<1), (0x00<<1), (0x00<<1));	// Clear
                            blink=1;
                            break;
                        default:
                            blink=1;
                            break;
                
                        }
                i=MAXBLINKDELAY;
                }
                    
                
        break;
        





}


}

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
    case FSM_OUTPUT_ENABLE_ACTION:
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
                    //cfg_output.vset = proposed_new_vset;
                    //output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_OUTPUT_ENABLE_SET_TXT;	// return to showing set value only 
                    
                    break;

            }
            
           display_show_uint16(proposed_new_output);
                
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
                fsm_state=FSM_SHOWVOUT_SET_NEW_ACTION;
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

                case BUTTON_OK:
                    cfg_output.output = proposed_new_output;
                    output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=FSM_OUTPUT_ENABLE_TXT;	// return to showing set value only 
                    
                    break;

            }
            
            // always blink until new value is set
                if (i--==0) {
                    switch (blink) {
                        case 1:
                            display_show_uint16(proposed_new_output);
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
        


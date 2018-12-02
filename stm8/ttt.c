
    case CALIBRATE_VIN_a_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                    //proposed_new_vset+=10;
                    fsm_state=CALIBRATE_VIN_a_NEW_TXT;
                    break;
                case BUTTON_DOWN:
                    //proposed_new_vset-=10;
                    fsm_state=CALIBRATE_VIN_a_NEW_TXT;
                    break;
                case BUTTON_OK:
                    fsm_state=CALIBRATE_VIN_a_NEW_TXT;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=CALIBRATE_VIN_a_ACTION;
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
    case CALIBRATE_VIN_a_ACTION:
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:
                    //proposed_new_vset+=10;
                    fsm_state=CALIBRATE_VIN_a_NEW_TXT;
                    proposed_new_cset=cfg_output.cset;	// init
                    break;
                case BUTTON_DOWN:
                    //proposed_new_vset-=10;
                    fsm_state=CALIBRATE_VIN_a_NEW_TXT;
                    proposed_new_cset=cfg_output.cset;	// init
                    break;
                case BUTTON_OK:
                    //cfg_output.vset = proposed_new_vset;
                    //output_commit(&cfg_output, &cfg_system, state.constant_current);
                    fsm_state=CALIBRATE_VIN_a_NEW_TXT;	// return to showing set value only 
                    proposed_new_cset=cfg_output.cset;	// init
                    break;

            }
            
           //display_show_uint16(proposed_new_output);
           display_show_uint16(proposed_new_cset);
                
        break;
        

//// -------------
    case CALIBRATE_VIN_a_NEW_TXT:
    
            switch(button) {
                case BUTTON_SET:
                    fsm_state=FSM_SHOWVIN_TXT;
                    break;
                case BUTTON_UP:		// do not rush to change value here
                case BUTTON_DOWN:	// do not rush to change value here
                case BUTTON_OK:
                    fsm_state=CALIBRATE_VIN_a_NEW_ACTION;
                    break;
            }

            if (repeated == 0) {
                repeated=MAXREPEAT;
                fsm_state=CALIBRATE_VIN_a_NEW_ACTION;
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
    case CALIBRATE_VIN_a_NEW_ACTION:
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
                    fsm_state=CALIBRATE_VIN_a_TXT;	// return to showing set value only 
                    
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
        

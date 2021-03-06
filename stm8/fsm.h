


#define FSM_SHOWVIN_TXT				0	// Show input voltage banner
#define FSM_SHOWVIN_ACTION			1	// show input voltage value

#define FSM_SHOWVOUT_TXT			2	// show current output voltage banner
#define FSM_SHOWVOUT_ACTION			3	// show current output voltage value

#define FSM_SHOWVOUT_SET_TXT			4	// display output set voltage banner
#define FSM_SHOWVOUT_SET_ACTION			5	// display output set voltage value

#define FSM_SHOWVOUT_SET_NEW_TXT		6	// set new output set voltage banner
#define FSM_SHOWVOUT_SET_NEW_ACTION		7	// set new output set voltage value


#define FSM_OUTPUT_ENABLE_TXT			8	// enable/diable output banner
#define FSM_OUTPUT_ENABLE_ACTION		9	// enable/disable value
#define FSM_OUTPUT_ENABLE_SET_TXT		10	// set current value out output enable/disable banner
#define FSM_OUTPUT_ENABLE_SET_ACTION		11	// set current value out output enable/disable  action


#define FSM_SHOWIOUT_TXT			12	// show actual current banner
#define FSM_SHOWIOUT_ACTION			13	// show actual current value

#define FSM_SHOWIOUT_SET_TXT			14	// show set value banner 
#define FSM_SHOWIOUT_SET_ACTION			15	// show set value value

#define FSM_SHOWIOUT_SET_NEW_TXT		16	// set new value banner 
#define FSM_SHOWIOUT_SET_NEW_ACTION		17	// set new value action

#define CALIBRATE_TXT				18	// calibration menu banner
#define CALIBRATE_ACTION			19	// Select calibration function

#define CALIBRATE_F1_LOW_TXT			20
#define CALIBRATE_F1_LOW_ACTION			21	// calibration F1_low (2V)
#define CALIBRATE_F1_HIGH_TXT			20
#define CALIBRATE_F1_HIGH_ACTION		21	// calibration F1_low (2V)
#define CALIBRATE_F2_LOW_TXT			22	
#define CALIBRATE_F2_LOW_ACTION			23	// calibraiton 
#define CALIBRATE_F2_HIGH_TXT			22	
#define CALIBRATE_F2_HIGH_ACTION		23	// calibraiton 
#define CALIBRATE_F3_LOW_TXT			24
#define CALIBRATE_F3_LOW_ACTION			25
#define CALIBRATE_F3_HIGH_TXT			24
#define CALIBRATE_F3_HIGH_ACTION		25

#define CALIBRATE_F4_LOW_TXT			24
#define CALIBRATE_F4_LOW_ACTION			25
#define CALIBRATE_F4_HIGH_TXT			24
#define CALIBRATE_F4_HIGH_ACTION		25

#define CALIBRATE_F5_LOW_TXT			24
#define CALIBRATE_F5_LOW_ACTION			25
#define CALIBRATE_F5_HIGH_TXT			24
#define CALIBRATE_F5_HIGH_ACTION		25

#define CALIBRATE_F6_LOW_TXT			24
#define CALIBRATE_F6_LOW_ACTION			25
#define CALIBRATE_F6_HIGH_TXT			24
#define CALIBRATE_F6_HIGH_ACTION		25


#define CALIBRATE_VIN_a_TXT			26		// SET is equals SHOW, so 2 layer menu only
#define CALIBRATE_VIN_a_ACTION                  27		// SET is equals SHOW, so 2 layer menu only
#define CALIBRATE_VIN_a_NEW_TXT			28
#define CALIBRATE_VIN_a_NEW_ACTION             29



#define	MAXBLINKDELAY 1500
#define MAXREPEAT	12
#define ON	1
#define OFF	0

void process_fsm_state(uint8_t);

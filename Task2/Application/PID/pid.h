#ifndef PID_H
#define PID_H

typedef struct 
{
	float  Ref;   			    // Input: Reference input
	float  Fdb;   			    // Input: Feedback input
	float  Err;				    // Variable: Error
  	float LastErr;
	
	float  Kp;				    // Parameter: Proportional gain
	float  Ki;			        // Parameter: Integral gain
	float  Kd; 		            // Parameter: Differential gain
	
	float  Up;				    // Variable: Proportional output
	float  Ui;				    // Variable: Integral output
	float  Ud;				    // Variable: Differential output
	float  OutPreSat; 		    // Variable: Pre-saturated output

    float  InteMax;
    float  InteMin;

	float  OutMax;		        // Parameter: Maximum output
	float  OutMin;	    	    // Parameter: Minimum output
	float  Out;   			    // Output: PID output
}PID;

/*------------------------------------------------------------------------------
Prototypes for the functions in PID.C
------------------------------------------------------------------------------*/
void PidCalc(PID *);
void PidClear(PID *v);

#endif

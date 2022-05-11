#include "pid.h"

void PidCalc(PID *v)
{	
    // Compute the error
    v->Err = v->Ref - v->Fdb;

    // Compute the proportional output
    v->Up = (v->Kp)*(v->Err);
    // Compute the integral output
    v->Ui = v->Ui + (v->Ki)*(v->Err);
    if(v->Ui > v->InteMax) v->Ui = v->InteMax;
    if(v->Ui < v->InteMin) v->Ui = v->InteMin;
    // Compute the differential output
    v->Ud = (v->Kd)*(v->Err - v->LastErr);

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;
    // Saturate the output
    if (v->OutPreSat > v->OutMax)                   
      v->Out =  v->OutMax;
    else if (v->OutPreSat < v->OutMin)
      v->Out =  v->OutMin;  
    else
      v->Out = v->OutPreSat;

    // Update the previous output
    v->LastErr = v->Err;

}

void PidClear(PID *v)
{
    v->Ui = 0;
    v->Ud = 0;
}



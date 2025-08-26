



#include "PI.h"

void PI_Init(PI_Handle_t *p , float KP)
{
    p->KP = KP;          // Proportional gain
}

float PI_Eval(PI_Handle_t *p, float SetPoint, float PV)
{
    // 1) Compute raw error
    p->Error = SetPoint - PV;

    // 2) Compute dead‑band threshold (10% of the magnitude of SP)
    float deadband = 0.1f * fabsf(SetPoint);

    // 3) If error is small, force output to zero
    if (fabsf(p->Error) <= deadband) {
        p->Out = 0.0f;
        return p->Out;
    }

    // 4) Otherwise, proportional action as before
    p->Out = p->KP * p->Error;
    return p->Out;
}

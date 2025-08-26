/*
 * PI.h
 *
 *  Created on: Mar 3, 2025
 *      Author: MARIO
 */

#ifndef MOTOR_CONTROL_PI_PI_H_
#define MOTOR_CONTROL_PI_PI_H_

#include <math.h>
typedef struct
{
	float Error;
	//float SumError;

	float Out;
	float Error_Margin;

	//Controller Constant
	float KP;


}PI_Handle_t;

void PI_Init(PI_Handle_t *p, float KP);
float PI_Eval(PI_Handle_t *p, float SetPoint, float PV);





#endif /* MOTOR_CONTROL_PI_PI_H_ */

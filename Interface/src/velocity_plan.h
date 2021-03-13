#pragma once

#ifndef VELOCITY_PLAN_H
#define VELOCITY_PLAN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "PCS.h"
#include "LookAhead.h"
#include "myMatrix.h"
#include "Interpolation.h"

#define DEFAULT_VALUE

#ifdef DEFAULT_VALUE

#define PI                     3.14159

#define Default_max_velocity            0.5                  //max velocity in the joint angle.
#define Default_max_acceleration        1                    //max acceleration in the joint angle.
#define Default_Frequency                100                 // sampling frequence
#define Default_delta_vmax               (0.001*PI)           //max velocity change
#define Default_dof                      6                    //degree of freedom
#define Default_lgLookahead              50                  //number of lookahead
#define Default_Interpolation_points	 5

#endif

typedef struct
{
	char* angle_file_name;
	char* MaxVelocity_file_name;
	char* MaxAcceleration_file_name;
	char* save_inter_joint_angle_file_name;
	int Sample_Frequency;
	double delta_Vmax;
	double Max_velocity;
	double Max_acceleration;
	int Dof;
	int LgLookahead;
	int Interpolation_point;

}Interface_data;

Matrix_t velocity_plan(Interface_data data);
void velocity_plan_calculation(Interface_data data);

#endif

/*
 * main.c
 *
 *  Created on: 2019年8月7日
 *      Author: hyh
 */
#include "stdio.h"
#include "velocity_plan.h"

char joint_filename[100] = "/home/hyh/eclipse-workspace/Velocity_plan/Debug/refjoint.txt";
char Save_inter_joint_angle_file_name[100] = "/home/hyh/eclipse-workspace/Velocity_plan/Debug/joint_angle_interpolation_v3.txt";
char MaxVelocity_file[100] = "MaxVelocity.txt";
char MaxAcceleration_file[100] = "MaxAcceleration.txt";

int main()
{
	Matrix_t Inter_Joint_angle;

	unsigned char MaxVelocity_flag = 0;
	unsigned char MaxAccelertion_flag = 0;
	unsigned char Save_inter_joint_angle_flag = 1;

	Interface_data data;
	data.Dof = 6;
	data.Interpolation_point = 5;
	data.LgLookahead = 50;
	data.Sample_Frequency = 100;
	data.delta_Vmax = 0.001*pi;
	data.angle_file_name = joint_filename;
	data.save_inter_joint_angle_file_name = Save_inter_joint_angle_file_name;

	if(!MaxVelocity_flag)
	{
		data.MaxVelocity_file_name = NULL;
		data.Max_velocity = max_velocity;
	}
	else
		data.MaxVelocity_file_name = MaxVelocity_file;
	if(!MaxAccelertion_flag)
	{
		data.MaxAcceleration_file_name = NULL;
		data.Max_acceleration = max_acceleration;
	}
	else
		data.MaxAcceleration_file_name = MaxAcceleration_file;
	if(!Save_inter_joint_angle_flag)
		data.save_inter_joint_angle_file_name = NULL;
	else
		data.save_inter_joint_angle_file_name = Save_inter_joint_angle_file_name;

	Inter_Joint_angle = velocity_plan(data);

	return 0;
}


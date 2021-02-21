#include "Interpolation.h"
#include <stdlib.h>
#include <math.h>
#include  <malloc.h>
#include <time.h>

/*
*function:calculate sum of former n numbers of array
*/
double get_number_of_joint_angle_interpolation(int len, double ta[], double tl[], double td[])
{
	double sum_t = 0;
	int row = 0;
	for (row = 0; row < len - 1; row++)
	{
		sum_t += ta[row] + td[row] + tl[row];
	}

	return sum_t;
}
/*
*function:Calculate the time of each segment
*/
int CalcTime(int len, double* synthetic_velocity_lookahead, double* max_synthetic_acceleration, double* max_synthetic_velocity, double* Length, double* actual_max_velocity, double* segment1, double* segment2, double* segment3, double* ta, double* td, double* tl)
{
	int row = 0;

	for (row = 0; row < len - 1; row++)
	{
		if (max_synthetic_velocity[row] < sqrt((pow(synthetic_velocity_lookahead[row], 2) + pow(synthetic_velocity_lookahead[row + 1], 2) + 2 * max_synthetic_acceleration[row] * Length[row]) / 2))
		{
			actual_max_velocity[row] = max_synthetic_velocity[row];
		}
		else
		{
			actual_max_velocity[row] = sqrt((pow(synthetic_velocity_lookahead[row], 2) + pow(synthetic_velocity_lookahead[row + 1], 2) + 2 * max_synthetic_acceleration[row] * Length[row]) / 2);
		}
		segment1[row] = (pow(actual_max_velocity[row], 2) - pow(synthetic_velocity_lookahead[row], 2)) / 2 / max_synthetic_acceleration[row];
		segment3[row] = (pow(actual_max_velocity[row], 2) - pow(synthetic_velocity_lookahead[row + 1], 2)) / 2 / max_synthetic_acceleration[row];
		segment2[row] = Length[row] - segment1[row] - segment3[row];
		ta[row] = (actual_max_velocity[row] - synthetic_velocity_lookahead[row]) / max_synthetic_acceleration[row];
		td[row] = (actual_max_velocity[row] - synthetic_velocity_lookahead[row + 1]) / max_synthetic_acceleration[row];
		tl[row] = segment2[row] / actual_max_velocity[row];

		if (fabs(tl[row] < 1e-15))
		{
			tl[row] = 0;
		}
	}

	return 0;
}
/*
*function:calculate sum of former n numbers of array
*/
double my_sum(double arr[], int n)
{
	double sum_of_number = 0;
	int count = 0;
	for (count = 0; count < n + 1; count++)
	{
		sum_of_number += arr[count];
	}
	return sum_of_number;
}
/*
*function: calculate the Coffecients of each section joint, and union length
*input parameters: @len--the number of PCS_joint_angle 
				   @Degree-- degrees of freedom
				   @Fre--sampling frequency of trajectories
				   @K--

*return parameters:@Position--joint_angle after interpolating
*/
Matrix_t Interpolation(int len, int Degree, int Fre, Matrix_t* joint_angle, Matrix_t* K, double* synthetic_velocity_lookahead, double* max_synthetic_acceleration, double* max_synthetic_velocity, double* Length)
{
	double* actual_max_velocity = (double*)malloc(sizeof(double) * (len - 1));

	double* segment1 = (double*)malloc(sizeof(double) * (len - 1));
	double* segment2 = (double*)malloc(sizeof(double) * (len - 1));
	double* segment3 = (double*)malloc(sizeof(double) * (len - 1));

	double* ta = (double*)malloc(sizeof(double) * (len - 1));
	double* td = (double*)malloc(sizeof(double) * (len - 1));
	double* tl = (double*)malloc(sizeof(double) * (len - 1));

	CalcTime(len, synthetic_velocity_lookahead, max_synthetic_acceleration, max_synthetic_velocity, Length, actual_max_velocity, segment1, segment2, segment3, ta, td, tl);

	const int NumOfInterPoint = (int)(Fre * get_number_of_joint_angle_interpolation(len, ta, tl, td));
	Matrix_t Position = create_mat(NumOfInterPoint, Degree);


	int t = 0; //the t-th point to be interpolated
	double time = 0; //the time when t-th point to be interpolated
	int row = 0;
	int column = 0;
	int count = 0;
	double sampling_period = 1.0 / Fre;
	Matrix_t P = create_mat(len - 1, Degree);

	for (row = 0; row < len - 1; row++)
	{
		for (column = 0; column < Degree; column++)
		{
			P.data[row][column] = (joint_angle->data[row + 1][column] - joint_angle->data[row][column]);//the change vector
		}
	}

	double past_time = 0;
	double dt = 0;
	double delta_Length = 0;

	for (row = 0; row < len - 1; row++)
	{
		if (row == 0)
		{
			for (count = 0; count < (int)(ta[row] * Fre); count++)
			{
				time = time + sampling_period;
				dt = time;
				delta_Length = (pow(dt / ta[row], 2) * segment1[row]) / Length[row];
				for (column = 0; column < Degree; column++)
				{
					Position.data[t][column] = joint_angle->data[row][column] + P.data[row][column] * delta_Length;
				}
				t++;
			}

			past_time = time;

			for (count = 0; count < (int)((ta[row] + tl[row] - past_time) * Fre); count++)
			{
				time = time + sampling_period;
				dt = time - ta[row];
				delta_Length = (segment1[row] + dt * segment2[row] / tl[row]) / Length[row];
				for (column = 0; column < Degree; column++)
				{
					Position.data[t][column] = joint_angle->data[row][column] + P.data[row][column] * delta_Length;
				}
				t++;
			}
			past_time = time;

			for (count = 0; count < (int)((ta[row] + td[row] + tl[row] - past_time) * Fre); count++)
			{
				time = time + sampling_period;
				dt = time - ta[row] - tl[row];
				delta_Length = (segment1[row] + segment2[row] + 0.5 * dt * (2 * actual_max_velocity[row] * td[row] - dt * (actual_max_velocity[row] - synthetic_velocity_lookahead[row + 1])) / td[row]) / Length[row];
				for (column = 0; column < Degree; column++)
				{
					Position.data[t][column] = joint_angle->data[row][column] + P.data[row][column] * delta_Length;
				}
				
				t++;
			}
		}
		else
		{
			past_time = time;
			for (count = 0; count < (int)((my_sum(ta, row) + my_sum(tl, row - 1) + my_sum(td, row - 1) - past_time) * Fre); count++)
			{
				time = time + sampling_period;
				dt = time - my_sum(ta, row - 1) - my_sum(tl, row - 1) - my_sum(td, row - 1);
				delta_Length = (0.5 * dt * (2 * synthetic_velocity_lookahead[row] * ta[row] + (actual_max_velocity[row] - synthetic_velocity_lookahead[row]) * dt) / ta[row]) / Length[row];
				for (column = 0; column < Degree; column++)
				{
					Position.data[t][column] = joint_angle->data[row][column] + P.data[row][column] * delta_Length;
				}
				t++;
			}

			past_time = time;

			for (count = 0; count < (int)((my_sum(ta, row) + my_sum(tl, row) + my_sum(td, row - 1) - past_time) * Fre); count++)
			{
				time = time + sampling_period;
				dt = time - my_sum(ta, row) - my_sum(tl, row - 1) - my_sum(td, row - 1);
				delta_Length = (segment1[row] + dt / tl[row] * segment2[row]) / Length[row];
				for (column = 0; column < Degree; column++)
				{
					Position.data[t][column] = joint_angle->data[row][column] + P.data[row][column] * delta_Length;
				}
				t++;
			}

			past_time = time;

			for (count = 0; count < (int)((my_sum(ta, row) + my_sum(tl, row) + my_sum(td, row) - past_time) * Fre); count++)
			{
				time = time + sampling_period;
				dt = time - my_sum(ta, row) - my_sum(tl, row) - my_sum(td, row - 1);
				delta_Length = (segment1[row] + segment2[row] + 0.5 * dt * (2 * actual_max_velocity[row] * td[row] - dt * (actual_max_velocity[row] - synthetic_velocity_lookahead[row + 1])) / td[row]) / Length[row];
				for (column = 0; column < Degree; column++)
				{
					Position.data[t][column] = joint_angle->data[row][column] + P.data[row][column] * delta_Length;
				}
				t++;
			}
		}
	}

	free_mat(&P);
	free(actual_max_velocity);			actual_max_velocity = NULL;
	free(segment1);						segment1 = NULL;
	free(segment2);						segment2 = NULL;
	free(segment3);						segment3 = NULL;
	free(ta);							ta = NULL;
	free(td);							td = NULL;
	free(tl);							tl = NULL;

	return Position;
}

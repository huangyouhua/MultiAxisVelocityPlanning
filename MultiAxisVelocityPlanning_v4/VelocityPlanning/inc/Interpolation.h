#pragma once

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "myMatrix.h"

double get_number_of_joint_angle_interpolation(int len, double ta[], double tl[], double td[]);
int CalcTime(int len, double* synthetic_velocity_lookahead, double* max_synthetic_acceleration, double* max_synthetic_velocity, double* Length, double* actual_max_velocity, double* segment1, double* segment2, double* segment3, double* ta, double* td, double* tl);
double my_sum(double arr[], int n);
Matrix_t Interpolation(int len, int Degree, int Fre, Matrix_t* joint_angle, Matrix_t* K, double* synthetic_velocity_lookahead, double* max_synthetic_acceleration, double* max_synthetic_velocity, double* Length);
#endif 


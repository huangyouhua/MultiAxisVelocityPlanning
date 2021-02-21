#pragma once

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "myMatrix.h"

//#define MSVC__COMPILER
#define MSVC__COMPILER

int GetLineNum(char* name);
Matrix_t get_ref_joint_angle(char* name, int len);
int save_joint_angle_data(char* name, Matrix_t* angle);
Matrix_t Get_Max_limited(int len, char* name, double Limited);

#endif

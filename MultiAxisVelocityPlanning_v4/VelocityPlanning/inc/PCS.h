#pragma once

#include "myMatrix.h"

#ifndef PCS_H
#define PCS_H

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE -1
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef ERR
#define ERR  -2
#endif

Matrix_t PCS(int Degree, int interpolation_points, int* NumOfPoint, Matrix_t* joint_angle);

#endif

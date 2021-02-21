#pragma once
#ifndef FILTER_H
#define FILTER_H

#include "myMatrix.h"

#define PI						3.14159
#define Nfilt					20			//WindowSize of MeanFilter
#define Gaussian_r				10
#define Gaussian_sigma			1
#define LowFilter_K			    0.01		//

Matrix_t MeanFilter(Matrix_t *x, int nfilt);
Matrix_t Gaussianfilter(Matrix_t* x, int r, double sigma);
Matrix_t low_filter(Matrix_t* x, double K);

#endif 
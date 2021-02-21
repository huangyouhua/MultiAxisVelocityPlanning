#include "filter.h"
#include <math.h>
#include <stdlib.h>

/*
function: use mean value of nfilt data 
input parameters: x -- the PT data need to be filtered, nfilter -- Window Size of filter 
return : y -- the filtered PT data
*/
Matrix_t MeanFilter(Matrix_t* x, int nfilt)
{
	Matrix_t y = create_mat(x->row, x->column);

	for (unsigned int col = 0; col < x->column; col++)
	{
		for (unsigned int i = 0; i < x->row ; i++)
		{
			if (i < (nfilt / 2) || i > (x->row - nfilt / 2))
			{
				y.data[i][col] = x->data[i][col];
			}
			else
			{
				for (int j = -nfilt / 2; j < nfilt / 2; j++)
				{
					y.data[i][col] += x->data[i + j][col];
				}
				y.data[i][col] = y.data[i][col] / nfilt;
			}
		}
	}

	return y;
}

/* function£º operation on one dimension data, 
* r     :size of Gaussian model ,advise odd number
* sigma : standard error
* y : the data need to be filtered
*/
Matrix_t Gaussianfilter(Matrix_t* x, int r, double sigma)
{
	unsigned int len = r * 2 - 1;
	int index = 0;
	Matrix_t y_filted = create_mat(x->row, x->column);
	double* GaussTemp = (double*)malloc(sizeof(double) * len);

	for (int row = 0; row < len; row++)
	{
		GaussTemp[row] = exp(-pow((row - r), 2) / (2 * pow(sigma, 2))) / (sigma * sqrt(2 * PI));
	}

	for (int col = 0; col < x->column; col++)
	{
		for (int row = 0; row < x->row; row++)
		{
			if (row < r - 1 || row >= x->row - r)
				y_filted.data[row][col] = x->data[row][col];
		}
		for (int row = r - 1; row < x->row - r; row++)
		{
			index = 0;
			for (int j = 1 - r; j < r - 1; j++)
			{
				y_filted.data[row][col] += x->data[row - r + 1][col] * GaussTemp[index++];
			}
		}
	}

	return y_filted;
}

/*
function: use low filter 
input parameters: x -- the PT data need to be filtered
return : y -- the filtered PT data
*/
Matrix_t low_filter(Matrix_t* x, double K)
{
	Matrix_t y_filted = create_mat(x->row, x->column);

	for (int col = 0; col < x->column; col++)
	{
		y_filted.data[0][col] = x->data[0][col];
		for (int row = 1; row < x->row; row++)
		{
			y_filted.data[row][col] = x->data[row][col] * K + (1 - K) * x->data[row - 1][col];
		}
	}

	return  y_filted;
}
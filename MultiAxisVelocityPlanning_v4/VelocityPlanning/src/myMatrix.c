#include"../inc/myMatrix.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"malloc.h"
#include <assert.h>
/*
create a matrix with 0
*/
Matrix_t create_mat(uint16 row, uint16 column)
{
	Matrix_t mat;
	if (row <= 0 || column <= 0)
	{
		printf("error, in create_mat: row <= 0||column<=0\n");
		exit(1);
	}
	if (row > 0 && column > 0)
	{
		mat.row = row;
		mat.column = column;
		mat.data = (double**)malloc(row * sizeof(double*));
		if (mat.data == NULL)
		{
			printf("error, in create_mat: mat.data==NULL");
			exit(1);
		}
		uint16 i;
		for (i = 0; i < row; i++)
		{
			*(mat.data + i) = (double*)malloc(column * sizeof(double));
			if (mat.data[i] == NULL)
			{
				printf("////////error, in create_mat: mat.data==NULL");
				exit(1);
			}
		}
		clear_mat(&mat);
	}
	return mat;
}
/*
free a matrix
*/
void free_mat(Matrix_t* mat)
{
	uint16 i;
	for (i = 0; i < mat->row; i++)
		free(mat->data[i]);/*�ͷ���*/
	free(mat->data);/*�ͷ�ͷָ��*/
}

/*
display the matrix
*/
void show_mat(const char* name, const Matrix_t* mat)
{
	uint16 i, j;
	printf("%s=\n", name);
	for (i = 0; i < mat->row; i++)
	{
		for (j = 0; j < mat->column; j++)
		{
			printf("%.6f\t", mat->data[i][j]);
		}
		printf("\n");
	}
}
/*
set datas to the matrix
*/
void set_mat_data(Matrix_t* mat, const double* data)
{
	uint16 i, j;
	for (i = 0; i < mat->row; i++)
	{
		for (j = 0; j < mat->column; j++)
		{
			mat->data[i][j] = data[i * mat->column + j];
		}
	}
}
/*
mat=mat1+mat2
*/
Matrix_t add_mat(const Matrix_t* mat1, const Matrix_t* mat2)
{

	if (mat1->row != mat2->row)
	{
		printf("error, in add_mat: mat1->row != mat2->row\n");
		exit(1);
	}
	if (mat1->column != mat2->column)
	{
		printf("error, in add_mat: mat1->column != mat2->column\n");
		exit(1);
	}
	Matrix_t mat;
	uint16 i, j;
	mat = create_mat(mat1->row, mat1->column);
	for (i = 0; i < mat1->row; i++)
	{
		for (j = 0; j < mat1->column; j++)
			mat.data[i][j] = mat1->data[i][j] + mat2->data[i][j];
	}
	return mat;
}
/*
mat=mat1-mat2
*/
Matrix_t sub_mat(const Matrix_t* mat1, const Matrix_t* mat2)//mat1-mat2;
{
	if (mat1->row != mat2->row)
	{
		printf("error, in sub_mat: mat1->row != mat2->row\n");
		exit(1);
	}
	if (mat1->column != mat2->column)
	{
		printf("error, in sub_mat: mat1->column != mat2->column\n");
		exit(1);
	}
	Matrix_t mat;
	uint16 i, j;
	mat = create_mat(mat1->row, mat1->column);
	for (i = 0; i < mat1->row; i++)
	{
		for (j = 0; j < mat1->column; j++)
			mat.data[i][j] = mat1->data[i][j] - mat2->data[i][j];
	}
	return mat;
}
/*
transpose the matrix, mat=mat'
*/
Matrix_t transpose_mat(const Matrix_t* mat)//mat'
{
	Matrix_t mat_T;
	mat_T = create_mat(mat->row, mat->column);
	uint16 i, j;
	for (i = 0; i < mat->row; i++)
	{
		for (j = 0; j < mat->column; j++)
		{
			mat_T.data[i][j] = mat->data[j][i];
		}
	}
	return mat_T;
}
/*
mat=scaler*mat
every element in the matrix multiplys a scaler
*/
Matrix_t scale_mat(const Matrix_t* mat, const double scaler)//scaler*Mat
{
	Matrix_t mat1;
	mat1 = create_mat(mat->row, mat->column);
	uint16 i, j;
	for (i = 0; i < mat->row; i++)
	{
		for (j = 0; j < mat->column; j++)
		{
			mat1.data[i][j] = mat->data[i][j] * scaler;
		}
	}
	return mat1;
}
/*
set all datas in matrix to zero
*/
void clear_mat(Matrix_t* mat)
{
	uint16 i, j;
	for (i = 0; i < mat->row; i++)
	{
		for (j = 0; j < mat->column; j++)
		{
			mat->data[i][j] = 0;
		}
	}
}
/*
mat=mat1*mat2
*/
Matrix_t mult_mat(const Matrix_t* mat1, const Matrix_t* mat2)
{
	Matrix_t mat;
	if (mat1->column != mat2->row)
	{
		printf("error,In mult_mat: mat1->column != mat2->row\n");
		exit(1);
	}
	else
	{
		mat = create_mat(mat1->row, mat2->column);
		clear_mat(&mat);
		uint16 i, j;
		for (i = 0; i < mat1->row; i++)
		{
			for (j = 0; j < mat2->column; j++)
			{
				uint16 m;
				for (m = 0; m < mat1->column; m++)
				{
					mat.data[i][j] += mat1->data[i][m] * mat2->data[m][j];
				}
			}
		}
	}
	return mat;
}
/*
generate a I(nxn) matrix
*/
Matrix_t eye(uint16 n)
{
	if (n <= 0)
	{
		printf("error, in eye: n<0\n");
		exit(1);
	}
	Matrix_t mat = create_mat(n, n);
	uint16 i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			if (i == j)
				mat.data[i][j] = 1;
			else
				mat.data[i][j] = 0;
		}
	return mat;
}
/*
generate a diagonal matrix with diag[n] as its main diagonal elements
*/
Matrix_t diag_mat(uint16 n, double* diag, int diff)
{
	if (n <= 0) {
		printf("error: in diag_mat(n<0)\n");
		exit(1);
	}
	Matrix_t mat = create_mat(n, n);
    unsigned int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			if (j - i == diff)
				mat.data[i][j] = diag[i];
			else
				mat.data[i][j] = 0;
		}
	return mat;
}
/*
copy a matrix
*/
Matrix_t copy_mat(const Matrix_t* mat)
{
	uint16 i, j;
	Matrix_t mat_copy = create_mat(mat->row, mat->column);
	for (i = 0; i < mat->row; i++)
	{
		for (j = 0; j < mat->column; j++)
		{
			mat_copy.data[i][j] = mat->data[i][j];
		}
	}
	return mat_copy;
}
/*
copy matrix's data to another matrix
*/
void copy_mat_data(const Matrix_t* mat, Matrix_t* copy)
{
	if (mat->row != copy->row || mat->column != copy->column)
	{
		printf("error, in copy_mat_data: mat->row != copy->row || mat->column != copy->column\n");
		exit(1);
	}
	uint16 i, j;
	for (i = 0; i < mat->row; i++)
		for (j = 0; j < mat->column; j++)
		{
			copy->data[i][j] = mat->data[i][j];
		}
}
/*
get matrix's derterminent value
*/
double det_mat(Matrix_t* m)
{
	uint16 i, j, n, max_row;
	char swap_f;
	double max, k;
	double det = 1;
	swap_f = 0;
	if (m->column != m->row)
	{
		printf("error:In det_mat (m->column != m->row)\n");
		exit(1);
	}
	Matrix_t mat = copy_mat(m);
	for (i = 0; i < mat.row - 1; i++)
	{
		max = fabs(mat.data[i][i]);
		max_row = i;
		for (j = i + 1; j < mat.row; j++)
		{
			if (max < fabs(mat.data[j][i]))
			{
				max = fabs(mat.data[j][i]);
				max_row = j;
			}
		}
		if (i != max_row)
		{
			swap_row_mat(&mat, i, max_row);
			swap_f++;
		}
		for (j = i + 1; j < mat.row; j++)
		{
			k = -mat.data[j][i] / mat.data[i][i];
			for (n = 0; n < mat.column; n++)
			{
				mat.data[j][n] = mat.data[i][n] * k + mat.data[j][n];
			}
		}
	}
	if (swap_f % 2 == 1)swap_f = -1;
	else swap_f = 1;
	det = 1;
	for (i = 0; i < mat.column; i++)
		det *= mat.data[i][i];
	det *= swap_f;
	//show_mat("in det_mat: mat_upper",&mat);
	free_mat(&mat);
	return det;
}
/*
get inverse matrix
use main column element of Gauss-Jordan algrithm: A|I  --->  I|A^(-1)
*/
Matrix_t inverse_mat(Matrix_t* m)
{
	if (det_mat(m) == 0)
	{
		printf("error: In inverse_mat(det_mat(mat) == 0)\n");
		exit(1);
	}

	Matrix_t mat = copy_mat(m);
	Matrix_t inv_mat = eye(m->row);

	uint16 i, j, n, max_row;
	int swap_f = 0;
	double max, k;
	for (i = 0; i < mat.row - 1; i++)
	{
		max = fabs(mat.data[i][i]);
		max_row = i;
		for (j = i + 1; j < mat.row; j++)
		{
			if (max < fabs(mat.data[j][i]))
			{
				max = fabs(mat.data[j][i]);
				max_row = j;
			}
		}
		if (i != max_row)
		{
			swap_row_mat(&mat, i, max_row);
			swap_row_mat(&inv_mat, i, max_row);
			swap_f++;
		}
		for (j = i + 1; j < mat.row; j++)
		{
			k = -mat.data[j][i] / mat.data[i][i];
			for (n = 0; n < mat.column; n++)
			{
				mat.data[j][n] = mat.data[i][n] * k + mat.data[j][n];
				inv_mat.data[j][n] = inv_mat.data[i][n] * k + inv_mat.data[j][n];
			}
		}
	}

	for (i = 0; i < mat.row; i++)
	{
		k = 1 / mat.data[i][i];
		scale_row_mat(&mat, i, k);
		scale_row_mat(&inv_mat, i, k);
	}
	for (int row = mat.row - 1; row > 0; row--)
	{
		for (int col = row - 1; col >= 0; col--)
		{
			k = -mat.data[col][row] / mat.data[row][row];
			for (n = 0; n < mat.column; n++)
			{
				mat.data[col][n] = mat.data[col][n] + k * mat.data[row][n];
				inv_mat.data[col][n] = inv_mat.data[col][n] + k * inv_mat.data[row][n];
			}
		}

	}
	//show_mat("in inverse_mat: mat", &mat);
	//show_mat("in inverse_mat: inv_mat", &inv_mat);
	free_mat(&mat);
	return inv_mat;
}

Matrix_t destructive_invert_matrix(Matrix_t input) {
	uint16 i;
	uint16 j;
	uint16 r;
	double scalar;
	double shear_needed;

	Matrix_t output = eye(input.row);
	assert(input.row == input.column);
	assert(input.row == output.column);
	assert(input.row == output.column);


	/* Convert input to the identity matrix via elementary row operations.
	   The ith pass through this loop turns the element at i,i to a 1
	   and turns all other elements in column i to a 0. */

	for (i = 0; i < input.row; ++i) {

		if (input.data[i][i] == 0.0) {
			/* We must swap rows to get a nonzero diagonal element. */

			for (r = i + 1; r < input.row; ++r) {
				if (input.data[r][i] != 0.0) {
					break;
				}
			}
			if (r == input.row) {
				/* Every remaining element in this column is zero, so this
				   matrix cannot be inverted. */
				exit(1);
			}
			swap_row_mat(&input, i, r);
			swap_row_mat(&output, i, r);
		}

		/* Scale this row to ensure a 1 along the diagonal.
		   We might need to worry about overflow from a huge scalar here. */
		scalar = 1.0 / input.data[i][i];
		scale_row_mat(&input, i, scalar);
		scale_row_mat(&output, i, scalar);

		/* Zero out the other elements in this column. */
		for (j = 0; j < input.row; ++j) {
			if (i == j) {
				continue;
			}
			shear_needed = -input.data[j][i];
			shear_row(&input, j, i, shear_needed);
			shear_row(&output, j, i, shear_needed);
		}
	}

	return output;
}

/*swap NO.m and NO.n row in mat*/
void swap_row_mat(Matrix_t* mat, uint16 m, uint16 n)
{
	double temp;
	uint16 i;
	for (i = 0; i < mat->column; i++)
	{
		temp = mat->data[m][i];
		mat->data[m][i] = mat->data[n][i];
		mat->data[n][i] = temp;
	}
}
/*
NO.m row in matrix multiply a scaler
*/
void scale_row_mat(Matrix_t* mat, uint16 m, double scaler)
{
	uint16 i;
	for (i = 0; i < mat->column; i++)
		mat->data[m][i] *= scaler;
}

/* Add scalar * row r2 to row r1. */
void shear_row(Matrix_t* m, int r1, int r2, double scalar) {
	uint16 i;
	assert(r1 != r2);
	for (i = 0; i < m->column; ++i) {
		m->data[r1][i] += scalar * m->data[r2][i];
	}
}

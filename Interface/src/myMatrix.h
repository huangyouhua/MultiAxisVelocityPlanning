#ifndef __MY_MATRIX_H
#define __MY_MATRIX_H
typedef unsigned int uint16;

typedef struct
{
	uint16 row;
	uint16 column;
	double **data;
}Matrix_t;

Matrix_t create_mat(uint16 row, uint16 column);//create a matrix
void free_mat(Matrix_t *mat);//free a matrix
void show_mat(const char* name, const Matrix_t *mat);//show the matrix
void set_mat_data(Matrix_t* mat, const double *data);//set data to matrix
Matrix_t add_mat(const Matrix_t* mat1, const Matrix_t* mat2);//mat1+mat2;
Matrix_t sub_mat(const Matrix_t* mat1, const Matrix_t* mat2);//mat1-mat2;
Matrix_t transpose_mat(const Matrix_t* mat);//mat'
Matrix_t scale_mat(const Matrix_t* mat, const double scaler);//scaler*Mat
Matrix_t mult_mat(const Matrix_t *mat1, const Matrix_t* mat2);//mat1*mat2
double det_mat(Matrix_t *m);//get matrix's derterminent value
Matrix_t inverse_mat(Matrix_t* m);//get inverse matrix
void clear_mat(Matrix_t* mat);//set all matrix's data to 0
Matrix_t eye(uint16 n);//generate I(nxn) matrix
Matrix_t diag_mat(uint16 n,double* diag, int diff);//generate diag matrix which is nxn matrix
Matrix_t copy_mat(const Matrix_t *mat);//copy a matrix
void copy_mat_data(const Matrix_t* mat, Matrix_t* copy);//copy matrix's data to another matrix

void swap_row_mat(Matrix_t* mat, uint16 m, uint16 n);//swap NO.m and NO.n row in mat
void scale_row_mat(Matrix_t* mat, uint16 m, double scaler);//NO.m row in matrix multiply a scaler
void shear_row(Matrix_t* m, int r1, int r2, double scalar);

#endif


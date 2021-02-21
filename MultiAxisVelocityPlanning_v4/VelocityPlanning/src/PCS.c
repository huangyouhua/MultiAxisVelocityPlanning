#include <math.h>   
#include <stdio.h>  
#include <stdlib.h>   
#include "PCS.h"
#include <malloc.h>

typedef struct SPLINE    //定义样条结构体，用于存储一条样条的所有信息
{	
	//初始化数据输入
	 double *x;    //存储样条上的点的x坐标，最多51个点
	 double *y;    //存储样条上的点的y坐标，最多51个点
	 unsigned int point_num;   //存储样条上的实际的 点 的个数
	 double begin_k1;     //开始点的一阶导数信息
	 double end_k1;     //终止点的一阶导数信息
	 //double begin_k2;    //开始点的二阶导数信息
	 //double end_k2;     //终止点的二阶导数信息
	 //计算所得的样条函数S(x)
	 double *k1;    //所有点的一阶导数信息
	 double *k2;    //所有点的二阶导数信息
	 //51个点之间有50个段，func[]存储每段的函数系数
	 double *a3,*a1;    
	 double *b3,*b1;
	 //分段函数的形式为 Si(x) = a3[i] * {x(i+1) - x}^3  + a1[i] * {x(i+1) - x} +
	 //        b3[i] * {x - x(i)}^3 + b1[i] * {x - x(i)}
	 //xi为x[i]的值，xi_1为x[i+1]的值      
}SPLINE;

typedef int RESULT;      //返回函数执行的结果状态，下面为具体的返回选项

/*===============================================================================
*** 函数名称： Spline3()
*** 功能说明： 完成三次样条差值，其中使用追赶法求解M矩阵
*** 入口参数： (pSPLINE)pLine  样条结构体指针pLine中的x[],y[],num,begin_k1,end_k1
*** 出口参数： (pSPLINE)pLine  样条结构体指针pLine中的函数参数
*** 返回参数： 返回程序执行结果的状态TRUE or FALSE
================================================================================*/
RESULT Spline3(SPLINE* pLine, unsigned int NumOfPoint)
{
	 double* H = (double*)malloc(sizeof(double) * NumOfPoint);//小区间的步长
	 double* Fi = (double*)malloc(sizeof(double) * NumOfPoint);//中间量
	 double* U = (double*)malloc(sizeof(double) * (NumOfPoint + 1));//中间量
	 double* A = (double*)malloc(sizeof(double) * (NumOfPoint + 1));//中间量
	 double* D = (double*)malloc(sizeof(double) * (NumOfPoint + 1)); //中间量
	 double* M = (double*)malloc(sizeof(double) * (NumOfPoint + 1));//M矩阵
	 double* B = (double*)malloc(sizeof(double) * (NumOfPoint + 1));//追赶法中间量
	 double* Y = (double*)malloc(sizeof(double) * (NumOfPoint + 1));//追赶法中间变量

	 unsigned int i = 0;
	 ////////////////////////////////////////计算中间参数
	 if((pLine->point_num < 3) || (pLine->point_num > NumOfPoint + 1))
	 {
		return ERR;       //输入数据点个数太少或太多
	 }

	 for(i = 0;i <= pLine->point_num - 2;i++)
	{          //求H[i]
		H[i] = pLine->x[i+1] - pLine->x[i];
		Fi[i] = (pLine->y[i+1] - pLine->y[i]) / H[i]; //求F[x(i),x(i+1)]
	 }
	 for(i = 1;i <= pLine->point_num - 2;i++)
	 {          //求U[i]和A[i]和D[i]
		U[i] = H[i-1] / (H[i-1] + H[i]);
		A[i] = H[i] / (H[i-1] + H[i]);
		D[i] = 6 * (Fi[i] - Fi[i-1]) / (H[i-1] + H[i]);
	 }
	 //若边界条件为1号条件，则
	 U[i] = 1;
	 A[0] = 1;
	 D[0] = 6 * (Fi[0] - pLine->begin_k1) / H[0];
	 D[i] = 6 * (pLine->end_k1 - Fi[i-1]) / H[i-1];
	 //若边界条件为2号条件，则
	 //U[i] = 0;
	 //A[0] = 0;
	 //D[0] = 2 * begin_k2;
	 //D[i] = 2 * end_k2;
	 /////////////////////////////////////////追赶法求解M矩阵
	 B[0] = A[0] / 2;
	 for(i = 1;i <= pLine->point_num - 2;i++)
	 {
		B[i] = A[i] / (2 - U[i] * B[i-1]);
	 }

	 Y[0] = D[0] / 2;
	 for(i = 1;i <= pLine->point_num - 1;i++)
	 {
		Y[i] = (D[i] - U[i] * Y[i-1]) / (2 - U[i] * B[i-1]);
	 }

	 M[pLine->point_num - 1] = Y[pLine->point_num - 1];
	 for(i = pLine->point_num - 1;i > 0;i--)
	 {
		M[i-1] = Y[i-1] - B[i-1] * M[i];
	 }
	//////////////////////////////////////////计算方程组最终结果
	 for(i = 0;i <= pLine->point_num - 2;i++)
	 {
		  pLine->a3[i] = M[i] / (6 * H[i]);
		  pLine->a1[i] = (pLine->y[i] - M[i] * H[i] * H[i] / 6) / H[i];
		  pLine->b3[i] = M[i+1] / (6 * H[i]);
		  pLine->b1[i] = (pLine->y[i+1] - M[i+1] * H[i] * H[i] / 6) /H[i];
	 }

	 free(H);
	 free(Fi);
	 free(U);
	 free(A);
	 free(D);
	 free(M);
	 free(B);
	 free(Y);

	 return TRUE;
}
/*
*function: calculate the PCS_joint angle
*input parameters: @interpolation_points --- the number of points to be interpolated between two origin joint angles 
				   @NumOfPoint --- number of origin joint angles

*output parameters:@PCS_joint_angle --- joint_angle after interpolating
*/
Matrix_t PCS(int Degree, int interpolation_points, int* NumOfPoint, Matrix_t* joint_angle)
{

	Matrix_t PCS_joint_angle = create_mat(interpolation_points * (*NumOfPoint - 1), Degree);

    double x;
    int i_x;
	int count = 0;
	SPLINE line1;
  
	line1.x = malloc((*NumOfPoint + 1) * sizeof(double));
	line1.y = malloc((*NumOfPoint + 1) * sizeof(double));
	line1.k1 = malloc((*NumOfPoint + 1) * sizeof(double));
	line1.k2 = malloc((*NumOfPoint + 1) * sizeof(double));

	line1.a3 = malloc((*NumOfPoint) * sizeof(double));
	line1.b3 = malloc((*NumOfPoint) * sizeof(double));
	line1.a1 = malloc((*NumOfPoint) * sizeof(double));
	line1.b1 = malloc((*NumOfPoint) * sizeof(double));

    for(int i = 0; i < *NumOfPoint; i++)
    {
		line1.x[i] = i;
    }

	line1.point_num = *NumOfPoint;
	line1.begin_k1 = 0;
	line1.end_k1 = 0;

    for(int axis = 0; axis < Degree; axis++)
    {
      for(int j = 0; j < *NumOfPoint; j++)
        line1.y[j] = joint_angle->data[j][axis];

		Spline3(&line1, *NumOfPoint);
		count = 0;
		for(int i = 0; i < *NumOfPoint - 1; i++)
		{
		  for(int j = 0; j < interpolation_points; j++)
		  {
			 i_x = i;
			 x = (double)((double)i+((double)j)/interpolation_points);
			 PCS_joint_angle.data[interpolation_points *i+j][axis] = line1.a3[i_x] * (i_x+1 - x)*(i_x+1-x)*(i_x+1-x)  + line1.a1[i_x] * (i_x+1 - x) + line1.b3[i_x] * (x-i_x)*(x-i_x )*(x-i_x) + line1.b1[i_x] * (x - i_x);
			 count++;
		  }
		}
    }

    *NumOfPoint = count;

	free(line1.x);
	free(line1.y);
	free(line1.k1);
	free(line1.k2);
	free(line1.a3);
	free(line1.a1);
	free(line1.b3);
	free(line1.b1);

	return PCS_joint_angle;
}




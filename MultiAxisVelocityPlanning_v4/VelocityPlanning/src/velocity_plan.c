//19.07.31
//model:ER3A-C60
#include "../inc/velocity_plan.h"
#include  <malloc.h>
#include "filter.h"

char joint_filename[100] = "data/TrainRandNurbs.ltxt";
char Save_PT_file_name[100] = "data/TrainRandNurbs.ptxt";
char MaxVelocity_file[100] = "MaxVelocity.txt";
char MaxAcceleration_file[100] = "MaxAcceleration.txt";

/*
function:
*/
int unique_unsorted(int* Textlen, Matrix_t* joint_angle)
{

	int column = 0;
	int row = 0;
	int len = 1;
	Matrix_t temp_angle = copy_mat(joint_angle);

	for (row = 0; row < *Textlen - 1; row++)
	{
		if (temp_angle.data[row + 1][0] != temp_angle.data[row][0] || temp_angle.data[row + 1][1] != temp_angle.data[row][1] || temp_angle.data[row + 1][2] != temp_angle.data[row][2] || temp_angle.data[row + 1][3] != temp_angle.data[row][3] || temp_angle.data[row + 1][4] != temp_angle.data[row][4] || temp_angle.data[row + 1][5] != temp_angle.data[row][5])
		{
            for (column = 0; column < Default_dof; column++)
			{
				joint_angle->data[len][column] = temp_angle.data[row + 1][column];
			}
			++len;
		}
	}

	*Textlen = len;

	free_mat(&temp_angle);

	return len;
}

/*
*function: calculate the Coffecients of each section joint, and union length
*input parameters: @
				  @
				 @
*return parameters:None
*/
Matrix_t CalcCoffecient(int len, Matrix_t* joint_angle, double* Length)
{
	int row = 0;
	int column = 0;
	int nrow = len;
	double* length_square = (double*)malloc(sizeof(double) * (nrow - 1));
    Matrix_t K = create_mat(len - 1, Default_dof);

	for (row = 0; row < nrow - 1; row++)
	{
		length_square[row] = 0;
        for (column = 0; column < Default_dof; column++)
		{
			length_square[row] = length_square[row] + pow((joint_angle->data[row][column] - joint_angle->data[row + 1][column]), 2);
		}
		Length[row] = sqrt(length_square[row]);

	}

	for (row = 0; row < nrow - 1; row++)
	{
        for (column = 0; column < Default_dof; column++)
		{
			K.data[row][column] = fabs(joint_angle->data[row][column] - joint_angle->data[row + 1][column]) / Length[row]; //coffecient of each segment
		}
	}

	free(length_square);

	return K;
}
/*
*function:
*/
double my_min(double* arr, int len)
{
	double min_num = arr[0];
	int row = 1;
	for (row = 1; row < len; row++)
	{
		if (arr[row] < min_num)
			min_num = arr[row];
	}

	return min_num;
}

/*
function: calculate the max_joint_union_velocity and max_joint_union_acceleration
*/

int CalcMax_Union_Velocity_Acceleration(int len, Interface_data data, Matrix_t* K, double max_synthetic_velocity[], double max_synthetic_acceleration[], double max_velocity_limit_by_unaxial_error[])
{
	int row = 0;
	int column = 0;
    double* unaxial_velocity_limit = (double*)malloc(sizeof(double) * Default_dof);
    double* unaxial_acceleration_unaxial = (double*)malloc(sizeof(double) * Default_dof);
    double* unaxial_velocity_error = (double*)malloc(sizeof(double) * Default_dof);

	Matrix_t max_joint_velocity = Get_Max_limited(len, data.MaxVelocity_file_name, data.Max_velocity);
	Matrix_t max_joint_acceleration = Get_Max_limited(len, data.MaxAcceleration_file_name, data.Max_acceleration);


	for (row = 0; row < len - 1; row++)
	{
        for (column = 0; column < Default_dof; column++)
		{
			if (row < 100)
			{
				if(unaxial_velocity_limit)
					unaxial_velocity_limit[column] = 0.5 * max_joint_velocity.data[row][column] / K->data[row][column];
				if(unaxial_acceleration_unaxial)
					unaxial_acceleration_unaxial[column] = 0.5 * max_joint_acceleration.data[row][column] / K->data[row][column];
			}
			else
			{
				unaxial_velocity_limit[column] = max_joint_velocity.data[row][column] / K->data[row][column];
				unaxial_acceleration_unaxial[column] = max_joint_acceleration.data[row][column] / K->data[row][column];
			}
		}
        max_synthetic_velocity[row] = my_min(unaxial_velocity_limit, Default_dof);
        max_synthetic_acceleration[row] = my_min(unaxial_acceleration_unaxial, Default_dof);
	}

	for (row = 1; row < len - 1; row++)
	{
        for (column = 0; column < Default_dof; column++)
		{
			if (unaxial_velocity_error)
				unaxial_velocity_error[column] = data.delta_Vmax / fabs(K->data[row - 1][column] - K->data[row][column]); //the max_joint_velocity is determined by error admitted changes
		}
        max_velocity_limit_by_unaxial_error[row] = my_min(unaxial_velocity_error, Default_dof);
	}
	max_velocity_limit_by_unaxial_error[len - 1] = 0;

	free_mat(&max_joint_velocity);
	free_mat(&max_joint_acceleration);
	free(unaxial_velocity_limit);       unaxial_velocity_limit = NULL;
	free(unaxial_acceleration_unaxial); unaxial_acceleration_unaxial = NULL;
	free(unaxial_velocity_error);       unaxial_velocity_error = NULL;

	return 0;
}
/*
function: Use default value to initialize a Interface_data variable
*/
Interface_data Init_Data(void)
{
	Interface_data data;
	data.Dof = Default_dof;
	data.Interpolation_point = Default_Interpolation_points;
	data.LgLookahead = Default_lgLookahead;
	data.Sample_Frequency = Default_Frequency;
	data.delta_Vmax = Default_delta_vmax;
	data.angle_file_name = joint_filename;
	data.save_PT_file_name = Save_PT_file_name;
	data.MaxVelocity_file_name = NULL;
	data.Max_velocity = Default_max_velocity;
	data.MaxAcceleration_file_name = NULL;
	data.Max_acceleration = Default_max_acceleration;

	return data;
}
/*
 * please don't forget to free Matrix_t variables and malloc variables
 */
Matrix_t velocity_plan(Interface_data data)
{
    int NumOfPoint = GetLineNum(data.angle_file_name); 
	Matrix_t joint_angle = get_ref_joint_angle(data.angle_file_name, NumOfPoint); 
	unique_unsorted(&NumOfPoint, &joint_angle); 

    Matrix_t PCS_joint_angle = PCS(data.Dof, data.Interpolation_point, &NumOfPoint, &joint_angle); 
	free_mat(&joint_angle);

	double* Length = (double*)malloc(sizeof(double) * (NumOfPoint - 1));
	Matrix_t K = CalcCoffecient(NumOfPoint, &PCS_joint_angle, Length); 

	double* max_synthetic_velocity = (double*)malloc(sizeof(double) * (NumOfPoint - 1));
	double* max_synthetic_acceleration = (double*)malloc(sizeof(double) * (NumOfPoint - 1));
	double* max_velocity_limit_by_unaxial_error = (double*)malloc(sizeof(double) * NumOfPoint);
	CalcMax_Union_Velocity_Acceleration(NumOfPoint, data, &K, max_synthetic_velocity, max_synthetic_acceleration, max_velocity_limit_by_unaxial_error);
	
	//LookAhead module
	double* synthetic_velocity_lookahead = LookAhead(NumOfPoint, data.LgLookahead, max_synthetic_acceleration, Length, max_synthetic_velocity, max_velocity_limit_by_unaxial_error);

    //Interpolation module
	Matrix_t joint_angle_interpolation = Interpolation(NumOfPoint, data.Dof, data.Sample_Frequency, &PCS_joint_angle, &K, synthetic_velocity_lookahead, max_synthetic_acceleration, max_synthetic_velocity, Length);
	
	//Matrix_t filter_jiont_angle = MeanFilter(&joint_angle_interpolation, Nfilt);  free_mat(&joint_angle_interpolation); 
	Matrix_t filter_jiont_angle = Gaussianfilter(&joint_angle_interpolation, Gaussian_r, Gaussian_sigma);  free_mat(&joint_angle_interpolation);
	//Matrix_t filter_jiont_angle = low_filter(&joint_angle_interpolation, LowFilter_K);  free_mat(&joint_angle_interpolation);
	save_joint_angle_data(data.save_PT_file_name, &filter_jiont_angle);

	free(max_synthetic_velocity);	  max_synthetic_velocity = NULL;
	free(max_synthetic_acceleration); max_synthetic_acceleration = NULL;
	free(max_velocity_limit_by_unaxial_error); max_velocity_limit_by_unaxial_error = NULL;
	free_mat(&PCS_joint_angle);
	free_mat(&K);
	free(Length);						Length = NULL;
	free(synthetic_velocity_lookahead); synthetic_velocity_lookahead = NULL;
	free_mat(&filter_jiont_angle); 

	return joint_angle_interpolation;
}


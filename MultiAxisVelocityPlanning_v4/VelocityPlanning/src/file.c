#include "file.h"
#include "velocity_plan.h"

#ifdef MSVC__COMPILER

int GetLineNum(char* name)
{
	FILE* fp;
	int count = 0, flag = 0;

	errno_t err;
	err = fopen_s(&fp, name, "r");
	if (err)
		printf("GetLineNum: can't open angle file: %s!\n", name);

	while (!feof(fp))
	{
		flag = fgetc(fp);
		if (flag == '\n')
			count++;
	}
	fclose(fp);
	return count;                  //close
}
/*
function:
*/
Matrix_t get_ref_joint_angle(char* name, int len)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, name, "r");
	if (err)
		printf("get_ref_joint_angle: can't open angle file: %s!\n", name);

	Matrix_t angle = create_mat(len, Default_dof);

	for (int row = 0; row < len; row++)
	{
		for (int column = 0; column < Default_dof; column++)
		{
			if(fp)
				fscanf_s(fp, "%lf", &angle.data[row][column]);
		}
	}

	fclose(fp);

	return angle;
}
/*
*
*/
int save_joint_angle_data(char* name, Matrix_t* angle)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, name, "w+");
	if (err)
		printf("save_joint_angle_data: can't open file: %s!\n", name);
	else
		printf("printf data...\n");

	for (unsigned int row = 0; row < angle->row; row++)
	{
		for (unsigned int column = 0; column < angle->column; column++)
		{
			fprintf_s(fp, "%.10lf  ", angle->data[row][column]);
		}
		fprintf_s(fp, "\n");
	}

	fclose(fp);

	return 0;
}
Matrix_t Get_Max_limited(int len, char* name, double Limited)
{
	Matrix_t mat = create_mat(len - 1, Default_dof);

	if (name != NULL)
	{
		FILE* fp;
		errno_t err;
		err = fopen_s(&fp, name, "r");
		if (err)
			printf("Get_Max_limited :can't open file: %s!\n", name);

		for (int row = 0; row < len - 1; row++)
		{
			for (int column = 0; column < Default_dof; column++)
			{
				fscanf_s(fp, "%lf", &mat.data[row][column]);
			}
		}
	}
	else
	{
		for (int row = 0; row < len - 1; ++row)
		{
			for (int column = 0; column < Default_dof; ++column)
			{
				mat.data[row][column] = Limited;
			}
		}
	}

	return mat;
} 

///for gcc compiler

#else 

int GetLineNum(char* name)
{
	FILE* fp;
	int count = 0, flag = 0;
	if ((fp = fopen(name, "r")) == NULL)
	{
		printf("GetLineNum: can't open angle file: %s!\n", name);
		exit(1);
	}
	while (!feof(fp))
	{
		flag = fgetc(fp);
		if (flag == '\n')
			count++;
	}
	fclose(fp);
	return count;                  //close

}
/*
function:
*/
Matrix_t get_ref_joint_angle(char* name, int len)
{
	FILE* fp;
	if ((fp = fopen(name, "r")) == NULL)
	{
		printf("get_ref_joint_angle: can't open angle file: %s!\n", name);
		exit(1);
	}

	Matrix_t angle = create_mat(len, Default_dof);

	for (int row = 0; row < len; row++)
	{
		for (int column = 0; column < Default_dof; column++)
		{
			fscanf(fp, "%lf", &angle.data[row][column]);
		}
	}

	fclose(fp);

	return angle;
}
/*
*
*/
int save_joint_angle_data(char* name, Matrix_t* angle)
{
	FILE* fp;
	if ((fp = fopen(name, "w+")) == NULL)
	{
		printf("save_joint_angle_data: can't open file: %s!\n", name);
		return 0;
	}
	for (unsigned int row = 0; row < angle->row; row++)
	{
		for (unsigned int column = 0; column < angle->column; column++)
		{
			fprintf(fp, "%.10lf  ", angle->data[row][column]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);

	return 0;
}

Matrix_t Get_Max_limited(int len, char* name, double Limited)
{
	Matrix_t mat = create_mat(len - 1, Default_dof);;
	if (name != NULL)
	{
		FILE* fp;
		if ((fp = fopen(name, "r")) == NULL)
		{
			printf("Get_Max_limited :can't open file: %s!\n", name);
			exit(1);
		}

		for (int row = 0; row < len - 1; row++)
		{
			for (int column = 0; column < Default_dof; column++)
			{
				fscanf(fp, "%lf", &mat.data[row][column]);
			}
		}
	}
	else
	{
		for (int row = 0; row < len - 1; ++row)
		{
			for (int column = 0; column < Default_dof; ++column)
			{
				mat.data[row][column] = Limited;
			}
		}
	}

	return mat;
}
#endif // MSVC__COMPILER
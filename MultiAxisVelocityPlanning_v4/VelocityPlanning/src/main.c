/*
 * main.c
 *
 *  Created on: 8.7
 *      Author: 
 */
#include <stdio.h>
#include "../inc/velocity_plan.h"

/*
function : calculate PT
*/
int main()
{
	Matrix_t Inter_Joint_angle;
	Interface_data data = Init_Data();
	Inter_Joint_angle = velocity_plan(data);

	return 0;
}


#include "LookAhead.h"
#include <stdlib.h>
#include <math.h>

double* LookAhead(int len, int LookaheadPoints, double max_synthetic_acceleration[], double Length[], double max_synthetic_velocity[], double max_velocity_limit_by_unaxial_error[])
{
	int i = 0;
	double* b = (double*)malloc(sizeof(double) * len);
	double* c = (double*)malloc(sizeof(double) * len);
	double* d = (double*)malloc(sizeof(double) * len);
	double* speedLimitSquare = (double*)malloc(sizeof(double) * len);
	double* synthetic_velocity_lookahead = (double*)malloc(sizeof(double) * len);

	synthetic_velocity_lookahead[0] = 0;
	synthetic_velocity_lookahead[len - 1] = 0;

	double speedSquareLimitNow;
	double speedBackNow;
	int considerNt, lookAheadFlag;
	int Nt;
	int iterationEndFlag = 0;

	double sum;

	Nt = LookaheadPoints;

	for (i = 0; i < len - 1; i++)
	{
		b[i] = 2 * max_synthetic_acceleration[i] * Length[i];
		d[i] = max_synthetic_velocity[i] * max_synthetic_velocity[i];
		if ((max_velocity_limit_by_unaxial_error[i] * max_velocity_limit_by_unaxial_error[i]) > d[i])
			c[i] = d[i];
		else
			c[i] = max_velocity_limit_by_unaxial_error[i] * max_velocity_limit_by_unaxial_error[i];

	}

	for (i = 0; i < len; i++)
		speedLimitSquare[i] = c[i];


	for (i = 2; i < len; i++)
	{
		int k;

		if (LookaheadPoints > len - i)
			considerNt = len - i;
		else
			considerNt = LookaheadPoints;

		for (k = 1; k < considerNt + 1; k++)
		{
			sum = 0;
			int l = i + 1;
			for (l = i + 1; l < i + k + 1; l++)
				sum += b[l - 1];

			if (speedLimitSquare[i + k - 1] < b[i + k - 1])
			{
				lookAheadFlag = 0; break;
			}

			if (sum > speedLimitSquare[i - 1])
			{
				lookAheadFlag = 1;  break;
			}

			if (k == ((Nt) < (len - i) ? (Nt) : (len - i)))
			{
				lookAheadFlag = 2;  break;
			}
		}


		speedSquareLimitNow = speedLimitSquare[i - 1];
		int calc_count = i + k;
		for (calc_count = i + k; calc_count > i; calc_count--)
		{
			if ((lookAheadFlag == 2) && (calc_count == i + k))
				speedBackNow = 0;
			else
				speedBackNow = speedLimitSquare[calc_count - 1];

			if (speedBackNow >= speedSquareLimitNow)
				continue;
			iterationEndFlag = 0;
			int sum_count = calc_count - 1;
			for (sum_count = calc_count - 1; sum_count > i - 1; sum_count--)
			{
				speedBackNow = speedBackNow + b[sum_count - 1];
				if (speedBackNow >= speedSquareLimitNow)
				{
					iterationEndFlag = 1;  break;
				}
			}

			if (iterationEndFlag == 1)
				continue;
			else
				speedSquareLimitNow = speedBackNow;
		}

		if (speedSquareLimitNow < (synthetic_velocity_lookahead[i - 2] * synthetic_velocity_lookahead[i - 2] + b[i - 2]))
			synthetic_velocity_lookahead[i - 1] = sqrt(speedSquareLimitNow);
		else
			synthetic_velocity_lookahead[i - 1] = sqrt(synthetic_velocity_lookahead[i - 2] * synthetic_velocity_lookahead[i - 2] + b[i - 2]);
	}
	free(b);
	free(c);
	free(d);
	free(speedLimitSquare);

	return synthetic_velocity_lookahead;
}


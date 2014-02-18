#include "pid.h"

double time()
{
	return ((double)tickGet()) / (double)sysClkRateGet();
}

double deltat(double& t)
{
	double dt = time() - t;
	t = time();
	return dt;
}

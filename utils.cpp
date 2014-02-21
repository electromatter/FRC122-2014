#include "utils.h"

#include <vxWorks.h>
#include <tickLib.h>
#include <drv/timer/timerDev.h>

double time()
{
	return ((double)tickGet()) / (double)sysClkRateGet();
}

//TODO: move to macro
double deltat(double& t)
{
	double dt = time() - t;
	t = time();
	return dt;
}

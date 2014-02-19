#include "pid.h"

#include <vxWorks.h>
#include <tickLib.h>
#include <drv/timer/timerDev.h>

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

pid::pid(float sp, float p, float i, float d)
{
	params(sp, p, i, d);
}

pid::~pid()
{
}

void pid::params(float sp, float p, float i, float d)
{
	m_sp = sp;
	m_p = p;
	m_i = i;
	m_d = d;
	reset();
}

float pid::input(float x, float setpoint)
{
	float e = x * m_sp - setpoint;
	float pwm;
	float dt = deltat(m_tlast);
	float de = e - m_last;
	m_last = e;
	if (m_it == 0) {
		pwm = m_p * e;
	} else {
		m_int += e * dt;
		pwm = m_p * e + m_i * m_int + m_d * de;
	}
	m_it++;
	return pwm;
}

void pid::reset()
{
	m_it = 0;
	m_int = 0;
	m_last = 0;
	m_tlast = time();
}

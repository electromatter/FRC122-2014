#ifndef MOTOR_GROUP_H
#define MOTOR_GROUP_H

#include "pid.h"
#include <WPILib.h>

class motorgroup
{
public:
	motorgroup(int pwma, int pwmb, int dir = 0);
	motorgroup(int pwma, int pwmb, int enca, int encb);
	virtual ~motorgroup();
	void speed(float sp);
	void params(float sp = 0, float p = 1, float i = 0, float d = 0);
	void set(float pwm);
	float get();
	void reset();
private:
	float m_sp, m_p, m_i, m_d;
	float m_last;
	float m_int;
	double m_tlast;
	int m_it;
	Talon *m_a, *m_b;
	Encoder* m_enc;
	int m_dir;
};

#endif

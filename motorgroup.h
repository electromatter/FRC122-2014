#ifndef MOTOR_GROUP_H
#define MOTOR_GROUP_H

#include "pid.h"
#include <WPILib.h>

class motorgroup : public pid
{
public:
	motorgroup(int pwma, int pwmb, int dir = 0);
	motorgroup(int pwma, int pwmb, int enca, int encb);
	virtual ~motorgroup();
	void speed(float sp);
	void set(float pwm);
	float get();
	void reset();
private:
	Talon *m_a, *m_b;
	Encoder* m_enc;
	int m_dir;
};

#endif

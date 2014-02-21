#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "a10dof.h"
#include "motorgroup.h"
#include "pid.h"
#include "utils.h"

//TODO: minimalize members
class robot : public SimpleRobot
{
public:
	robot();
	float readtemp();
	void feed();
	void Disabled();
	void OperatorControl();
	void Autonomous();
private:
	motorgroup ma, mb, mc, md, me;
	DigitalInput d;
	Relay r, cmp;
	Solenoid sa, sb, sc;
	float a;
	float sp;
	double t;
	double dt;
	l3gd20 gyro;
	lsm303m mag;
	lsm303a acc;
	Gyro gyrob;
	AnalogChannel tempb;
	AnalogChannel arma;
	AnalogChannel armb;
	pid anglepid;
};

#endif

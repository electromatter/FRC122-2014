#include "WPILib.h"
#include "a10dof.h"
#include "motorgroup.h"
#include "pid.h"
#include <math.h>
#include <tickLib.h>
#include <drv/timer/timerDev.h>

#define PI (3.1415926535897932384626433832795)
#define DEGTORAD(d)	((d) * (PI / 180.))
#define RADTODEG(r)	((r) * (180. / PI))

#define BUILD	(100)

inline float imod(float x, float min, float max)
{
	if (x > min && x < max)
		return x;
	else if (x < min)
		return min;
	else if (x > max)
		return max;
	else // x is not real -> return unreal
		return x;
}

inline float qmod(float x, float min, float max)
{
	float mod = max - min;
	if (x - min < 0)
		return fmod(x + min, mod) - min;
	else
		return fmod(x - min, mod) + min;
}

float angulardrive(float x, float y, float ang, float omega = 0, float radius = 1)
{
	//TODO: make a better system for omega -> however seems to work
	//FIXME: tune omega for the torque of each wheel
	x = imod(x, -1, 1) * 0.75;
	y = imod(y, -1, 1) * 0.75;
	omega = imod(omega, -1, 1) * 0.25
	return omega + x * sin(DEGTORAD(ang)) + y * cos(DEGTORAD(ang));
}

class RobotDemo : public SimpleRobot
{
	motorgroup ma, mb, mc, md, me;
	DigitalInput d;
	Relay r, cmp;
	Solenoid sa, sb, sc;
	float a;
	float r;
	double t;
	double dt;
	l3gd20 gyro;
	lsm303m mag;
	lsm303a acc;
	Gyro gyrob;
	AnalogChannel tempb;

public:
	RobotDemo() : ma(1, 2, 2, 3),
	              mb(3, 4, 4, 5), 
	              mc(5, 6, 6, 7),
	              md(7, 8),
	              me(9, 10, 1),
	              d(1),
	              r(2),
	              cmp(1),
	              sa(1),
	              sb(2),
	              sc(3),
	              a(0),
	              r(0),
	              t(time()),
	              dt(0),
	              gyro(),
	              mag(),
	              acc(),
	              gyrob(1),
	              tempb(2)
	{
		gyro.start();
		acc.start();
		mag.start();
	}
	
	float readtemp()
	{
		//values from datasheet
		return 25 + ((tempb.GetVoltage() - 2.5) / 0.009);
	}
	
	void feed()
	{
		DriverStationLCD *lcd = DriverStationLCD::GetInstance();
		lcd->Clear();
		float th = gyro.getangle();
		a = qmod(th * dt + a, -180, 180);
		lcd->PrintfLine(DriverStationLCD::kUser_Line1, "BUILD: %i", BUILD);
		//lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", gyrob.GetAngle());
		//lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", ma.get());
		//lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%f", mb.get());
		//lcd->PrintfLine(DriverStationLCD::kUser_Line5, "%f", aa);
		lcd->UpdateLCD();
	}
	
	void Disabled()
	{
		while (IsDisabled()) {
			dt = deltat(t);
			feed();
			Wait(0.005);
		}
	}

	void Autonomous()
	{
		//TODO: reset state for autonomous
		while (IsAutonomous()) {
			//TODO: Autonomous. cv?
			dt = deltat(t);
			feed();
			Wait(0.005);
		}
	}

	void OperatorControl()
	{
		//joystick assignments: ja(main x-y drive), jb(turning)
		Joystick ja(1), jb(2), jc(3);
		a = 0;
		r = 0;
		gyrob.Reset();
		ma.reset();
		mb.reset();
		mc.reset();
		while (IsOperatorControl())
		{
			float x = ja.GetRawAxis(1);//ja.x
			float y = -ja.GetRawAxis(2);//ja.y is reversed
			float t = jb.GetRawAxis(1);//jb.y
			
			//solenoids:
			if (ja.GetRawButton(7))//close
				sa.Set(true);
			else
				sa.Set(false);
			
			if (ja.GetRawButton(8))//release
				sb.Set(true);
			else
				sb.Set(false);
			
			if (ja.GetRawButton(9))//open
				sc.Set(true);
			else
				sc.Set(false);
			
			if (!d.Get())//compressor
				cmp.Set(Relay::kForward);
			else
				cmp.Set(Relay::kOff);
			
			//turning
			r = qmod(r + t * dt, -180, 180);
			float c = qmod(gyrob.GetAngle() - r, -180, 180);
			float ang = c * -0.025;//FIXME: tune angle pid -> replace with external pid
			
			x *= 0.75;
			y *= 0.75;
			
			ma.set(angulardrive(x, y, 30 + c, t + ang));
			mb.set(angulardrive(x, y, -90 + c, t + ang));
			mc.set(angulardrive(x, y, 150 + c, t + ang));
			md.set(0);//wind winch -> -1
			me.set(0);//arm
			
			//TODO: arm pid
			
			if (ja.GetRawButton(6))//roller
				r.Set(Relay::kForward);//in
			else if (ja.GetRawButton(4))
				r.Set(Relay::kReverse);//out
			else 
				r.Set(Relay::kOff);
			
			dt = deltat(t);
			feed();
			Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

#include "WPILib.h"
#include "a10dof.h"
#include "motorgroup.h"
#include "pid.h"
#include <math.h>

#define PI (3.1415926535897932384626433832795)
#define DEGTORAD(d)	((d) * (PI / 180.))
#define RADTODEG(r)	((r) * (180. / PI))

#define BUILD	(120)

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
	//FIXME: tune omega for the torque of each wheel
	x = imod(x, -1, 1);
	y = imod(y, -1, 1);
	omega = imod(omega, -1, 1);
	return omega + x * sin(DEGTORAD(ang)) + y * cos(DEGTORAD(ang));
}

class RobotDemo : public SimpleRobot
{
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
	              sp(0),
	              t(time()),
	              dt(0),
	              gyro(),
	              mag(),
	              acc(),
	              gyrob(1),
	              tempb(2),
	              arma(3),
	              armb(4)
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
		lcd->PrintfLine(DriverStationLCD::kUser_Line2, "%f", gyrob.GetAngle());
		lcd->PrintfLine(DriverStationLCD::kUser_Line3, "%f", arma.GetVoltage());
		lcd->PrintfLine(DriverStationLCD::kUser_Line4, "%f", armb.GetVoltage());
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
		//TODO: reset state for autonomous, Autonomous
		while (IsAutonomous()) {
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
		sp = 0;
		gyrob.Reset();
		ma.reset();
		mb.reset();
		mc.reset();
		
		//anglepid.params(1, -0.025, 0, 0);//-0.000375);
		while (IsOperatorControl())
		{
			float x = ja.GetRawAxis(1);//ja.x
			float y = -ja.GetRawAxis(2);//ja.y is reversed
			float theta = jb.GetRawAxis(1);//jb.y
			dt = deltat(t);

			sa.Set(false);
			sb.Set(false);
			sc.Set(false);
			
			//solenoids:
			if (jc.GetRawButton(4))//open
				sa.Set(true);
			else if (jc.GetRawButton(5))//close
				sc.Set(true);
			
			if (jc.GetRawButton(3))//release
				sb.Set(true);
			else
				sb.Set(false);
			
			if (!d.Get())//compressor
				cmp.Set(Relay::kForward);
			else
				cmp.Set(Relay::kOff);
			
			//deadband
			if (theta > -0.111 && theta < 0.111)
				theta = 0;
			
			//turning
			sp = qmod(sp + theta * 1.5, -180, 180);

			if (jb.GetRawButton(3))
				sp = 0;
			if (jb.GetRawButton(4))
				sp = -90;
			if (jb.GetRawButton(2))
				sp = 180;
			if (jb.GetRawButton(5))
				sp = 90;
			
			float c = qmod(gyrob.GetAngle(), -180, 180);
			//FIXME: tune angle pid -> replace with external pid
			float ang = qmod(gyrob.GetAngle() - sp, -180, 180) * -0.025;
			
			if (ja.GetRawButton(11)) {
				a = 0;
				sp = 0;
				gyrob.Reset();
				ma.reset();
				mb.reset();
				mc.reset();
			}
			
			float pwm[3];
			float maxpwm = 0;
			
			pwm[0] = angulardrive(x, y, 30 + c, ang);
			pwm[1] = angulardrive(x, y, -90 + c, ang);
			pwm[2] = angulardrive(x, y, 150 + c, ang);
			
			for (int i = 0; i < 3; i++)
				if (fabs(pwm[i]) > maxpwm)
					maxpwm = pwm[i];

			for (int i = 0; i < 3; i++)
				if (maxpwm > 1)
					pwm[i] /= maxpwm;
			
			ma.set(pwm[0]);
			mb.set(pwm[1]);
			mc.set(pwm[2]);
			
			if (jc.GetRawButton(1))//wind winch -> -1
				md.set(-1);
			else
				md.set(0);

			//TODO: arm pid
			//Arm
			if (!(jc.GetRawAxis(2) < -0.25 && jc.GetRawAxis(2) > 0.25))
			me.set(jc.GetRawAxis(2));
			
			if (jc.GetRawButton(9))//roller
				r.Set(Relay::kReverse);//in
			else if (jc.GetRawButton(11))
				r.Set(Relay::kForward);//out
			else
				r.Set(Relay::kOff);
			
			feed();
			Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

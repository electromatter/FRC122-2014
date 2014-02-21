#include "robot.h"

void robot::OperatorControl()
{
	//joystick assignments: ja(main x-y drive), jb(turning)
	Joystick ja(1), jb(2), jc(3);
	a = 0;
	sp = 0;
	gyrob.Reset();
	ma.reset();
	mb.reset();
	mc.reset();
	t = time();
	//anglepid.params(1, -0.025, 0, 0);//-0.000375);
	while (IsOperatorControl()) {
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
		sb.Set(!!jc.GetRawButton(3));//release
		cmp.Set(!d.Get() ? Relay::kOff : Relay::kForward);//compressor
		//deadband
		if (theta > -0.111 && theta < 0.111)
			theta = 0;
		//turning
		sp = qmod(sp + theta * 1.5, -180, 180);
		//quick turning - needs d tuning
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
		//TODO: correct angles and angulardrive()
		pwm[0] = angulardrive(x, y, 30 + c, ang);
		pwm[1] = angulardrive(x, y, -90 + c, ang);
		pwm[2] = angulardrive(x, y, 150 + c, ang);
		//find max pwm to scale back to |pwm| e [0,1]
		for (int i = 0; i < 3; i++)
			if (fabs(pwm[i]) > maxpwm)
				maxpwm = pwm[i];
		for (int i = 0; i < 3; i++)
			if (maxpwm > 1)
				pwm[i] /= maxpwm;
		ma.set(pwm[0]);
		mb.set(pwm[1]);
		mc.set(pwm[2]);
		md.set(jc.GetRawButton(1) ? -1 : 0);//wind winch -> -1
		//TODO: arm pid
		//Arm deadzone
		if (!(jc.GetRawAxis(2) < -0.25 && jc.GetRawAxis(2) > 0.25))
			me.set(jc.GetRawAxis(2));
		else
			me.set(0);
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

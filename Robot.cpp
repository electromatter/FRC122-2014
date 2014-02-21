#include "robot.h"

robot::robot()
	: ma(1, 2, 2, 3),
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

//should move this out to a macro	
float robot::readtemp()
{
	//values from datasheet
	return 25 + ((tempb.GetVoltage() - 2.5) / 0.009);
}

//should move this to helper function
void robot::feed()
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

void robot::Disabled()
{
	while (IsDisabled()) {
		dt = deltat(t);
		feed();
		Wait(0.005);
	}
}

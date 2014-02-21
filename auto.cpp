#include "robot.h"

void robot::Autonomous()
{
	while (IsAutonomous()) {
		dt = deltat(t);
		feed();
		Wait(0.005);
	}
}

#include <signal.h>
#include "BrickPi3.h"
#include "Sensors.h"
#include "UserControl.h"
#include <iostream>

// Variables
BrickPi3 BP;
bool USER_CONTROLLED = false;

// Functions
void invokeMainloop();
void exit_signal_handler(int signo);

// Main
int main()
{
	signal(SIGINT, exit_signal_handler);
	BP.detect();
	BP.set_motor_limits(PORT_B, 50, 0);
	BP.set_motor_limits(PORT_C, 50, 0);
	initializeSensors(BP);
	updateSensors(BP);
	sleep(5);
	invokeMainloop();
	return 0;
}

// Main loop
void invokeMainloop()
{
	bool secondTime = false;
	int lane = 0;
	updateSensors(BP);
	while (true)
	{
		updateSensors(BP);
		if (USER_CONTROLLED) // Afstandsbediening
		{
			//updateUserControls(BP);
			//freeMode(BP);
			avoid(BP);
		}
		else // Vrije modus
		{
			
			patrol(BP);
			lane += edge(BP, lane);
			std::cout << '\n' << "lane " << lane;
			//balldetection(BP);
			//secondTime = true;
		}
	}
}

// Program exit
void exit_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		BP.reset_all();
		exit(-2);
	}
}

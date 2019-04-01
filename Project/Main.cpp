#include <signal.h>
#include "BrickPi3.h"
#include "Sensors.h"
#include "UserControl.h"

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
	invokeMainloop();
	return 0;
}

// Main loop
void invokeMainloop()
{
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
			
			followLine(BP);
			avoid(BP);
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

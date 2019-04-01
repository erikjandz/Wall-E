#include <string>
#include <iostream>
#include "BrickPi3.h"

// Variables
int leftMotorSpeed  = 0;
int rightMotorSpeed = 0;

// Afstandsbediening
void updateUserControls(BrickPi3 & BP)
{
	int speedStep = 20;
	std::string chars = "UPDOWNLEFGHTIR";
	std::string temp;
	std::getline(std::cin, temp);

	// Rare tekens in string verwijderen
	std::string input;
	for (auto & character : temp)
	{
		for (auto & i : chars)
		{
			if (character == i)
			{
				input += character;
			}
		}
	}

	//std::cout << input << std::endl;

	// Update movement
	if (input == "UP")
	{
		leftMotorSpeed += speedStep;
		rightMotorSpeed += speedStep;
	}

	if (input == "DOWN")
	{
		leftMotorSpeed -= speedStep;
		rightMotorSpeed -= speedStep;
	}

	if (input == "LEFT")
	{
		leftMotorSpeed -= speedStep;
		rightMotorSpeed += speedStep;
	}

	if (input == "RIGHT")
	{
		leftMotorSpeed += speedStep;
		rightMotorSpeed -= speedStep;
	}

	if (input == "FIRE")
	{
		leftMotorSpeed = 0;
		rightMotorSpeed = 0;
	}

	BP.set_motor_power(PORT_C, leftMotorSpeed);
	BP.set_motor_power(PORT_B, rightMotorSpeed);
}

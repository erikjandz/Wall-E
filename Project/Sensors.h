#include "BrickPi3.h"
#include <iostream>
#include "unistd.h"
// Sensors
sensor_color_t      SENSOR_COLOR;
sensor_ultrasonic_t SENSOR_USONIC;
sensor_light_t      SENSOR_LIGHT;

// Variables
int RED, GREEN, BLUE;
int DISTANCE;
int LIGHT;

// Sensors setup
void initializeSensors(BrickPi3 & BP)
{
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
}

// Sensors update
void updateSensors(BrickPi3 & BP)
{
	//std::cout << "I got this far" << std::endl;
	if (BP.get_sensor(PORT_1, SENSOR_COLOR) == 0)
	{

		RED = SENSOR_COLOR.reflected_red;
		GREEN = SENSOR_COLOR.reflected_green;
		BLUE = SENSOR_COLOR.reflected_blue;
		//std::cout << "Color: " << RED << " " << GREEN << " " << BLUE << std::endl;
	}

	if (BP.get_sensor(PORT_2, SENSOR_USONIC) == 0)
	{
		DISTANCE = SENSOR_USONIC.cm;
		//std::cout << "Distance: " << DISTANCE << std::endl;
	}

	if (BP.get_sensor(PORT_3, SENSOR_LIGHT) == 0)
	{
		LIGHT = SENSOR_LIGHT.reflected;
		//std::cout << "Light: " << LIGHT << std::endl;
	}
	//std::cout << "\r" << "Colour: " << RED << " " << GREEN << " " << BLUE << "   " << "Distance: " << DISTANCE << "    " <<  "Light: " << LIGHT;
}

void freeMode(BrickPi3 & BP)
{
	if (DISTANCE <= 15) // Object dat in de weg staat
	{
		BP.set_motor_power(PORT_B, 0);
		BP.set_motor_power(PORT_C, 0);
	}
}

void avoid(BrickPi3 & BP)
{
	if (DISTANCE <= 19 && BLUE - GREEN >= 10 && BLUE - RED >= 10 && DISTANCE > 0) //Gekleurd object dat in de weg staat
	{
		BP.set_motor_power(PORT_B, 0);
		BP.set_motor_power(PORT_C, 0);
		sleep(1);
		BP.set_motor_position_relative(PORT_B, -600);
		BP.set_motor_position_relative(PORT_C, 600);
		sleep(1);
		BP.set_motor_power(PORT_C, 20);
		BP.set_motor_power(PORT_B, 20);
		sleep(3);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
		BP.set_motor_position_relative(PORT_B, 600);
		BP.set_motor_position_relative(PORT_C, -600);
		sleep(1);
		BP.set_motor_power(PORT_C, 20);
		BP.set_motor_power(PORT_B, 20);
		sleep(6);
		BP.set_motor_power(PORT_B, 0);
		BP.set_motor_power(PORT_C, 0);
		sleep(1);
		BP.set_motor_position_relative(PORT_B, 1200);
		BP.set_motor_position_relative(PORT_C, -1200);
		sleep(1);
		BP.set_motor_power(PORT_C, 20);
		BP.set_motor_power(PORT_B, 20);
		sleep(2);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
		BP.set_motor_position_relative(PORT_B, -600);
		BP.set_motor_position_relative(PORT_C, 600);
		sleep(2);
	}
}

// Movement functions

void moveForward(BrickPi3 & BP, int & degrees)
{
	BP.set_motor_position_relative(PORT_B, degrees);
	BP.set_motor_position_relative(PORT_C, degrees);
}

void turnRight(BrickPi3 & BP)
{
	BP.set_motor_position_relative(PORT_B, -560);
	BP.set_motor_position_relative(PORT_C, 560);
}

void turnLeft(BrickPi3 & BP)
{
	BP.set_motor_position_relative(PORT_B, 560);
	BP.set_motor_position_relative(PORT_C, -560);
}

void turnRound(BrickPi3 & BP)
{
	BP.set_motor_position_relative(PORT_B, 1200);
	BP.set_motor_position_relative(PORT_C, -1200);
}

void stopMoving(BrickPi3 & BP)
{
	BP.set_motor_power(PORT_B, 0);
	BP.set_motor_power(PORT_C, 0);
}


// Sensors using
void followLine(BrickPi3 & BP)
{
	if (DISTANCE <= 7) // Object dat in de weg staat
	{
		BP.set_motor_power(PORT_B, 0);
		BP.set_motor_power(PORT_C, 0);
	}
	else // Lijn volgen
	{
		int speed = 25;
		int lightspeed = abs((1800 - LIGHT) / 10);
		int leftMotorSpeed;
		int rightMotorSpeed;

		if (lightspeed > 60)
		{
			lightspeed = 60;
		}

		if (LIGHT < 1800)
		{
			lightspeed *= 3;
			leftMotorSpeed = speed - lightspeed;
			rightMotorSpeed = speed + lightspeed;
		}

		if (LIGHT >= 1800)
		{
			leftMotorSpeed = speed + lightspeed;
			rightMotorSpeed = speed - lightspeed;
		}

		BP.set_motor_power(PORT_C, leftMotorSpeed);
		BP.set_motor_power(PORT_B, rightMotorSpeed);
	}
}

void patrol(BrickPi3 & BP)
{
	int speed = 25;
	BP.set_motor_power(PORT_C, speed);
	BP.set_motor_power(PORT_B, speed);
}

int edge(BrickPi3 & BP, int lane)
{
	int degrees = 600;
	if(LIGHT > 2000 && lane % 2 == 0)
	{
		stopMoving(BP);
		sleep(2);
		turnRight(BP);
		sleep(2);
		moveForward(BP, degrees);
		sleep(2);
		turnRight(BP);
		sleep(2);
		//secondTime = true;
		return 1;
	}
	else if(LIGHT > 2000)
	{
		stopMoving(BP);
		sleep(2);
		turnLeft(BP);
		sleep(2);
		moveForward(BP, degrees);
		sleep(2);
		turnLeft(BP);
		sleep(2);
		return 1;
	}
	else
	{
		return 0;
	}
}



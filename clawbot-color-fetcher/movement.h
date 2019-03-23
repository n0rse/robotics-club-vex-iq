#pragma config(Sensor, port4,    gyroscope,      sensorVexIQ_Gyro)
#pragma config(Sensor, port7,    distanceSensor, sensorVexIQ_Distance)
#pragma config(Sensor, port8,    clawBumper,     sensorVexIQ_Touch)
#pragma config(Sensor, port9,    frontBumper,    sensorVexIQ_Touch)
#pragma config(Motor,  motor1,   rightMotor,     tmotorVexIQ, openLoop,   reversed, encoder)
#pragma config(Motor,  motor6,   leftMotor,      tmotorVexIQ, openLoop,   encoder)
#pragma config(Motor,  motor10,  armMotor,       tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor11,  clawMotor,      tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*------------------------------------------------------------------------------------------------
Move forward until the VEX IQ Distance Sensors sees an object 100 millimeters (mm) away. The VEX
IQ Distance Sensor returns values in millimeters.

ROBOT CONFIGURATION: VEX IQ Clawbot
MOTORS & SENSORS:
[I/O Port]          [Name]              [Type]                  [Info]
Port 7              distanceSensor      VEX IQ Distance         Distance Sensor (returns mm)
Port 1              rightMotor          VEX IQ Motor            Left side motor (reversed)
Port 6              leftMotor           VEX IQ Motor            Right side motor
------------------------------------------------------------------------------------------------*/
const int ACTUATOR_DELAY = 300; // in ms
const int SENSOR_DELAY = 100; // in ms
const int MAX_SPEED = 75;
const float ACCEL_RATE = 0.01; // range: (0.0, 1.0]
const int ACCEL_TIME = 5; // in ms

static int currSpeed = 0;

float max (float x, float y)
{
	return (x > y) ? x : y;
}

float cosInterpolate(float y1, float y2, float mu)
{
	float mu2 = (1 - cos(mu * PI)) / 2;
	return (y1 * (1 - mu2) + y2 * mu2);
}

// Function returns error code -1 if rate is not within the range (0.0, 1.0].
int gradualAcceleration(int startSpeed, int finalSpeed, float rate,  int incTime)
{
	if (rate <= 0.0 || rate > 1.0)
	{
		return -1;
	}

	float i = 0.0;
	while (i < 1.0)
	{
		setMotorSpeed(leftMotor, (int) round(cosInterpolate(startSpeed, finalSpeed, i)) );
		setMotorSpeed(rightMotor, (int) round(cosInterpolate(startSpeed, finalSpeed, i)) );
		i += rate;
		sleep(incTime);
	}
	setMotorSpeed(leftMotor, (int) round(cosInterpolate(startSpeed, finalSpeed, 1.0)) );
	setMotorSpeed(rightMotor, (int) round(cosInterpolate(startSpeed, finalSpeed, 1.0)) );

	currSpeed = finalSpeed;
	return 0;
}

void turnRight(float degrees)
{
	degrees = abs(degrees) * -1.0;
	float offset = 3.0; // accounts for motor stop delay.

	resetGyro(gyroscope);
	while(getGyroDegreesFloat(gyroscope) > degrees + offset)
	{
		setMotorSpeed(leftMotor, 100);
		setMotorSpeed(rightMotor, -100);
	}
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);

	currSpeed = 0;
}

void turnLeft(float degrees)
{
	degrees = abs(degrees);
	float offset = 3.0; // accounts for motor stop delay.

	resetGyro(gyroscope);
	while(getGyroDegreesFloat(gyroscope) < degrees - offset)
	{
		setMotorSpeed(leftMotor, -100);
		setMotorSpeed(rightMotor, 100);
	}
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);

	currSpeed = 0;
}

void moveForward(int targetSpeed)
{
	targetSpeed = abs(targetSpeed);

	if (targetSpeed > MAX_SPEED)
	{
		targetSpeed = MAX_SPEED;
	}
	else if (targetSpeed == 0)
	{
		targetSpeed = 1;
	}

	if(currSpeed != targetSpeed)
	{
		gradualAcceleration(0, targetSpeed, ACCEL_RATE, ACCEL_TIME);
	}
	setMotorSpeed(leftMotor, targetSpeed);
	setMotorSpeed(rightMotor, targetSpeed);
	currSpeed = targetSpeed;
}

void moveBackward(int targetSpeed)
{
	targetSpeed = abs(targetSpeed) * -1;

	if (targetSpeed < -MAX_SPEED)
	{
		targetSpeed = -MAX_SPEED;
	}
	else if (targetSpeed == 0)
	{
		targetSpeed = -1;
	}

	if(currSpeed != targetSpeed)
	{
		gradualAcceleration(0, targetSpeed, ACCEL_RATE, ACCEL_TIME);
	}
	setMotorSpeed(leftMotor, targetSpeed);
	setMotorSpeed(rightMotor, targetSpeed);
	currSpeed = targetSpeed;
}

void stopMotion()
{
	gradualAcceleration(currSpeed, 0, ACCEL_RATE, ACCEL_TIME);
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
	currSpeed = 0;
}

void stopMotionImmediate()
{
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
	currSpeed = 0;
}

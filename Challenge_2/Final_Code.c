#pragma config(Sensor, S1,     touch,          sensorEV3_Touch)
#pragma config(Sensor, S2,     gyro,           sensorEV3_Gyro)
#pragma config(Sensor, S3,     color,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     sonar,          sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          arm,           tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,           ,             tmotorEV3_Large, openLoop, driveLeft, encoder)
#pragma config(Motor,  motorC,           ,             tmotorEV3_Large, openLoop, driveRight, encoder)
//* !!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


/* Global variables */
int wall;
int position;
int first = 0; /* To see that emptyOrRedRoom function is only called once. */

 /* ************************************** Function ENDS ************************************** */

void gyroTurnLeft(int d) {
	resetGyro(S2);
	repeatUntil(getGyroDegrees(S4) < d) {
		motor[motorC] = 20;
		motor[motorB] = -20;
	}

	motor[motorC] = 0;
	motor[motorB] = 0;
}

 /* ************************************** Function ENDS ************************************** */

void gyroTurnRight(int d) {
	resetGyro(S2);
	repeatUntil(getGyroDegrees(S4) > d) {
		motor[motorC] = -20;
		motor[motorB] = 20;
	}

	motor[motorC] = 0;
	motor[motorB] = 0;
}

 /* ************************************** Function ENDS ************************************** */

void movementRobot(int speed, long int runTime) {
	motor[motorB] = speed;
	motor[motorC] = speed;
	wait1Msec(runTime);
}

 /* ************************************** Function ENDS ************************************** */

void pauseMovement() {
	motor[motorB] = 0;
	motor[motorC] = 0;
	wait1Msec(1000);
}

 /* ************************************** Function ENDS ************************************** */

/* Helps detect whether the movement should be clockwise or anti-clockwise */
void isWallOnRight() {

	if(SensorValue(S4)<20)
		wall = 0;
    else {
		playSound(soundUpwardTones);
		wall = 1;
	}
}

 /* ************************************** Function ENDS ************************************** */

void emptyOrRedRoom() {
	playSound(soundBlip);

	while(SensorValue(S1) == 0 && getColorName(S3)!= colorRed)
		movementRobot(25,0);

	if (getColorName(S3) == colorRed) {
		//reaches the fire and backs away a bit.
		movementRobot(-15,0);
		sleep(1300);

		// turns 180 degrees
		resetGyro(S2);
		repeatUntil(getGyroDegrees(S2) > 170) {
			setMotorSpeed(motorC,-20);
			setMotorSpeed(motorB, 20);
		}

		//reverses into the fire
		movementRobot(-15,0);
		sleep(5000);


		// makes a 90 degree right turn (gyro value was 180 before)
		if(wall==0) {
			resetGyro(S2);
			repeatUntil(getGyroDegrees(S2) > 85) {
				//Point turn to the left
				setMotorSpeed(motorC,-20);
				setMotorSpeed(motorB, 20);
			}
			movementRobot(25, 1500);
		}
		else {
			resetGyro(S2);
			repeatUntil(getGyroDegrees(S2) < -80) {
				//Point turn to the left
				setMotorSpeed(motorC,20);
				setMotorSpeed(motorB,-20);
			}
		}

		if(first == 0) {
			position=5+wall;
			first++;
		}
		movementRobot(25, 1500);
	}

	/* CASE FOR THE EMPTY ROOM */
	else {
		movementRobot(-25,0);
		sleep(500);

		//Resets the gyro on port 2 to 0 degrees
		resetGyro(S2);

		// CHANGED IT FROM -70 TO -80
		if(wall == 0) {
			repeatUntil(getGyroDegrees(S2) < -70) {
				//Point turn to the left
				setMotorSpeed(motorC, 50);
				setMotorSpeed(motorB, -50);
			}
		}
		else {
			repeatUntil(getGyroDegrees(S2) > 75) {
				//Point turn to the left
				setMotorSpeed(motorC, -50);
				setMotorSpeed(motorB, 50);
			}
		}

		//Stop the motors at the end of the turn
		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 0);

		if(first == 0) {
			position=1+wall;
			first++;
		}
	}
}

 /* ************************************** Function ENDS ************************************** */

/* This code tells us what starting point we are at 1,2,3 or 4 */
void startingPosition() {
	int sum=0;
	int distance1,distance2;

	distance1 = SensorValue(S4);

	resetGyro(S2);
	//turns 180 degrees to check the value for opposite side
	repeatUntil(getGyroDegrees(S2) > 85) {
		setMotorSpeed(motorC,-25);
		setMotorSpeed(motorB, 25);
	}

	setMotorSpeed(motorC,0);
	setMotorSpeed(motorB,0);
	sleep(2000);

	isWallOnRight();

	resetGyro(S2);

	//turns 180 degrees to check the value for opposite side
	repeatUntil(getGyroDegrees(S2) > 85) {
		setMotorSpeed(motorC,-25);
		setMotorSpeed(motorB, 25);
	}
	setMotorSpeed(motorC,0);
	setMotorSpeed(motorB,0);
	sleep(2000);

	distance2= SensorValue(S4);
	sum=distance1+distance2;

	resetGyro(S2);
	repeatUntil(getGyroDegrees(S2) > 180) {
		setMotorSpeed(motorC,-25);
		setMotorSpeed(motorB, 25);
	}
	setMotorSpeed(motorC,0);
	setMotorSpeed(motorB,0);
	sleep(2000);


	//the sum of the distances is different from each point hence helping us distinguish the points we are starting at.


	if(sum>100) {
	   if(wall==0)
			emptyOrRedRoom();
	   else if(distance1<40)
			position=6;
	   else
			position=2;
	}

	else if(sum>40 && sum <60)
		position=3+wall;
	else if(sum>60 && sum<80)
		position=7+wall;
	else
		startingPosition();
}

 /* ************************************** Function ENDS ************************************** */

void boxRoom() {

	while(SensorValue(S4) > 15)
		movementRobot(20, 0);

	playSound(soundException);


/* If wall is to the right or the robot is going anticlockwise */
	if(wall == 0) {
		resetGyro(S2);

		//Turn 90 degrees to the left
		repeatUntil(getGyroDegrees(S2) < -80) {
			setMotorSpeed(motorC, 20);
			setMotorSpeed(motorB, -20);
		}

		movementRobot(25, 3000);
		resetGyro(S2);

		//Turn 90 degrees to the right
		repeatUntil(getGyroDegrees(S2) > 85) {
			setMotorSpeed(motorC, -25);
			setMotorSpeed(motorB, 25);
		}

		while(SensorValue(S1) == 0)
			movementRobot(20, 0);

		resetGyro(S2);

		//Turn 90 degrees to the left
		repeatUntil(getGyroDegrees(S2) < -80) {
			setMotorSpeed(motorC, 20);
			setMotorSpeed(motorB, -20);
		}
	}

/* If the wall is to the left or the robot is going clockwise */
	else {

		resetGyro(S2);

		//Turn 90 degrees to the right
		repeatUntil(getGyroDegrees(S2) > 85) {
			setMotorSpeed(motorC, -20);
			setMotorSpeed(motorB, 20);
		}

		movementRobot(20, 3000);
		resetGyro(S2);

		//Turn 90 degrees to the left
		repeatUntil(getGyroDegrees(S2) < -60) {
			setMotorSpeed(motorC, 25);
			setMotorSpeed(motorB, -25);
		}

		while(SensorValue(S1) == 0)
			movementRobot(20, 0);

		movementRobot(-20, 1000);
		resetGyro(S2);

		//Turn 90 degrees to the right
		repeatUntil(getGyroDegrees(S2) > 85) {
			setMotorSpeed(motorC, -20);
			setMotorSpeed(motorB, 20);
		}
	}

	/* Go to the next room */
	movementRobot(25, 1500);
}

 /* ************************************** Function ENDS ************************************** */

void survivorRoom() {

	while(SensorValue(S4) > 10)
		movementRobot(20, 0);

	movementRobot(-13, 570);
	moveMotor(arm, -180, degrees, 40);
	sleep(300);

	movementRobot(20, 1500);
	moveMotor(arm, 180, degrees, 40);

	/* Added the following if else statement */
    /* If wall is to the right */
	if(wall == 0) {
		resetGyro(S2);

		//Full 90 to the left
		repeatUntil(getGyroDegrees(S2) <-60) {
			setMotorSpeed(motorC, 20);
			setMotorSpeed(motorB, -20);
		}

		setMotorSpeed(motorC, 0);
		setMotorSpeed(motorB, 0);
		sleep(2000);

		//Go to the next room
		movementRobot(30, 4000);
	}

	else {
		resetGyro(S2);

		//Turn 90 degrees to the right
		repeatUntil(getGyroDegrees(S2) > 85) {
			setMotorSpeed(motorC, -20);
			setMotorSpeed(motorB, 20);
		}

		setMotorSpeed(motorC, 0);
		setMotorSpeed(motorB, 0);
		sleep(2000);

		//Move to the next room
		movementRobot(30, 2000);
	}
}

 /* ************************************** Function ENDS ************************************** */

task main() {

	//function call to find the staring position on the board
	startingPosition();

	//function call to determine where the wall is relative to the robot.
	switch(position) {
        case 1: boxRoom();
                emptyOrRedRoom();
                survivorRoom();
                break;

        case 2: survivorRoom();
                emptyOrRedRoom();
                boxRoom();
                emptyOrRedRoom();
                break;

        case 3: boxRoom();
                emptyOrRedRoom();
                survivorRoom();
                emptyOrRedRoom();
                break;

        case 4: emptyOrRedRoom();
                survivorRoom();
                emptyOrRedRoom();
                boxRoom();
                break;

        case 5: survivorRoom();
                emptyOrRedRoom();
                boxRoom();
                break;

        case 6: boxRoom();
                emptyOrRedRoom();
                survivorRoom();
                emptyOrRedRoom();
                break;

        case 7: survivorRoom();
                emptyOrRedRoom();
                boxRoom();
                emptyOrRedRoom();
                break;

        case 8: emptyOrRedRoom();
                boxRoom();
                emptyOrRedRoom();
                survivorRoom();
                break;

        default: break;
    }
}

// Link for video https://photos.app.goo.gl/RybuFTHrEhEuz3Ru8

 /* ************************************** CODE ENDS ************************************** */




void emptyRoom() {
		boxRoom();
		personRoom();
}

void boxRoom() {
		emptyRoom();
		fireRoom();
}

void fireRoom() {
	  personRoom();
	  boxRoom();
}

void personRoom() {
	fireRoom();
	emptyRoom();
}

void flagCount() {
	movementFlag++;
}

void movementRobot(int speed, long int runTime) {
	motor[motorB] = speed;
	motor[motorC] = speed;
	wait1Msec(runTime);
}

int movementFlag = 0;

task main()
{
		if(getColorName(S3) == colorBlack)
				flagCount();





	}


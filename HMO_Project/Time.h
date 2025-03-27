#pragma once

typedef struct {

	int hour;
	int minute;


}Time;

void initTime(Time* time);
int checkIfTimeValid(Time time);
void printTime(Time time);
int isTimesEqual(const Time t1, const Time t2);
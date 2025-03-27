#define _CRT_SECURE_NO_WARNINGS
#include "Time.h"
#include <stdio.h>

void initTime(Time* time) {
	int hour, minute, scanResult;
	char buffer[256];
	do {
		printf("Enter desired time (hh:mm): ");
		scanResult = scanf("%d:%d", &hour, &minute);
		if (scanResult != 2) {
			printf("Invalid input. Use the format hh:mm.\n");
			scanf("%255s", buffer);
		}
		else if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
			printf("Invalid time. Use 00-23 for hour, 00-59 for minute.\n");
			scanResult = 0;
		}
		else {
			time->hour = hour;
			time->minute = minute;
		}
		while (getchar() != '\n');
	} while (scanResult != 2 || checkIfTimeValid(*time) == 0);
}

int checkIfTimeValid(Time time) {
	int flag = 1;
	if (time.hour > 23 || time.hour < 0) {
		printf("\nHour in invalid! // [0-23]\n");
		flag = 0;
	}
	if (time.minute > 59 || time.minute < 0) {
		printf("Minute in invalid! // [0-59]\n\n");
		flag = 0;
	}
	return flag;

}


void printTime(Time time) {
	printf("Time: ");
	if (time.hour < 10)
		printf("0%d:", time.hour);
	else
		printf("%d:", time.hour);
	if (time.minute < 10)
		printf("0%d", time.minute);
	else
		printf("%d", time.minute);
}

int isTimesEqual(const Time t1, const Time t2) {
	if (t1.hour == t2.hour&&t1.minute == t2.minute)
		return 1;
	return 0;
}
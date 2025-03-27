#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Date.h"
#include <string.h>
#include <stdlib.h>
#include "General.h"

int isValidDate(Date date) {
	if (date.year < 2024) return 0;
	if (date.month < 1 || date.month > 12) return 0;
	if (date.day < 1) return 0;

	int daysInMonth;
	switch (date.month) {
	case 4: case 6: case 9: case 11:
		daysInMonth = 30;
		break;
	case 2:
		daysInMonth = 28;
		break;
	default:
		daysInMonth = 31;
		break;
	}
	return date.day <= daysInMonth;
}

void printDate(const Date* date) {

	if (!date)
		return;
	printf("%d/%d/%d", date->day, date->month, date->year);

}
int isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
	switch (month) {
	case 1:  // January
	case 3:  // March
	case 5:  // May
	case 7:  // July
	case 8:  // August
	case 10: // October
	case 12: // December
		return 31;
	case 4:  // April
	case 6:  // June
	case 9:  // September
	case 11: // November
		return 30;
	case 2:  // February
		return isLeapYear(year) ? 29 : 28;
	default:
		return -1;
	}
}
int initDate(Date* date) {
	if (date == NULL) {
		return -1;
	}

	char buffer[MAX_STR_LEN];
	char* sDate = (char*)malloc(DATE_LEN);
	if (!sDate) {
		printf("\nallocation failed\n");
		return -1;
	}


	while (1) {

		printf("\nPlease enter date in format dd^^mm^^yyyy: ");
		if (gets_s(buffer, MAX_STR_LEN) == NULL) {
			printf("Error reading input.\n");
			free(sDate);
			return -1;
		}

		strcpy(sDate, buffer);

		if (!isDateFormat(sDate, &date->year, &date->month, &date->day)) {
			printf("Invalid date format. Please try again.\n");
			continue;
		}

		if (date->month < 1 || date->month > 12) {
			printf("Invalid month. Please try again.\n");
			continue;
		}
		if (date->day < 1 || date->day > 31) {
			printf("Invalid day. Please try again.\n");
			continue;
		}
		if (date->year < 1900 || date->year>2060) {

			printf("Invalid year. Please try again. //>> valid range [1900-2025]\n");
			continue;
		}


		int maxDays = daysInMonth(date->month, date->year);
		if (date->day < 1 || date->day > maxDays) {
			printf("Invalid day for the given month. Please try again.\n");
			continue;
		}
		if (sDate)
			free(sDate);
		return 0;
	}
}

int isDateFormat(char* str, int* year, int* month, int* day) {
	if (strlen(str) != 12) {
		return 0;

	}

	if (str[2] != '^' || str[3] != '^' || str[6] != '^' || str[7] != '^') {
		return 0;
	}

	char dayStr[3], monthStr[3], yearStr[5];

	strncpy(dayStr, str, 2);
	dayStr[2] = '\0';
	strncpy(monthStr, str + 4, 2);
	monthStr[2] = '\0';
	strncpy(yearStr, str + 8, 4);
	yearStr[4] = '\0';

	*day = atoi(dayStr);
	*month = atoi(monthStr);
	*year = atoi(yearStr);


	return 1;
}
int areDatesEqual(const Date* date1, const Date* date2) {
	if (date1 == NULL || date2 == NULL) {
		return 0; // Handle null pointers, consider them not equal
	}

	return (date1->day == date2->day &&
		date1->month == date2->month &&
		date1->year == date2->year);
}

int compareDate(const Date* a, const Date* b) {
	if (a->year != b->year) {
		return a->year - b->year;
	}

	// If years are the same, compare months
	if (a->month != b->month) {
		return a->month - b->month;
	}

	// If months are the same, compare days
	return a->day - b->day;
}
#pragma once

typedef struct {
	int day;
	int month;
	int year;
} Date;

int areDatesEqual(const Date* date1, const Date* date2);
int isValidDate(Date date);
void printDate(const Date* date);
int isLeapYear(int year);
int daysInMonth(int month, int year);
int initDate(Date* date);
int isDateFormat(char* str, int* year, int* month, int* day);
int compareDate(const Date* a, const Date* b);
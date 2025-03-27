#pragma once
#include "General.h"

typedef struct {
	char* name;
	int id;
	int seniority_years;
	char gender;
} HMO_Admin;

int initHMOAdmin(HMO_Admin* admin);
void printHMOAdmin(const HMO_Admin* admin);
void freeHMOAdmin(HMO_Admin* admin);

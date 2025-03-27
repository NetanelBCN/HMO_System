#pragma once

#include "General.h"

typedef struct {
	char* name;
	int id;
	char licence_id[L_ID_MAX];
} Pharmacist;

int initPharmacist(Pharmacist** pharmacist);
void printPharmacist(const Pharmacist* pharmacist);
int isPharmacistsEqual(const Pharmacist* p1, const Pharmacist* p2);
int addPharmacistToArray(Pharmacist*** pharmacists, Pharmacist* newP, int size);
void freePharmacist(Pharmacist** ph);
void printPharmacists(const Pharmacist** pharmacists, int size);
int findPharmacistById(const Pharmacist** p, int id, int size);
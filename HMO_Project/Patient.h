#pragma once
#include "Date.h"

typedef struct {
	int id;
	char* name;
	Date birthDate;
	char gender;
}Patient;

int initPatient(Patient** patient);
void printPatient(const Patient* patient);
void printPatients(const Patient** patient, int size);
int isPatientsEqual(const Patient* p1, const Patient* p2);
int isPatientExist(const Patient** patients, int patientsNum, int id);
const Patient* findPatientById(const Patient** patients, int numPatients, int id);
int addPatientToArray(Patient*** patients, Patient* newP, int size);
void freePatient(Patient** patient);
int searchPatientById(const Patient** patients, int id, int size);
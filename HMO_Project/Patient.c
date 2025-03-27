#define _CRT_SECURE_NO_WARNINGS
#include "Patient.h"
#include "General.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int initPatient(Patient** patient) {
	if (!patient) {
		printf("\nError: Invalid patient pointer!\n");
		return -1;
	}
	Patient* p = *patient;

	char buffer[MAX_STR_LEN];
	p->id = 0;
	p->id = getID();
	printf("\nEnter the patient's full name:\n");
	gets_s(buffer, MAX_STR_LEN);
	p->name = (char*)calloc((int)strlen(buffer) + 1, sizeof(char));
	if (!p->name) {
		printf("\nallocation error\n");
		return -1;
	}
	strcpy(p->name, buffer);
	if (initDate(&p->birthDate) == -1) {
		printf("\nError: Failed to initialize birth date!\n");
		free(p->name);
		free(p);  // Free the patient struct if date initialization fails
		return -1;
	}
	p->gender = getGender();
	return 1;
}
void printPatient(const Patient* patient) {
	if (!patient) {
		printf("Error: Invalid patient pointer\n");
		return;
	}
	printf("---------------------------\n");
	printf("ID: %d\n", patient->id);
	printf("Name: %s\n", patient->name);
	printf("Birth Date: ");
	printDate(&patient->birthDate);
	printf(" | Gender: %c\n", patient->gender);
	printf("----------------------------\n");
}
int isPatientsEqual(const Patient* p1, const Patient* p2) {
	if (p1 == NULL || p2 == NULL) {
		return -1;
	}
	return p1->id - p2->id;
}
int isPatientExist(const Patient** patients, int patientsNum, int id) {
	if (patients == NULL) {
		return 0;  // or handle error as appropriate
	}

	for (int i = 0; i < patientsNum; i++) {
		if (patients[i]->id == id) {
			return 1;  // Patient found
		}
	}
	return 0;  // Patient not found
}
const Patient* findPatientById(const Patient** patients, int numPatients, int id) {
	if (patients == NULL || numPatients <= 0) {
		return NULL;  // Invalid input
	}
	for (int i = 0; i < numPatients; i++) {
		if (patients[i] != NULL && patients[i]->id == id) {
			return patients[i];  // Found the patient
		}
	}

	return NULL;  // Patient not found
}

void printPatients(const Patient** patients, int size) {
	printf("\n****************************\n");
	for (int i = 0; i < size; i++) {
		const Patient* patient = patients[i];
		if (patient != NULL) {
			printf("Patient No. %d)", i + 1);
			printf("\n");
			printPatient(patient);
		}
	}
}

int addPatientToArray(Patient*** patients, Patient* newP, int size) {
	if (!patients || !newP) {
		return -1;  // Error: Null pointer passed
	}

	for (int i = 0; i < size; i++) {
		if (newP->id == (*patients)[i]->id) {
			printf("\nThere are already patient with this id/licence id in the system!\nadding new patient canceled!\n");
			return 0;
		}
	}



	// Reallocate the array to fit one more Patient*
	Patient** temp = (Patient**)realloc(*patients, (size + 1) * sizeof(Patient*));
	if (temp == NULL) {
		return -1;  // Error: Memory allocation failed
	}

	*patients = temp;  // Update the original pointer to point to the new array
	(*patients)[size] = newP;
	return 1;
}







void freePatient(Patient** patient) {
	Patient* p = *patient;
	if (p == NULL) {
		return;
	}

	if (p->name != NULL) {
		free(p->name);
		p->name = NULL;
	}

}

int searchPatientById(const Patient** patients, int id, int size) {

	if (!patients)
		return -1;

	for (int i = 0; i < size; i++) {
		if (patients[i]->id == id)
			return i;
	}

	return -1;

}
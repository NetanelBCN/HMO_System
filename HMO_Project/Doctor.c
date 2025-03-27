#define _CRT_SECURE_NO_WARNINGS
#include "Doctor.h"
#include "General.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* MedicalSpecializationStrings[] = {
	"General",
	"Eyes",
	"Cardiology",
	"Neurology",
	"Oncology",
	"Surgery",
	"Psychiatry",
	"Dermatology",
	"Orthopedics",
	"Gynecology",
	"Urology",
	"Gastroenterology",
	"Endocrinology"
};

int initDoctor(Doctor** doctor) {
	if (!doctor) {
		return -1;
	}
	Doctor* p = *doctor;
	char buffer[MAX_STR_LEN];
	p->id = getID();


	printf("\nEnter the doctor's full name:\n");
	gets_s(buffer, MAX_STR_LEN);
	p->name = (char*)calloc((int)strlen(buffer) + 1, sizeof(char));
	if (!p->name) {
		printf("\nallocation error\n");
		return -1;
	}
	strcpy(p->name, buffer);


	do {
		printf("\nPlease enter the doctor's licence id (%d alphanumeric characters):\n", L_ID_MAX - 1);
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			free(p->name);
			return -1;
		}
		buffer[strlen(buffer) - 1] = '\0';
	} while (isValidlicenceID(buffer) != 1);
	strncpy(p->licence_id, buffer, L_ID_MAX - 1);
	p->licence_id[L_ID_MAX - 1] = '\0';

	p->gender = getGender();
	p->eS = (enum MedicalSpecialization)getSpecialization();

	return 1;
}


void printAllSpecializations() {
	for (int i = 0; i < ESIZE; i++) {
		printf("%d. %s\n", i + 1, MedicalSpecializationStrings[i]);
	}
	printf("\n");
}

void printDoctorsBasicInfo(Doctor* doctors, int size) {

	printf("\n****************************\n");

	generalArrayFunction(doctors, size, sizeof(Doctor), printDoctorBasicInfo);
}

int isDoctorExist(const Doctor* doctors, int doctorsNum, const char* licenceId) {
	if (doctors == NULL || licenceId == NULL) {
		return 0;  // or handle error as appropriate
	}

	for (int i = 0; i < doctorsNum; i++) {
		if (strcmp(doctors[i].licence_id, licenceId) == 0) {
			return 1;  // Doctor found
		}
	}
	return 0;  // Doctor not found
}

void printDoctor(const Doctor* doctor) {
	printf("\n--- Doctor Information ---\n");
	printf("Name: %s\n", doctor->name ? doctor->name : "N/A");
	printf("Specialization: %s\n", MedicalSpecializationStrings[doctor->eS]);
	printf("Gender: %c\n", doctor->gender);
	printf("licence ID: %s\n", doctor->licence_id);
	printf("Id: %d\n", doctor->id);
	printf("-------------------------\n");
}

void printDoctorBasicInfo(const Doctor* doctor) {
	printf("Name: %s | ", doctor->name);
	printf("Specialization: %s\n", MedicalSpecializationStrings[doctor->eS]);
	printf("Id: %d\n", doctor->id);
	printf("licence Id: %s | ", doctor->licence_id);
	printf("Gender: %c\n", doctor->gender);
	printf("-------------------------\n");
}



int isDoctorsEqual(const Doctor* d1, const Doctor* d2) {
	if (d1 == NULL || d2 == NULL) {
		return 1;
	}
	if (d1->id == d2->id || strcmp(d1->licence_id, d2->licence_id) == 0)
		return 0;
	return 1;
}
int getSpecialization() {
	printf("\nSelect the doctor's specialization:\n");
	for (int i = 0; i < sizeof(MedicalSpecializationStrings) / sizeof(MedicalSpecializationStrings[0]); i++) {
		printf("%d. %s\n", i + 1, MedicalSpecializationStrings[i]);
	}

	int choice;
	do {
		printf("Enter the number corresponding to the specialization: ");
		if (scanf("%d", &choice) != 1 || choice < 1 || choice > sizeof(MedicalSpecializationStrings) / sizeof(MedicalSpecializationStrings[0])) {
			printf("Invalid input. Please try again.\n");
			while (getchar() != '\n');
		}
		else {
			while (getchar() != '\n');
			return choice - 1;
		}
	} while (1);
}

int getDoctorIdBylicenceId(char* licenceId, const Doctor* doctors, int size) {
	if (licenceId == NULL || doctors == NULL || size <= 0) {
		return -1;
	}

	for (int i = 0; i < size; i++) {
		if (strcmp(doctors[i].licence_id, licenceId) == 0) {
			return doctors[i].id;
		}
	}

	return -1;
}

int printDoctorsBySpecialization(const Doctor* doctors, int size, eSpecialization spec) {

	printf("\nDoctors with %s specialization:\n", MedicalSpecializationStrings[spec]);
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (doctors[i].eS == spec) {
			printDoctorBasicInfo(&doctors[i]);
			count++;
		}
	}
	if (count == 0) {
		printf("No doctors found with this specialization.\n");
	}
	printf("\n");
	return count;
}

const Doctor* findDoctorById(const Doctor* doctors, int numDoctors, int id) {
	if (doctors == NULL || numDoctors <= 0) {
		return NULL;  // Invalid input
	}

	for (int i = 0; i < numDoctors; i++) {
		if (doctors[i].id == id) {
			return &doctors[i];  // Found the doctor
		}
	}

	return NULL;  // Doctor not found
}
/*
int addDoctorToArray(Doctor** doctors, Doctor* newD, int size) {

	if (!doctors) {
		return -1;
	}

	for (int i = 0;i < size;i++) {
		if (newD->id == doctors[i]->id || strcmp(newD->licence_id, doctors[i]->licence_id) == 0) {
			printf("\nThere are already doctor with this id/licence id in the system!\nadding new doctor canceled!\n");
			return -1;
		}
	}


	Doctor* temp = (Doctor*)realloc(*doctors, (size + 1) * sizeof(Doctor));
	if (!temp) {
		return -1;  // realloc failed, original memory is untouched
	}

	*doctors = temp;  // Update the pointer to the reallocated memory

	(*doctors)[size].name = _strdup(newD->name);
	if (!(*doctors)[size].name) {
		return -1;
	}

	strcpy((*doctors)[size].licence_id, newD->licence_id);
	(*doctors)[size].eS = newD->eS;
	(*doctors)[size].gender = newD->gender;
	(*doctors)[size].id = newD->id;
	printf("\nDoctor Added Successfully!\n");
	return 1;
}
*/
int addDoctorToArray(Doctor** doctors, Doctor* newD, int size) {
	if (!doctors || !newD) {
		return -1;
	}

	// Check if a doctor with the same ID or license ID already exists
	for (int i = 0; i < size; i++) {
		if (newD->id == (*doctors)[i].id || strcmp(newD->licence_id, (*doctors)[i].licence_id) == 0) {
			printf("\nThere is already a doctor with this ID or license ID in the system!\nAdding new doctor canceled!\n");
			return -1;
		}
	}

	// Reallocate memory for the array of doctors
	Doctor* temp = (Doctor*)realloc(*doctors, (size + 1) * sizeof(Doctor));
	if (!temp) {
		printf("\nError: Memory allocation failed!\n");
		return -1;  // Realloc failed, original memory is untouched
	}

	*doctors = temp;  // Update the pointer to the reallocated memory

	// Initialize the new doctor
	(*doctors)[size].name = _strdup(newD->name);
	if (!(*doctors)[size].name) {
		printf("\nError: Memory allocation for doctor's name failed!\n");
		return -1;
	}

	strncpy((*doctors)[size].licence_id, newD->licence_id, L_ID_MAX - 1);
	(*doctors)[size].licence_id[L_ID_MAX - 1] = '\0';  // Ensure null-termination
	(*doctors)[size].eS = newD->eS;
	(*doctors)[size].gender = newD->gender;
	(*doctors)[size].id = newD->id;

	printf("\nDoctor Added Successfully!\n");
	return 1;
}

void freeDoctor(Doctor* doc) {
	if (doc == NULL) {
		return;
	}
	if (doc->name != NULL) {
		free(doc->name);
		doc->name = NULL;
	}
}
int searchDoctorById(const Doctor* doctors, int id, int size) {

	if (!doctors)
		return -1;
	for (int i = 0; i < size; i++) {
		if (doctors[i].id == id)
			return i; //match found!
	}
	return -1;
}

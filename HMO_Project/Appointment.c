#define _CRT_SECURE_NO_WARNINGS
#include "Appointment.h"
#include <stdio.h>
#include "General.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>



int initAppointment(Appointment* appointment, const Doctor* doctors, const Patient** patients, int numPatients, int numDoctors) {
	int patientId;
	eSpecialization chosenSpec;
	char licenseId[L_ID_MAX];
	char buffer[MAX_STR_LEN];
	int doctorId;
	int count;
	// Verify patient exists
	printf("Enter patient ID: ");
	if (scanf("%d", &patientId) != 1) {
		printf("Invalid input for patient ID.\n");
		while (getchar() != '\n'); // Clear input buffer
		return -1;
	}
	while (getchar() != '\n'); // Clear input buffer

	if (!isPatientExist(patients, numPatients, patientId)) {
		printf("Patient with ID %d not found in the system.\n", patientId);
		return -1;
	}

	// Choose specialization
	printAllSpecializations();
	printf("Choose a specialization (1-%d): ", ESIZE);
	int specChoice;
	if (scanf("%d", &specChoice) != 1 || specChoice < 1 || specChoice > ESIZE) {
		printf("Invalid specialization choice.\n");
		while (getchar() != '\n'); // Clear input buffer
		return -1;
	}
	while (getchar() != '\n'); // Clear input buffer

	chosenSpec = (eSpecialization)(specChoice - 1);

	// Display doctors with chosen specialization
	count = printDoctorsBySpecialization(doctors, numDoctors, chosenSpec);
	if (count == 0) {
		return -1;
	}

	// Choose doctor
	printf("Enter the desired doctor's license ID: ");
	if (fgets(buffer, MAX_STR_LEN, stdin) == NULL) {
		printf("Error reading input.\n");
		return -1;
	}

	// Remove newline character if present
	buffer[strcspn(buffer, "\n")] = 0;

	if (strlen(buffer) >= L_ID_MAX) {
		printf("License ID is too long. Maximum length is %d characters.\n", L_ID_MAX - 1);
		return -1;
	}

	strcpy(licenseId, buffer);

	doctorId = getDoctorIdBylicenceId(licenseId, doctors, numDoctors);
	if (doctorId == -1) {
		printf("Doctor with license ID %s and chosen specialization not found.\n", licenseId);
		return -1;
	}

	// Set appointment details
	appointment->doctor_id = doctorId;
	appointment->patient_id = patientId;

	initDate(&appointment->date);
	initTime(&appointment->time);


	return 1;
}
void printAppointment(const Appointment* appointment, const Doctor* doctors, const Patient** patients, int numPatients, int numDoctors) {
	if (appointment == NULL) {
		printf("Error: Invalid appointment pointer\n");
		return;
	}

	// Find the patient
	const Patient* patient = findPatientById(patients, numPatients, appointment->patient_id);
	if (patient == NULL) {
		printf("Error: Patient not found\n");
		return;
	}

	// Find the doctor
	const Doctor* doctor = findDoctorById(doctors, numDoctors, appointment->doctor_id);
	if (doctor == NULL) {
		printf("Error: Doctor not found\n");
		return;
	}

	printf("\n--- Appointment Details ---\n");
	printf("* Date: ");
	printDate(&appointment->date);
	printf(" | ");
	printTime(appointment->time);
	printf(" *\n");
	printf("Doctor Name: %s\n", doctor->name);
	printf("Patient Name: %s\n", patient->name);
	printf("Topic: %s\n", MedicalSpecializationStrings[doctor->eS]);
	printf("---------------------------\n");
}

int areAppointmentsConflicting(const Appointment* a1, const Appointment* a2) {
	// First, check if the appointments are on the same date
	if (!areDatesEqual(&a1->date, &a2->date)) {
		return 0; // Not on the same date, no conflict
	}
	if (a1->patient_id != a2->patient_id && a1->doctor_id != a2->doctor_id)
		return 0;
	// Calculate the time difference in minutes
	int time1 = a1->time.hour * 60 + a1->time.minute;
	int time2 = a2->time.hour * 60 + a2->time.minute;
	int timeDiff = abs(time1 - time2);

	// Check if the time difference is less than or equal to 15 minutes
	return (timeDiff <= CONFLICT_WINDOW);
}


void addAppointment(NODE** list, Appointment* newAppointment) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (!newNode) {
		printf("Memory allocation failed!\n");
		return;
	}

	newNode->data = newAppointment;
	newNode->next = *list;
	*list = newNode;
}

// Function to remove an appointment from the list by index
void removeAppointment(NODE** list, int index) {
	if (*list == NULL || index < 0) {
		printf("Invalid list or index.\n");
		return;
	}

	NODE* current = *list;
	NODE* prev = NULL;
	int currentIndex = 0;

	while (current != NULL && currentIndex < index) {
		prev = current;
		current = current->next;
		currentIndex++;
	}

	if (current == NULL) {
		printf("Appointment at index %d not found.\n", index);
		return;
	}

	if (prev == NULL) {
		*list = current->next;
	}
	else {
		prev->next = current->next;
	}

	free(current->data);  // Free the appointment structure
	free(current);        // Free the node
	printf("Appointment at index %d removed.\n", index);
}

// Function to print all appointments with ascending numbers
void printAppointments(NODE* list) {
	NODE* current = list;
	int index = 1;  // Start numbering from 1 for user-friendliness
	printf("Appointments:\n");
	while (current != NULL) {
		Appointment* app = (Appointment*)current->data;
		printf("%d. Doctor: %d, Patient: %d, Date: %d/%d/%d, Time: %d:%d\n",
			index, app->doctor_id, app->patient_id,
			app->date.day, app->date.month, app->date.year,
			app->time.hour, app->time.minute);
		current = current->next;
		index++;
	}
	if (index == 1) {
		printf("No appointments found.\n");
	}
}

// Function to free the appointment list
void freeAppointmentList(NODE** list) {
	NODE* current = *list;
	NODE* next;
	while (current != NULL) {
		next = current->next;
		free(current->data);  // Free the appointment structure
		free(current);        // Free the node
		current = next;
	}
	*list = NULL;
}
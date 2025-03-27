#define  _CRT_SECURE_NO_WARNINGS
#include "HMO.h"
#include <stdio.h>
#include <stdlib.h>
#include "General.h"
#include <ctype.h>
#include <string.h>
#include <time.h>



int initHMO(HMO** hmo) {
	if (!hmo) {
		printf("\nError occurred: Invalid HMO pointer!\n");
		return -1;
	}
	*hmo = (HMO*)malloc(sizeof(HMO));
	printf("\n*********Initializing HMO*********\n");
	if (!*hmo) {
		printf("\nError occurred: Memory allocation for HMO failed!\n");
		return -1;
	}
	(*hmo)->prescriptionListHead = NULL;
	(*hmo)->prescriptionListTail = NULL;
	(*hmo)->address = askForAddress();
	(*hmo)->appointmentsNum = 0;
	if (initHMOAdmin(&(*hmo)->admin) != 1) {
		free(*hmo);
		return -1;
	}
	;
	if (!(*hmo)->address) {
		printf("\nError occurred: Failed to allocate memory for address!\n");
		free(*hmo);
		return -1;
	}

	(*hmo)->doctors = (Doctor*)malloc(sizeof(Doctor));  // Assuming you want to allocate space for one Doctor initially
	if (!(*hmo)->doctors) {
		printf("\nError occurred: Memory allocation for doctors failed!\n");
		free((*hmo)->address);
		free(*hmo);
		return -1;
	}

	(*hmo)->patients = (Patient**)malloc(sizeof(Patient*));  // Allocating space for an array of Patient pointers
	if (!(*hmo)->patients) {
		printf("\nError occurred: Memory allocation for patients failed!\n");
		free((*hmo)->doctors);
		free((*hmo)->address);
		free(*hmo);
		return -1;
	}

	(*hmo)->numDoctors = 0;
	(*hmo)->numPatients = 0;
	(*hmo)->appointmentList = NULL;

	(*hmo)->pharmacy = (Pharmacy*)malloc(sizeof(Pharmacy));
	if (!(*hmo)->pharmacy) {
		free((*hmo)->doctors);
		free((*hmo)->address);
		free((*hmo)->patients);
		free(*hmo);
		return -1;
	}

	Pharmacy* p = (*hmo)->pharmacy;
	if (initPharmacy(&p) == -1) {
		free((*hmo)->doctors);
		free((*hmo)->address);
		free((*hmo)->patients);
		free(*hmo);
		free(p);
		return -1;
	};
	return 1;
}

void freeHMO(HMO** hmo) {
	HMO* p = *hmo;
	generalArrayFunction(p->doctors, p->numDoctors, sizeof(Doctor), freeDoctor);
	free(p->doctors);
	p->numDoctors = 0;
	generalArrayFunction(p->patients, p->numPatients, sizeof(Patient*), freePatient);
	free(p->patients);
	p->numPatients = 0;
	freeAppointmentList(&p->appointmentList);
	freePrescriptionList(p);
	free(p->address);
	freePharmacy(&p->pharmacy);
	free(p);
}

void printHMO(HMO* hmo) {
	printf("\n==========================================================\n");
	printf("||                HMO Information                       ||\n");
	printf("==========================================================\n");
	if (hmo->address) {
		char street[128] = "", city[128] = "";
		if (sscanf(hmo->address, "%[^,], %s", street, city) == 0)
			return;
		street[0] = toupper(street[0]);
		city[0] = toupper(city[0]);
		printf("|| %s HMO in %s ||\n", street, city);
	}
	printHMOAdmin(&hmo->admin);
	printf("==========================================================\n");
	printf("|| %s %d                        ||\n", "Number of Doctors:", hmo->numDoctors);
	printf("|| %s %d                        ||\n", "Number of Patients:", hmo->numPatients);
	printf("==========================================================\n");
}

int addNewDoctorToHMO(HMO* hmo, Doctor* doctor) {

	if (!hmo) {
		printf("\nError occured, can't add doctor!\n");
		return -1;
	}


	if (addDoctorToArray(&hmo->doctors, doctor, hmo->numDoctors) == -1) {
		printf("\nError occured, can't add doctor!\n");
		return -1;
	}

	hmo->numDoctors++;
	return 1;
}



int addNewPatientToHMO(HMO* hmo, Patient* patient) {

	if (!hmo) {
		printf("\nError occured, can't add doctor!\n");
		return -1;
	}


	if (addPatientToArray(&hmo->patients, patient, hmo->numPatients) == -1) {
		printf("\nError occured, can't add doctor!\n");
		return -1;
	}

	hmo->numPatients++;
	return 1;
}

void printHMODoctors(HMO* hmo) {
	if (hmo->numDoctors == 0) {
		printf("\nThe Doctors list is empty!\n");
		return;
	}
	printf("\nThe Doctors in the HMO:");
	printDoctorsBasicInfo(hmo->doctors, hmo->numDoctors);

}

void printHMOPatients(const HMO* hmo) {
	if (hmo->numPatients == 0) {
		printf("\nThe Patients list is empty!\n");
		return;
	}
	printf("\nThe Patients in the HMO:");
	printPatients(hmo->patients, hmo->numPatients);
}

int createAndAddAppointmentToHMO(HMO* hmo, Appointment* newAppointment) {
	int flag;

	if (!hmo || !hmo->doctors || !hmo->patients) {
		printf("\nError: Invalid pointer!\n");
		return -1;
	}

	if (!newAppointment) {
		printf("\nError: New appointment failed!\n");
		return -1;
	}
	if (hmo->numDoctors == 0 || hmo->numPatients == 0) {
		printf("\nCan't schedual an appointment //>> Please make sure that there are doctors and patients in the system!\n");
		return -1;
	}


	flag = initAppointment(newAppointment, hmo->doctors, hmo->patients, hmo->numPatients, hmo->numDoctors);
	if (flag == -1) {
		return -1;
	}
	// Initialize the appointment
	while (flag == 1) {
		int conflictFound = 0;
		NODE* current = hmo->appointmentList;

		// Check for conflicts with existing appointments
		while (current != NULL) {
			Appointment* existingAppointment = (Appointment*)current->data;

			if (areAppointmentsConflicting(newAppointment, existingAppointment)) {
				conflictFound = 1;
				printf("\nError: Can't schedual the appointment because you alreday have an appointment in the desired time range or the desired doctor is busy at that time //>> Please try again later with different time/date .\n");
				printf("\n\n");
				break;
			}

			current = current->next;
		}

		// If no conflict is found, break out of the loop and add the appointment
		if (!conflictFound) {
			break;
		}
	}

	// Add the new, non-conflicting appointment to the HMO's appointment list
	addAppointment(&(hmo->appointmentList), newAppointment);
	hmo->appointmentsNum++;
	printf("\nAppointment schedualed successfuly!\n");
	return 1;
}

void printAppointmentsInHMO(const HMO* hmo) {
	if (!hmo) {
		printf("\nError: Invalid HMO pointer!\n");
		return;
	}

	NODE* current = hmo->appointmentList;
	int index = 0;

	while (current != NULL) {
		Appointment* appointment = (Appointment*)current->data;
		printf("\n--- Appointment %d ---\n", index + 1);
		printAppointment(appointment, hmo->doctors, hmo->patients, hmo->numPatients, hmo->numDoctors);
		current = current->next;
		index++;
	}
}
int searchDoctorByIdInHMO(const HMO* hmo, int id) {
	return searchDoctorById(hmo->doctors, id, hmo->numDoctors);

}

int searchPatientByIdInHMO(const HMO* hmo, int id) {
	return searchPatientById(hmo->patients, id, hmo->numPatients);
}



int initPrescription(const HMO* hmo, Prescription* pr) {
	if (!hmo || !pr)
		return -1;

	int index, input;
	char buffer[100];

	printf("\n\n******Dear doctor please identify by your Id:\n");
	int id = getID();
	index = searchDoctorByIdInHMO(hmo, id);
	if (index == -1) {
		printf("\nThis doctor doesn't exist in the system!\n");
		return -1;
	}
	if (hmo->numPatients < 1) {
		printf("\nHMO patient list is empty!\n");
		return -1;
	}
	if (hmo->pharmacy->numOfDrugs < 1) {
		printf("\nPharmacy drug stock is empty!\n");
		return -1;
	}
	pr->doctor_id = id;
	printf("\n");
	printf("\n-------Welcome Dr. %s-------\n\n", hmo->doctors[index].name);
	printHMOPatients(hmo);
	printf("\nplease choose a patient from the list above : //> choose by row number [1-%d]\n", hmo->numPatients);
	while (1) {
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			printf("Error reading input.\n");
			return -1;
		}
		if (sscanf(buffer, "%d", &input) == 1 && input >= 1 && input <= hmo->numPatients) {
			index = input - 1;
			break;
		}
		printf("Invalid input. Please enter a number between 1 and %d.\n", hmo->numPatients);
	}
	pr->patient_id = hmo->patients[index]->id;

	printf("\n");
	printPharmacyDrugs(hmo->pharmacy);
	printf("\nPlease enter the required drug No. from the list above:\n");

	while (1) {
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			printf("Error reading input.\n");
			return -1;
		}
		if (sscanf(buffer, "%d", &input) == 1 && input >= 1 && input <= hmo->pharmacy->numOfDrugs) {
			index = input - 1;
			break;
		}
		printf("Invalid input. Please enter a number between 1 and %d.\n", hmo->pharmacy->numOfDrugs);
	}
	strcpy(pr->drug_serial_id, hmo->pharmacy->drugs[index]->serial_id);

	printf("Please type the prescription Exp. date:\n");
	if (initDate(&pr->prescription_exp) == -1) {
		printf("Error initializing date.\n");
		return -1;
	}

	printf("\nWhat is the desired amount?\n");
	while (1) {
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			printf("Error reading input.\n");
			return -1;
		}
		if (sscanf(buffer, "%d", &pr->dose) == 1 && pr->dose > 0) {
			break;
		}
		printf("Invalid input. Please enter a positive integer.\n");
	}

	pr->pharmacist_id = -1; // buying the drugs will lead into the prescription list which we will choose to buy.
	return 1;
}


void printPrescription(const Prescription* pr, HMO* hmo) {
	if (!pr || !hmo) {
		printf("\nNothing to print!\n");
		return;
	}
	printf("\n------Prescription Details------\n");
	printf("\nExp. date: ");
	printDate(&pr->prescription_exp);

	int indexDrug = findDrugByIdInPharmacy(hmo->pharmacy, pr->drug_serial_id);
	if (indexDrug == -1) {
		printf("\nerror occured\n");
		return;
	}
	Drug* d = hmo->pharmacy->drugs[indexDrug];
	printf("\nDrug name:%s\nTotal dose:%d\n", d->name, pr->dose);

	int indexD = searchDoctorByIdInHMO(hmo, pr->doctor_id);
	if (indexD == -1) {
		printf("\nerror occured\n");
		return;
	}
	printf("Prescripted by Dr. %s\n", hmo->doctors[indexD].name);
	int indexP = searchPatientByIdInHMO(hmo, pr->patient_id);
	if (indexP == -1) {
		printf("\nerror occured\n");
		return;
	}
	printf("Prescripted for %s\n", hmo->patients[indexP]->name);
	if (isPrescriptionUsed(pr) == 1) {
		indexP = findPharmacistByIdInPharmacy(hmo->pharmacy, pr->pharmacist_id);
		if (indexP == -1) {
			printf("\nerror occured\n");
			return;
		}
		printf("\nThe prescription has been used, sold by the pharmacist: %s\n", hmo->pharmacy->pharmacists[indexP]->name);
	}
	else
		printf("\nThe prescription hasn't used yet\n");
	printf("--------------------------------------------------\n");
}



int addPrescriptionToList(HMO* hmo, Prescription* pr) {
	if (!hmo || !pr) {
		return -1; // Invalid input
	}

	PrescriptionNode* newNode = (PrescriptionNode*)malloc(sizeof(PrescriptionNode));
	if (newNode == NULL) {
		return -1; // Memory allocation failed
	}

	// Copy the prescription data
	newNode->data = *pr;
	newNode->prev = NULL;
	newNode->next = NULL;

	if (hmo->prescriptionListHead == NULL) {
		// List is empty
		hmo->prescriptionListHead = newNode;
		hmo->prescriptionListTail = newNode;
	}
	else {
		// Add to the end of the list
		newNode->prev = hmo->prescriptionListTail;
		hmo->prescriptionListTail->next = newNode;
		hmo->prescriptionListTail = newNode;
	}

	return 1; // Successfully added
}

// Function to print the prescription list
void printPrescriptionList(const HMO* hmo) {
	PrescriptionNode* current = hmo->prescriptionListHead;
	int count = 0;

	while (current != NULL) {
		printf("Prescription #%d:", ++count);
		printPrescription(&current->data, (HMO*)hmo);
		printf("\n");
		current = current->next;
	}

	if (count == 0) {
		printf("\nThe prescription list is empty.\n");
	}

}

void freePrescriptionList(HMO* hmo) {
	if (hmo == NULL) {
		return;  // Invalid input
	}

	PrescriptionNode* current = hmo->prescriptionListHead;
	PrescriptionNode* next;

	while (current != NULL) {
		next = current->next;

		// No need to free individual members of Prescription
		// as they are all statically allocated

		// Free the node itself
		free(current);

		current = next;
	}

	// Reset the head and tail pointers
	hmo->prescriptionListHead = NULL;
	hmo->prescriptionListTail = NULL;
}

void printPatientAppointments(const HMO* hmo, int id) {
	if (!hmo) {
		printf("\nError: Invalid HMO pointer!\n");
		return;
	}

	NODE* current = hmo->appointmentList;
	int index = 0;
	int count = 0;
	while (current != NULL) {
		Appointment* appointment = (Appointment*)current->data;
		if (appointment->patient_id == id) {
			printf("\n--- Appointment %d ---\n", index + 1);
			printAppointment(appointment, hmo->doctors, hmo->patients, hmo->numPatients, hmo->numDoctors);
			index++;
			count++;
		}
		current = current->next;
	}
	if (count == 0) {
		printf("\nCan't find appointments related to this patient! //> make sure that they are exist in the system and have appointments!\n");
	}

}
void buyDrugByPrescription(HMO* hmo) {
	int patientID, choice;
	PrescriptionNode* current;
	int prescriptionCount = 0;
	int randomPharmacistID;

	// Get patient ID
	printf("\nEnter patient ID: ");
	patientID = getID();

	// Show all unused prescriptions for this patient
	printf("\nUnused prescriptions for patient %d:\n", patientID);
	current = hmo->prescriptionListHead;
	while (current != NULL) {
		if (current->data.patient_id == patientID && !isPrescriptionUsed(&current->data)) {
			printf("%d. Drug ID: %s, Dose: %d\n", ++prescriptionCount,
				current->data.drug_serial_id, current->data.dose);
		}
		current = current->next;
	}

	if (prescriptionCount == 0) {
		printf("\nNo unused prescriptions found for this patient.\n");
		return;
	}

	// Let the patient choose a prescription
	printf("\nChoose a prescription [1-%d]: ", prescriptionCount);
	if (scanf("%d", &choice) != 1) {
		printf("Error reading input. Exiting program.\n");
		return;
	}


	if (choice < 1 || choice > prescriptionCount) {
		printf("Invalid choice.\n");
		return;
	}

	// Find the chosen prescription
	current = hmo->prescriptionListHead;
	prescriptionCount = 0;
	while (current != NULL) {
		if (current->data.patient_id == patientID && !isPrescriptionUsed(&current->data)) {
			if (++prescriptionCount == choice) {
				break;
			}
		}
		current = current->next;
	}

	if (current == NULL) {
		printf("Error: Prescription not found.\n");
		return;
	}

	// Randomly select a pharmacist
	if (hmo->pharmacy->numOfPharmacists > 0) {
		randomPharmacistID = hmo->pharmacy->pharmacists[rand() % hmo->pharmacy->numOfPharmacists]->id;
	}
	else {
		printf("Error: No pharmacists available.\n");
		return;
	}

	// Update the prescription with the pharmacist ID
	current->data.pharmacist_id = randomPharmacistID;
	printf("\nDrug purchased successfully!\n\n");
	printf("----------Prescription details----------\n");
	printf("Drug ID: %s\n", current->data.drug_serial_id);
	printf("Dose amount: %d\n", current->data.dose);
	int index = findPharmacistByIdInPharmacy(hmo->pharmacy, current->data.pharmacist_id);
	printf("Seller pharmacist Name: %s\n", hmo->pharmacy->pharmacists[index]->name);
	printf("\n-------Thank you--------\n");
	getchar();

}

void createAppointmentApprovalFile(HMO* hmo) {
	int patientID, choice;
	NODE* current;
	int appointmentCount = 0;
	char recipientName[100];
	FILE* file;
	int indexD;

	// Get patient ID
	printf("\nEnter patient ID: ");
	patientID = getID();

	// Show all appointments for this patient
	int index = searchPatientByIdInHMO(hmo, patientID);
	if (index == -1) {
		printf("\nCan't find patient!\n");
		return;
	}
	printf("\nAppointments for patient %s:\n\n", hmo->patients[index]->name);
	current = hmo->appointmentList;
	while (current != NULL) {
		Appointment* app = (Appointment*)current->data;
		indexD = searchDoctorByIdInHMO(hmo, app->doctor_id);
		if (indexD == -1) {
			printf("\nError occured!\n");
			return;
		}
		if (app->patient_id == patientID) {
			printf("%d. Date: %02d/%02d/%04d, Time: %02d:%02d, Doctor name: %s\n",
				++appointmentCount,
				app->date.day, app->date.month, app->date.year,
				app->time.hour, app->time.minute,
				hmo->doctors[indexD].name);
		}
		current = current->next;
	}

	if (appointmentCount == 0) {
		printf("No appointments found for this patient.\n");
		return;
	}

	// Let the patient choose an appointment
	printf("\nChoose an appointment (1-%d): ", appointmentCount);
	if (scanf("%d", &choice) != 1) {
		return;
	};
	while (getchar() != '\n'); // Clear input buffer

	if (choice < 1 || choice > appointmentCount) {
		printf("\nInvalid choice.\n");
		return;
	}

	// Find the chosen appointment
	current = hmo->appointmentList;
	appointmentCount = 0;
	Appointment* chosenApp = NULL;
	while (current != NULL) {
		Appointment* app = (Appointment*)current->data;
		if (app->patient_id == patientID) {
			if (++appointmentCount == choice) {
				chosenApp = app;
				break;
			}
		}
		current = current->next;
	}

	if (chosenApp == NULL) {
		printf("\nError: Appointment not found.\n");
		return;
	}

	// Get recipient name
	printf("\nEnter the name of the person/organization receiving the approval: ");
	fgets(recipientName, sizeof(recipientName), stdin);
	recipientName[strcspn(recipientName, "\n")] = 0; // Remove newline

	// Create and write to the approval file
	file = fopen("appointment_approval.txt", "w");
	if (file == NULL) {
		printf("\nError opening file for writing.\n");
		return;
	}

	fprintf(file, "Appointment approval/proof for: %s\n", recipientName);
	fprintf(file, "--------------------------------\n");
	fprintf(file, "HMO Admin name: %s\n", hmo->admin.name);
	fprintf(file, "Doctor name: %s\n", hmo->doctors[indexD].name);
	fprintf(file, "Patient name: %s\n", hmo->patients[index]->name);
	fprintf(file, "Appointment Date: %02d/%02d/%04d\n",
		chosenApp->date.day, chosenApp->date.month, chosenApp->date.year);
	fprintf(file, "Appointment Time: %02d:%02d\n",
		chosenApp->time.hour, chosenApp->time.minute);
	fclose(file);
	printf("Appointment approval file created successfully.\n");
}


////////////////////////////////////////////////////////////////////////


int writeHMOToBinaryFile(const HMO* hmo, const char* filename) {
	FILE* file = fopen(filename, "wb");
	if (!file) {
		printf("Error opening file for writing.\n");
		return -1;
	}

	// Write basic HMO data
	fwrite(&hmo->numDoctors, sizeof(int), 1, file);
	fwrite(&hmo->numPatients, sizeof(int), 1, file);
	fwrite(&hmo->appointmentsNum, sizeof(int), 1, file);

	// Write address
	int addressLength = strlen(hmo->address) + 1;
	fwrite(&addressLength, sizeof(int), 1, file);
	fwrite(hmo->address, sizeof(char), addressLength, file);

	// Write doctors
	for (int i = 0; i < hmo->numDoctors; i++) {
		writeDoctorToBinaryFile(&hmo->doctors[i], file);
	}

	// Write patients
	for (int i = 0; i < hmo->numPatients; i++) {
		writePatientToBinaryFile(hmo->patients[i], file);
	}

	// Write appointments
	writeAppointmentsToBinaryFile(hmo->appointmentList, file);

	// Write pharmacy
	writePharmacyToBinaryFile(hmo->pharmacy, file);

	// Write prescriptions
	writePrescriptionsToBinaryFile(hmo->prescriptionListHead, file);

	// Write HMO_Admin
	writeHMOAdminToBinaryFile(&hmo->admin, file);

	fclose(file);
	return 1;
}

void writeDoctorToBinaryFile(const Doctor* doctor, FILE* file) {
	int nameLength = strlen(doctor->name) + 1;
	fwrite(&nameLength, sizeof(int), 1, file);
	fwrite(doctor->name, sizeof(char), nameLength, file);
	fwrite(&doctor->id, sizeof(int), 1, file);
	fwrite(&doctor->gender, sizeof(char), 1, file);
	fwrite(doctor->licence_id, sizeof(char), L_ID_MAX, file);
	fwrite(&doctor->eS, sizeof(eSpecialization), 1, file);
}

void writePatientToBinaryFile(const Patient* patient, FILE* file) {
	int nameLength = strlen(patient->name) + 1;
	fwrite(&nameLength, sizeof(int), 1, file);
	fwrite(patient->name, sizeof(char), nameLength, file);
	fwrite(&patient->id, sizeof(int), 1, file);
	fwrite(&patient->birthDate, sizeof(Date), 1, file);
	fwrite(&patient->gender, sizeof(char), 1, file);
}

void writeAppointmentsToBinaryFile(NODE* appointmentList, FILE* file) {
	NODE* current = appointmentList;
	int count = 0;
	while (current) {
		count++;
		current = current->next;
	}
	fwrite(&count, sizeof(int), 1, file);

	current = appointmentList;
	while (current) {
		Appointment* app = (Appointment*)current->data;
		fwrite(app, sizeof(Appointment), 1, file);
		current = current->next;
	}
}

void writePharmacyToBinaryFile(const Pharmacy* pharmacy, FILE* file) {
	int addressLength = strlen(pharmacy->address) + 1;
	fwrite(&addressLength, sizeof(int), 1, file);
	fwrite(pharmacy->address, sizeof(char), addressLength, file);
	fwrite(&pharmacy->responsible_pharmacist_id, sizeof(int), 1, file);
	fwrite(&pharmacy->numOfDrugs, sizeof(int), 1, file);
	fwrite(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
	fwrite(&pharmacy->eSort, sizeof(DrugSortType), 1, file);

	for (int i = 0; i < pharmacy->numOfDrugs; i++) {
		writeDrugToBinaryFile(pharmacy->drugs[i], file);
	}

	for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
		writePharmacistToBinaryFile(pharmacy->pharmacists[i], file);
	}
}

void writeDrugToBinaryFile(const Drug* drug, FILE* file) {
	int nameLength = strlen(drug->name) + 1;
	fwrite(&nameLength, sizeof(int), 1, file);
	fwrite(drug->name, sizeof(char), nameLength, file);
	fwrite(&drug->exp_date, sizeof(Date), 1, file);
	fwrite(drug->serial_id, sizeof(char), S_ID_MAX, file);
}

void writePharmacistToBinaryFile(const Pharmacist* pharmacist, FILE* file) {
	int nameLength = strlen(pharmacist->name) + 1;
	fwrite(&nameLength, sizeof(int), 1, file);
	fwrite(pharmacist->name, sizeof(char), nameLength, file);
	fwrite(&pharmacist->id, sizeof(int), 1, file);
	fwrite(pharmacist->licence_id, sizeof(char), L_ID_MAX, file);
}

void writePrescriptionsToBinaryFile(PrescriptionNode* head, FILE* file) {
	PrescriptionNode* current = head;
	int count = 0;
	while (current) {
		count++;
		current = current->next;
	}
	fwrite(&count, sizeof(int), 1, file);

	current = head;
	while (current) {
		fwrite(&current->data, sizeof(Prescription), 1, file);
		current = current->next;
	}
}
void writeHMOAdminToBinaryFile(const HMO_Admin* admin, FILE* file) {
	unsigned char buffer[1024];  // Adjust size as needed
	int bufferSize = 0;

	// Serialize HMO_Admin data
	int nameLength = strlen(admin->name) + 1;
	memcpy(buffer + bufferSize, &nameLength, sizeof(int));
	bufferSize += sizeof(int);
	memcpy(buffer + bufferSize, admin->name, nameLength);
	bufferSize += nameLength;
	memcpy(buffer + bufferSize, &admin->id, sizeof(int));
	bufferSize += sizeof(int);
	memcpy(buffer + bufferSize, &admin->seniority_years, sizeof(int));
	bufferSize += sizeof(int);
	memcpy(buffer + bufferSize, &admin->gender, sizeof(char));
	bufferSize += sizeof(char);

	// Allocate memory for compressed buffer
	int allocatedCompressedSize = 2 * bufferSize;  // Worst case scenario
	unsigned char* compressedBuffer = malloc(allocatedCompressedSize);
	if (!compressedBuffer) {
		printf("Memory allocation failed for compression.\n");
		return;
	}

	// Compress data using RLE
	int compressedSize = 0;
	for (int i = 0; i < bufferSize; i++) {
		unsigned char currentByte = buffer[i];
		unsigned char count = 1;

		// Count consecutive identical bytes
		while (i + 1 < bufferSize && count < 255) {
			if (buffer[i + 1] != currentByte) {
				break;
			}
			count++;
			i++;
		}

		// Ensure we have enough space in compressedBuffer
		if (compressedSize + 2 > allocatedCompressedSize) {
			printf("Error: Compressed data exceeds buffer size.\n");
			free(compressedBuffer);
			return;
		}

		// Write count and byte value
		compressedBuffer[compressedSize++] = count;
		compressedBuffer[compressedSize++] = currentByte;
	}

	// Write compressed data to file
	fwrite(&compressedSize, sizeof(int), 1, file);
	fwrite(compressedBuffer, sizeof(unsigned char), compressedSize, file);

	// Free allocated memory
	free(compressedBuffer);
}





///////////////////////////////////////////////////////////////

int readHMOFromBinaryFile(HMO** hmo, const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("Error opening file for reading.\n");
		return -1;
	}

	*hmo = (HMO*)malloc(sizeof(HMO));
	HMO* newHMO = *hmo;

	// Read basic HMO data
	fread(&newHMO->numDoctors, sizeof(int), 1, file);
	fread(&newHMO->numPatients, sizeof(int), 1, file);
	fread(&newHMO->appointmentsNum, sizeof(int), 1, file);

	// Read address
	int addressLength;
	fread(&addressLength, sizeof(int), 1, file);
	newHMO->address = (char*)malloc(addressLength);
	fread(newHMO->address, sizeof(char), addressLength, file);

	// Read doctors
	newHMO->doctors = (Doctor*)malloc(sizeof(Doctor) * newHMO->numDoctors);
	for (int i = 0; i < newHMO->numDoctors; i++) {
		readDoctorFromBinaryFile(&newHMO->doctors[i], file);
	}

	// Read patients
	newHMO->patients = (Patient**)malloc(sizeof(Patient*) * newHMO->numPatients);
	for (int i = 0; i < newHMO->numPatients; i++) {
		newHMO->patients[i] = (Patient*)malloc(sizeof(Patient));
		readPatientFromBinaryFile(newHMO->patients[i], file);
	}

	// Read appointments
	readAppointmentsFromBinaryFile(&newHMO->appointmentList, file);

	// Read pharmacy
	newHMO->pharmacy = (Pharmacy*)malloc(sizeof(Pharmacy));
	readPharmacyFromBinaryFile(newHMO->pharmacy, file);

	// Read prescriptions
	readPrescriptionsFromBinaryFile(&newHMO->prescriptionListHead, &newHMO->prescriptionListTail, file);

	// Read HMO_Admin
	readHMOAdminFromBinaryFile(&newHMO->admin, file);

	fclose(file);
	return 1;
}

void readDoctorFromBinaryFile(Doctor* doctor, FILE* file) {
	int nameLength;
	fread(&nameLength, sizeof(int), 1, file);
	doctor->name = (char*)malloc(nameLength);
	fread(doctor->name, sizeof(char), nameLength, file);
	fread(&doctor->id, sizeof(int), 1, file);
	fread(&doctor->gender, sizeof(char), 1, file);
	fread(doctor->licence_id, sizeof(char), L_ID_MAX, file);
	fread(&doctor->eS, sizeof(eSpecialization), 1, file);
}

void readPatientFromBinaryFile(Patient* patient, FILE* file) {
	int nameLength;
	fread(&nameLength, sizeof(int), 1, file);
	patient->name = (char*)malloc(nameLength);
	fread(patient->name, sizeof(char), nameLength, file);
	fread(&patient->id, sizeof(int), 1, file);
	fread(&patient->birthDate, sizeof(Date), 1, file);
	fread(&patient->gender, sizeof(char), 1, file);
}

void readAppointmentsFromBinaryFile(NODE** appointmentList, FILE* file) {
	int count;
	fread(&count, sizeof(int), 1, file);

	*appointmentList = NULL;
	NODE* last = NULL;

	for (int i = 0; i < count; i++) {
		Appointment* app = (Appointment*)malloc(sizeof(Appointment));
		fread(app, sizeof(Appointment), 1, file);

		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		newNode->data = app;
		newNode->next = NULL;

		if (last == NULL) {
			*appointmentList = newNode;
		}
		else {
			last->next = newNode;
		}
		last = newNode;
	}
}

void readPharmacyFromBinaryFile(Pharmacy* pharmacy, FILE* file) {
	int addressLength;
	fread(&addressLength, sizeof(int), 1, file);
	pharmacy->address = (char*)malloc(addressLength);
	fread(pharmacy->address, sizeof(char), addressLength, file);
	fread(&pharmacy->responsible_pharmacist_id, sizeof(int), 1, file);
	fread(&pharmacy->numOfDrugs, sizeof(int), 1, file);
	fread(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
	fread(&pharmacy->eSort, sizeof(DrugSortType), 1, file);

	pharmacy->drugs = (Drug**)malloc(sizeof(Drug*) * pharmacy->numOfDrugs);
	for (int i = 0; i < pharmacy->numOfDrugs; i++) {
		pharmacy->drugs[i] = (Drug*)malloc(sizeof(Drug));
		readDrugFromBinaryFile(pharmacy->drugs[i], file);
	}

	pharmacy->pharmacists = (Pharmacist**)malloc(sizeof(Pharmacist*) * pharmacy->numOfPharmacists);
	for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
		pharmacy->pharmacists[i] = (Pharmacist*)malloc(sizeof(Pharmacist));
		readPharmacistFromBinaryFile(pharmacy->pharmacists[i], file);
	}
}

void readDrugFromBinaryFile(Drug* drug, FILE* file) {
	int nameLength;
	fread(&nameLength, sizeof(int), 1, file);
	drug->name = (char*)malloc(nameLength);
	fread(drug->name, sizeof(char), nameLength, file);
	fread(&drug->exp_date, sizeof(Date), 1, file);
	fread(drug->serial_id, sizeof(char), S_ID_MAX, file);
}

void readPharmacistFromBinaryFile(Pharmacist* pharmacist, FILE* file) {
	int nameLength;
	fread(&nameLength, sizeof(int), 1, file);
	pharmacist->name = (char*)malloc(nameLength);
	fread(pharmacist->name, sizeof(char), nameLength, file);
	fread(&pharmacist->id, sizeof(int), 1, file);
	fread(pharmacist->licence_id, sizeof(char), L_ID_MAX, file);
}

void readPrescriptionsFromBinaryFile(PrescriptionNode** head, PrescriptionNode** tail, FILE* file) {
	int count;
	fread(&count, sizeof(int), 1, file);

	*head = NULL;
	*tail = NULL;

	for (int i = 0; i < count; i++) {
		PrescriptionNode* newNode = (PrescriptionNode*)malloc(sizeof(PrescriptionNode));
		fread(&newNode->data, sizeof(Prescription), 1, file);
		newNode->next = NULL;
		newNode->prev = *tail;

		if (*tail == NULL) {
			*head = newNode;
		}
		else {
			(*tail)->next = newNode;
		}
		*tail = newNode;
	}
}

void readHMOAdminFromBinaryFile(HMO_Admin* admin, FILE* file) {
	unsigned int compressedSize;
	if (fread(&compressedSize, sizeof(unsigned int), 1, file) != 1) {
		printf("Failed to read compressed size.\n");
		return;
	}

	unsigned char* compressedBuffer = malloc(compressedSize);
	if (!compressedBuffer) {
		printf("Memory allocation failed for decompression.\n");
		return;
	}

	if (fread(compressedBuffer, sizeof(unsigned char), compressedSize, file) != compressedSize) {
		printf("Failed to read compressed data.\n");
		free(compressedBuffer);
		return;
	}

	unsigned char decompressedBuffer[1024] = { 0 }; // Adjust size as needed
	unsigned int decompressedSize = 0;

	for (unsigned int i = 0; i < compressedSize - 1; i += 2) {
		unsigned char count = compressedBuffer[i];
		unsigned char byte = compressedBuffer[i + 1];

		if (decompressedSize + count > sizeof(decompressedBuffer)) {
			printf("Decompressed data exceeds buffer size.\n");
			free(compressedBuffer);
			return;
		}

		for (unsigned char j = 0; j < count; j++) {
			decompressedBuffer[decompressedSize++] = byte;
		}
	}

	free(compressedBuffer);

	unsigned int bufferIndex = 0;
	unsigned int nameLength;

	if (bufferIndex + sizeof(unsigned int) > decompressedSize) {
		printf("Invalid data: cannot read name length.\n");
		return;
	}
	memcpy(&nameLength, decompressedBuffer + bufferIndex, sizeof(unsigned int));
	bufferIndex += sizeof(unsigned int);

	if (bufferIndex + nameLength > decompressedSize) {
		printf("Invalid data: name length exceeds buffer.\n");
		return;
	}
	admin->name = malloc(nameLength);
	if (!admin->name) {
		printf("Memory allocation failed for admin name.\n");
		return;
	}
	memcpy(admin->name, decompressedBuffer + bufferIndex, nameLength);
	bufferIndex += nameLength;

	if (bufferIndex + sizeof(int) > decompressedSize) {
		printf("Invalid data: cannot read id.\n");
		free(admin->name);
		return;
	}
	memcpy(&admin->id, decompressedBuffer + bufferIndex, sizeof(int));
	bufferIndex += sizeof(int);

	if (bufferIndex + sizeof(int) > decompressedSize) {
		printf("Invalid data: cannot read seniority years.\n");
		free(admin->name);
		return;
	}
	memcpy(&admin->seniority_years, decompressedBuffer + bufferIndex, sizeof(int));
	bufferIndex += sizeof(int);

	if (bufferIndex + sizeof(char) > decompressedSize) {
		printf("Invalid data: cannot read gender.\n");
		free(admin->name);
		return;
	}
	memcpy(&admin->gender, decompressedBuffer + bufferIndex, sizeof(char));
}

// Text file IO
int writeHMOToTextFile(const HMO* hmo, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (!file) {
		printf("Error opening file for writing.\n");
		return -1;
	}

	// Write basic HMO data
	fprintf(file, "%d %d %d\n", hmo->numDoctors, hmo->numPatients, hmo->appointmentsNum);

	// Write address
	int addressLength = strlen(hmo->address);
	fprintf(file, "%d %s\n", addressLength, hmo->address);

	// Write doctors
	for (int i = 0; i < hmo->numDoctors; i++) {
		writeDoctorToTextFile(&hmo->doctors[i], file);
	}

	// Write patients
	for (int i = 0; i < hmo->numPatients; i++) {
		writePatientToTextFile(hmo->patients[i], file);
	}

	// Write appointments
	writeAppointmentsToTextFile(hmo->appointmentList, file);

	// Write pharmacy
	writePharmacyToTextFile(hmo->pharmacy, file);

	// Write prescriptions
	writePrescriptionsToTextFile(hmo->prescriptionListHead, file);

	// Write HMO_Admin
	writeHMOAdminToTextFile(&hmo->admin, file);

	fclose(file);
	return 1;
}

void writeDoctorToTextFile(const Doctor* doctor, FILE* file) {
	int nameLength = strlen(doctor->name);
	fprintf(file, "%d %s\n", nameLength, doctor->name);
	fprintf(file, "%d %c %s %d\n", doctor->id, doctor->gender, doctor->licence_id, doctor->eS);
}

void writePatientToTextFile(const Patient* patient, FILE* file) {
	int nameLength = strlen(patient->name);
	fprintf(file, "%d %s\n", nameLength, patient->name);
	fprintf(file, "%d %d/%d/%d %c\n", patient->id, patient->birthDate.day, patient->birthDate.month, patient->birthDate.year, patient->gender);
}

void writeAppointmentsToTextFile(NODE* appointmentList, FILE* file) {
	NODE* current = appointmentList;
	int count = 0;
	while (current) {
		count++;
		current = current->next;
	}
	fprintf(file, "%d \n", count);

	current = appointmentList;
	while (current) {
		Appointment* app = (Appointment*)current->data;
		fprintf(file, "%d %d %d/%d/%d %d:%d\n", app->doctor_id, app->patient_id, app->date.day, app->date.month, app->date.year, app->time.hour, app->time.minute);
		current = current->next;
	}
}

void writePharmacyToTextFile(const Pharmacy* pharmacy, FILE* file) {
	int addressLength = strlen(pharmacy->address);
	fprintf(file, "%d %s\n", addressLength, pharmacy->address);

	fprintf(file, "%d %d %d %d\n", pharmacy->responsible_pharmacist_id, pharmacy->numOfDrugs, pharmacy->numOfPharmacists, pharmacy->eSort);

	for (int i = 0; i < pharmacy->numOfDrugs; i++) {
		writeDrugToTextFile(pharmacy->drugs[i], file);
	}

	for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
		writePharmacistToTextFile(pharmacy->pharmacists[i], file);
	}
}

void writeDrugToTextFile(const Drug* drug, FILE* file) {
	int nameLength = strlen(drug->name);
	fprintf(file, "%d %s\n", nameLength, drug->name);
	fprintf(file, "%d/%d/%d %s\n", drug->exp_date.day, drug->exp_date.month, drug->exp_date.year, drug->serial_id);
}

void writePharmacistToTextFile(const Pharmacist* pharmacist, FILE* file) {
	int nameLength = strlen(pharmacist->name);
	fprintf(file, "%d %s\n", nameLength, pharmacist->name);
	fprintf(file, "%d %s\n", pharmacist->id, pharmacist->licence_id);
}

void writePrescriptionsToTextFile(PrescriptionNode* head, FILE* file) {
	PrescriptionNode* current = head;
	int count = 0;
	while (current) {
		count++;
		current = current->next;
	}
	fprintf(file, "%d\n", count);

	current = head;
	while (current) {
		Prescription *pres = &current->data;
		fprintf(file, "%s %d %d/%d/%d %d %d %d\n",
			pres->drug_serial_id, pres->dose,
			pres->prescription_exp.day, pres->prescription_exp.month, pres->prescription_exp.year,
			pres->patient_id, pres->doctor_id, pres->pharmacist_id);
		current = current->next;
	}
}

void writeHMOAdminToTextFile(const HMO_Admin* admin, FILE* file) {
	int nameLength = strlen(admin->name);
	fprintf(file, "%d %s\n", nameLength, admin->name);
	fprintf(file, "%d %d %c\n", admin->id, admin->seniority_years, admin->gender);
}

void readStringFromTextFile(char** strPtr, FILE* file) {
	int strLength;
	fscanf(file, "%d", &strLength);
	fgetc(file);
	*strPtr = (char*)malloc(strLength + 1);
	fgets(*strPtr, strLength + 1, file);
}

int readHMOFromTextFile(HMO** hmo, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Error opening file for writing.\n");
		return -1;
	}

	*hmo = (HMO*)malloc(sizeof(HMO));
	HMO* newHMO = *hmo;

	// Read basic HMO data
	fscanf(file, "%d %d %d", &newHMO->numDoctors, &newHMO->numPatients, &newHMO->appointmentsNum);

	// Read address
	readStringFromTextFile(&newHMO->address, file);

	// Read doctors
	newHMO->doctors = (Doctor*)malloc(sizeof(Doctor) * newHMO->numDoctors);
	for (int i = 0; i < newHMO->numDoctors; i++) {
		readDoctorFromTextFile(&newHMO->doctors[i], file);
	}

	// Read patients
	newHMO->patients = (Patient**)malloc(sizeof(Patient*) * newHMO->numPatients);
	for (int i = 0; i < newHMO->numPatients; i++) {
		newHMO->patients[i] = (Patient*)malloc(sizeof(Patient));
		readPatientFromTextFile(newHMO->patients[i], file);
	}

	// Read appointments
	readAppointmentsFromTextFile(&newHMO->appointmentList, file);

	// Read pharmacy
	newHMO->pharmacy = (Pharmacy*)malloc(sizeof(Pharmacy));
	readPharmacyFromTextFile(newHMO->pharmacy, file);

	// Read prescriptions
	readPrescriptionsFromTextFile(&newHMO->prescriptionListHead, &newHMO->prescriptionListTail, file);

	// Read HMO_Admin
	readHMOAdminFromTextFile(&newHMO->admin, file);

	fclose(file);
	return 1;

}

void readDoctorFromTextFile(Doctor* doctor, FILE* file) {
	readStringFromTextFile(&doctor->name, file);
	fscanf(file, "%d %c %s %d", &doctor->id, &doctor->gender, doctor->licence_id, &doctor->eS);
	doctor->licence_id[L_ID_MAX - 1] = '\0';
}

void readPatientFromTextFile(Patient* patient, FILE* file) {
	readStringFromTextFile(&patient->name, file);
	fscanf(file, "%d %d/%d/%d %c", &patient->id,
		&patient->birthDate.day, &patient->birthDate.month, &patient->birthDate.year, 
		&patient->gender);
}

void readAppointmentsFromTextFile(NODE** appointmentList, FILE* file) {
	int count;
	fscanf(file, "%d", &count);

	*appointmentList = NULL;
	NODE* last = NULL;

	for (int i = 0; i < count; i++) {
		Appointment* app = (Appointment*)malloc(sizeof(Appointment));
		fscanf(file, "%d %d %d/%d/%d %d:%d", 
			&app->doctor_id, &app->patient_id, 
			&app->date.day, &app->date.month, &app->date.year, 
			&app->time.hour, &app->time.minute);

		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		newNode->data = app;
		newNode->next = NULL;

		if (last == NULL) {
			*appointmentList = newNode;
		}
		else {
			last->next = newNode;
		}
		last = newNode;
	}
}

void readPharmacyFromTextFile(Pharmacy* pharmacy, FILE* file) {
	readStringFromTextFile(&pharmacy->address, file);

	fscanf(file, "%d %d %d %d", &pharmacy->responsible_pharmacist_id, &pharmacy->numOfDrugs, &pharmacy->numOfPharmacists, &pharmacy->eSort);

	pharmacy->drugs = (Drug**)malloc(sizeof(Drug*) * pharmacy->numOfDrugs);
	for (int i = 0; i < pharmacy->numOfDrugs; i++) {
		pharmacy->drugs[i] = (Drug*)malloc(sizeof(Drug));
		readDrugFromTextFile(pharmacy->drugs[i], file);
	}

	pharmacy->pharmacists = (Pharmacist**)malloc(sizeof(Pharmacist*) * pharmacy->numOfPharmacists);
	for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
		pharmacy->pharmacists[i] = (Pharmacist*)malloc(sizeof(Pharmacist));
		readPharmacistFromTextFile(pharmacy->pharmacists[i], file);
	}
}

void readDrugFromTextFile(Drug* drug, FILE* file) {
	readStringFromTextFile(&drug->name, file);

	fscanf(file, "%d/%d/%d %s", &drug->exp_date.day, &drug->exp_date.month, &drug->exp_date.year, drug->serial_id);
	drug->serial_id[S_ID_MAX - 1] = '\0';
}

void readPharmacistFromTextFile(Pharmacist* pharmacist, FILE* file) {
	readStringFromTextFile(&pharmacist->name, file);

	fscanf(file, "%d %s", &pharmacist->id, pharmacist->licence_id);
	pharmacist->licence_id[L_ID_MAX - 1] = '\0';
}

void readPrescriptionsFromTextFile(PrescriptionNode** head, PrescriptionNode** tail, FILE* file) {
	int count;
	fscanf(file, "%d", &count);
	fgetc(file);

	*head = NULL;
	*tail = NULL;

	for (int i = 0; i < count; i++) {
		PrescriptionNode* newNode = (PrescriptionNode*)malloc(sizeof(PrescriptionNode));
		Prescription* pres = &newNode->data;
		
		fscanf(file, "%s %d %d/%d/%d %d %d %d\n",
			pres->drug_serial_id, &pres->dose,
			&pres->prescription_exp.day, &pres->prescription_exp.month, &pres->prescription_exp.year,
			&pres->patient_id, &pres->doctor_id, &pres->pharmacist_id
		); 

		pres->drug_serial_id[S_ID_MAX] = '\0';
		
		newNode->next = NULL;
		newNode->prev = *tail;

		if (*tail == NULL) {
			*head = newNode;
		}
		else {
			(*tail)->next = newNode;
		}
		*tail = newNode;
	}
}

void readHMOAdminFromTextFile(HMO_Admin* admin, FILE* file) {
	readStringFromTextFile(&admin->name, file);
	fscanf(file, "%d %d %c", &admin->id, &admin->seniority_years, &admin->gender);
}
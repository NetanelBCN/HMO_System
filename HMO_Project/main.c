#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Time.h"
#include "Doctor.h"
#include "Patient.h"
#include "Pharmacy.h"
#include "Pharmacist.h"
#include "HMO.h"
#include "Appointment.h"
#include "Prescription.h"

#include <stdio.h>
#define PRINT_MAIN_MENU() \
    printf("\n--- HMO Management System ---\n"); \
    printf("1. Print HMO\n"); \
    printf("2. Print pharmacy\n"); \
    printf("3. Add new doctor\n"); \
    printf("4. Add new patient\n"); \
    printf("5. Add new drug to pharmacy\n"); \
    printf("6. Schedule an appointment\n"); \
    printf("7. Add new pharmacist\n"); \
    printf("8. Print all doctors\n"); \
    printf("9. Print all patients\n"); \
    printf("10. Print all pharmacists\n"); \
    printf("11. Print all drugs\n"); \
    printf("12. Print all appointments\n"); \
    printf("13. Print all specific patient's appointments\n"); \
    printf("14. Sort drugs\n"); \
    printf("15. Search drug\n"); \
    printf("16. Create new prescription\n"); \
    printf("17. Print all prescriptions\n"); \
    printf("18. Buy drug by prescription\n"); \
    printf("19. Create appointment approval file\n"); \
    printf("20. Save to TextFile and BinaryFile and Exit\n"); \
    printf("----------------------\n"); \
    printf("Enter your choice: ");

int get_valid_int() {
	char input[10];
	char* endptr;
	int option;

	while (1) {
		if (fgets(input, sizeof(input), stdin) != NULL) {
			option = strtol(input, &endptr, 10);
			if (endptr == input || *endptr != '\n') {
				printf("Invalid input. Please enter a valid integer: ");
			}
			else {
				return option;
			}
		}
		else {
			printf("Invalid input. Please enter a valid integer: ");
		}
	}
}

void sort_drugs_menu(Pharmacy** pharmacy) {
	int sort_option;
	if ((*pharmacy)->numOfDrugs == 0) {
		printf("\nNo drugs to sort!\n");
		return;
	}
	printf("Select sort category:\n");
	printf("1. Sort by Name\n");
	printf("2. Sort by Serial ID\n");
	printf("3. Sort by Expiration Date\n");
	printf("Enter your choice: ");
	sort_option = get_valid_int();

	switch (sort_option) {
	case 1:
		sortDrugs(pharmacy, eName);
		printf("Sorting drugs by Name...\n");
		break;
	case 2:
		sortDrugs(pharmacy, eSerialId);
		printf("Sorting drugs by Serial ID...\n");
		break;
	case 3:
		sortDrugs(pharmacy, eExpDate);
		printf("Sorting drugs by Expiration Date...\n");
		break;
	default:
		printf("Invalid option. Please try again.\n");
		sort_drugs_menu(pharmacy); // Recursively call if the option is invalid
		break;
	}
	printf("\nSorted successfuly!\n");
}





int main() {


	HMO* hmo = NULL;

	int choice;
	do {
		printf("Choose initialization option:\n");
		printf("\t1. Load from binary file\n");
		printf("\t2. Load from text file\n");
		printf("\t3. start fresh\n");

		scanf("%d", &choice);
		getchar();

		if (choice == 1) {
			printf("Loading system from binary file...\n");
			if (readHMOFromBinaryFile(&hmo, "hmo_data.bin") == 1) {
				printf("System successfully loaded from binary file.\n");
			}
			else {
				printf("Failed to load system from binary file. Initializing new system.\n");
				if (initHMO(&hmo) == -1) {
					printf("\nError occurred while initializing new system!\n");
					return -1;
				}
			}
		}
		else if (choice == 2) {
			printf("Loading system from text file...\n");
			if (readHMOFromTextFile(&hmo, "hmo_data.txt") == 1) {
				printf("System successfully loaded from text file.\n");
			}
			else {
				printf("Failed to load system from text file. Initializing new system.\n");
				if (initHMO(&hmo) == -1) {
					printf("\nError occurred while initializing new system!\n");
					return -1;
				}
			}
		}
		else if (choice == 3) {
			printf("\nInitializing new HMO system...\n");
			if (initHMO(&hmo) == -1) {
				printf("\nError occurred!\n");
				return -1;
			}
		}

		else {
			printf("invalid choice!\n\n");
		}

	} while (hmo == NULL);

	Pharmacist* ph;
	Doctor* d;
	Patient* p;
	Drug* drug;
	Appointment* a;
	Prescription* pr;

	int option;

	while (1) {
		PRINT_MAIN_MENU();
		option = get_valid_int();

		switch (option) {
		case 1:
			printf("Printing HMO...\n");
			printHMO(hmo);
			break;
		case 2:
			printf("Printing pharmacy...\n");
			printPharmacy(hmo->pharmacy);
			break;
		case 3:
			d = (Doctor*)malloc(sizeof(Doctor));
			if (!d) {
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;
			}
			if (initDoctor(&d) == -1) {
				freeDoctor(d);
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;

			}
			if (addNewDoctorToHMO(hmo, d) == -1) {
				freeDoctor(d);
				printf("\nerror occured!\n");
				break;

			}
			break;
		case 4:
			p = (Patient*)malloc(sizeof(Patient));
			if (!p) {
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;

			}
			if (initPatient(&p) == -1) {
				freePatient(&p);
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;

			}
			if (addNewPatientToHMO(hmo, p) == -1) {
				freePatient(&p);
				printf("\nerror occured!\n");
				break;

			}
			printf("\nPatient added successfuly!\n");
			break;
		case 5:
			drug = (Drug*)malloc(sizeof(Drug));
			if (!drug) {
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;

			}
			if (initDrug(&drug) == -1) {
				freeDrug(&drug);
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;

			}
			if (addDrugToPharmacy(&hmo->pharmacy, drug) == -1) {
				freeDrug(&drug);
				printf("\nerror occured!\n");
				break;

			}
			printf("\nDrug added successfuly!\n");
			break;

		case 6:
			a = (Appointment*)malloc(sizeof(Appointment));
			if (!a) {
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;

			}
			if (createAndAddAppointmentToHMO(hmo, a) == -1) {
				free(a);
				printf("\nerror occured!\n");
				break;
			}
			break;
		case 7:
			ph = (Pharmacist*)malloc(sizeof(Pharmacist));
			if (!ph) {
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;
			}
			if (initPharmacist(&ph) == -1) {
				freePharmacist(&ph);
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;
			}
			if (addPharmacistToPharmacy(&hmo->pharmacy, ph) == -1) {
				freePharmacist(&ph);
				printf("\nerror occured!\n");
				break;
			}
			break;
		case 8:
			printHMODoctors(hmo);
			break;
		case 9:
			printHMOPatients(hmo);
			break;
		case 10:
			printPharmacists(hmo->pharmacy->pharmacists, hmo->pharmacy->numOfPharmacists);
			break;
		case 11:
			printf("\n****** All Drugs list ******\n");
			printPharmacyDrugs(hmo->pharmacy);
			break;
		case 12:
			printf("\n*******All Appointments*******\n\n");
			printAppointmentsInHMO(hmo);
			break;
		case 13:
			printf("\n*********Hello Patient , please identify by id:\n");
			printPatientAppointments(hmo, getID());
			break;
		case 14:
			sort_drugs_menu(&hmo->pharmacy);
			break;
		case 15:
			searchAndPrintDrug(hmo->pharmacy);
			break;
		case 16:
			if (hmo->numDoctors == 0 || hmo->numPatients == 0 || hmo->pharmacy->numOfDrugs == 0) {
				printf("\n Can't write prescription //>> Make sure that there are doctors,patients and drugs in the system!\n");
				break;
			}
			pr = (Prescription*)malloc(sizeof(Prescription));
			if (!pr) {
				freeHMO(&hmo);
				printf("\nerror occured!\n");
				return -1;
			}
			if (initPrescription(hmo, pr) == -1) {
				free(pr);
				printf("\nerror occured!\n");
				break;
			}
			if (addPrescriptionToList(hmo, pr) == -1) {
				free(pr);
				printf("\nerror occured!\n");
				break;
			};
			printf("\nPrescription created successfuly!\n");
			printPrescriptionList(hmo);
			break;
		case 17:
			printf("\n*********All prescriptions in the system*********\n\n");
			printPrescriptionList(hmo);
			break;
		case 18:
			if (hmo->numPatients == 0) {
				printf("\nNo patients in the system, can't execute buying process!\n");
				break;
			}
			buyDrugByPrescription(hmo);
			break;
		case 19:
			printf("Creating appointment approval file...\n");
			createAppointmentApprovalFile(hmo);
			break;

		case 20:
			printf("Exiting system...\n");
			printf("Writing system to binary file...\n");
			printf("Writing system to text file...\n");
			if (writeHMOToBinaryFile(hmo, "hmo_data.bin") == 1) {
				printf("System successfully written to binary file.\n");
			}
			else {
				printf("Failed to write system to binary file.\n");
			}
			if (writeHMOToTextFile(hmo, "hmo_data.txt") == 1) {
				printf("System successfully written to text file.\n");
			}
			else {
				printf("Failed to write system to text file.\n");
			}
			freeHMO(&hmo);
			return 0;
		default:
			printf("Invalid option. Please try again.\n");
		}
	}

	return 0;
}



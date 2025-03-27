#define _CRT_SECURE_NO_WARNINGS
#include "Pharmacy.h"
#include "General.h"
#include "Drug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int initPharmacy(Pharmacy** pharmacy) {
	Pharmacy* p = *pharmacy;
	if (!p) {
		printf("\nError occurred: Invalid Pharmacy pointer!\n");
		return -1;
	}
	printf("\n*********Initializing pharmacy*********\n");
	p->numOfDrugs = 0;
	p->numOfPharmacists = 0;

	char* address = askForAddress();
	if (!address) {
		printf("\nError occurred: Failed to allocate memory for address!\n");
		return -1;
	}
	p->address = address;

	p->drugs = (Drug**)malloc(sizeof(Drug*));
	if (!p->drugs) {
		printf("\nError occurred: Memory allocation for drugs failed!\n");
		free(p->address);
		return -1;
	}

	p->pharmacists = (Pharmacist**)malloc(sizeof(Pharmacist*));  // Allocating space for an array of Pharmacists pointers
	if (!p->pharmacists) {
		printf("\nError occurred: Memory allocation for pharmacists failed!\n");
		free(p->drugs);
		free(p->address);
		return -1;
	}

	printf("\nNew Pharmacy, pharmacy's responsible pharmacist required - initializing pharmacist:\n");
	Pharmacist* newPharmacist = (Pharmacist*)malloc(sizeof(Pharmacist)); // to check
	if (!newPharmacist) {
		printf("\nError occurred: Memory allocation for pharmacists failed!\n");
		free(p->drugs);
		free(p->address);
		return -1;
	}
	if (initPharmacist(&newPharmacist) == 1) {
		if (addPharmacistToPharmacy(&p, newPharmacist) != 1) {
			printf("\nFailed to add new pharmacist to the array.\n");
			free(newPharmacist);
			return -1;
		}
	}
	else {
		free(p->drugs);
		free(p->address);
		free(newPharmacist);
		return -1;
	}
	p->responsible_pharmacist_id = newPharmacist->id;
	p->eSort = eNothing;
	printf("\nPharmacy initialized successfully!\n");
	return 1;
}

int addPharmacistToPharmacy(Pharmacy** pharmacy, Pharmacist* newPharmacist) {

	if (!pharmacy || !newPharmacist) {
		return -1;
	}

	if (addPharmacistToArray(&(*pharmacy)->pharmacists, newPharmacist, (*pharmacy)->numOfPharmacists) == -1)
		return -1;
	(*pharmacy)->numOfPharmacists++;
	return 1;
}

void printPharmacy(const Pharmacy* pharmacy) {
	if (pharmacy == NULL) {
		printf("Error: Invalid pharmacy pointer.\n");
		return;
	}

	printf("\n--- Pharmacy Information ---\n");
	printf("Address: %s\n", pharmacy->address ? pharmacy->address : "Not specified");
	printf("Responsible Pharmacist ID: %d\n", pharmacy->responsible_pharmacist_id);

	// Print pharmacists
	printf("\nThere are %d pharmacists in the pharmacy:\n", pharmacy->numOfPharmacists);
	printPharmacists(pharmacy->pharmacists, pharmacy->numOfPharmacists);
	// Print drugs
	printf("\nThere are %d drugs in the pharmacy stock:\n", pharmacy->numOfDrugs);
	printDrugs(pharmacy->drugs, pharmacy->numOfDrugs);
	printf("-----------------------------\n");
}
void printPharmacyDrugs(const Pharmacy* pharmacy) {
	if (!pharmacy)
		return;
	printDrugs(pharmacy->drugs, pharmacy->numOfDrugs);
}

void sortDrugs(Pharmacy** pharmacy, DrugSortType sortType) {

	Drug** drugs = (*pharmacy)->drugs;
	if (!drugs) {
		return;  // Nothing to sort
	}
	if ((*pharmacy)->numOfDrugs == 0) {
		printf("\nNo Drugs To Sort!\n");
	}
	int(*compareFunc)(const void*, const void*);
	switch ((DrugSortType)sortType) {
	case eName:
		compareFunc = compareByName;
		break;
	case eSerialId:
		compareFunc = compareBySerialId;
		break;
	case eExpDate:
		compareFunc = compareByExpDate;
		break;
	default:
		return;  // Invalid sort type
	}

	// Use qsort to sort the array of product pointers
	qsort(drugs, (*pharmacy)->numOfDrugs, sizeof(Drug*), compareFunc);

	// Update the sort type
	(*pharmacy)->eSort = sortType;
}

int addDrugToPharmacy(Pharmacy** pharmacy, Drug* newDrug) {
	if (!pharmacy || !newDrug)
	{
		return -1;
	}
	if (addDrugToArray(&(*pharmacy)->drugs, newDrug, (*pharmacy)->numOfDrugs) == -1)
		return -1;
	(*pharmacy)->numOfDrugs++;
	(*pharmacy)->eSort = eNothing;
	return 1;
}

int findPharmacistByIdInPharmacy(Pharmacy* p, int id) {

	return findPharmacistById(p->pharmacists, id, p->numOfPharmacists);
}


Drug* searchDrug(const Pharmacy* pharmacy, const void* key) {
	if (pharmacy->eSort == eNothing) {
		printf("The drug list is not sorted. Please sort it first.\n");
		return NULL;
	}

	int (*compFunc)(const void*, const void*);
	Drug dummyDrug = { 0 };
	Drug* dummyPtr = &dummyDrug;

	switch (pharmacy->eSort) {
	case eName:
		compFunc = compareByName;
		dummyDrug.name = (char*)key;
		break;
	case eSerialId:
		compFunc = compareBySerialId;
		strcpy(dummyDrug.serial_id, (char*)key);
		break;
	case eExpDate:
		compFunc = compareByExpDate;
		dummyDrug.exp_date = *(Date*)key;
		break;
	default:
		printf("Invalid sort type.\n");
		return NULL;
	}

	void* result = bsearch(&dummyPtr, pharmacy->drugs, pharmacy->numOfDrugs, sizeof(Drug*), compFunc);
	if (result == NULL) {
		return NULL;  // Drug not found
	}
	return *(Drug**)result;
}

void searchAndPrintDrug(Pharmacy* pharmacy) {
	if (pharmacy->eSort == eNothing) {
		printf("Please sort the drugs first.\n");
		return;
	}

	Drug* foundDrug = NULL;
	switch (pharmacy->eSort) {
	case eName: {
		char searchKey[MAX_STR_LEN];
		printf("Enter drug name to search: ");

		if (fgets(searchKey, MAX_STR_LEN, stdin) == NULL) {
			printf("Error reading input.\n");
			return;
		}

		size_t len = strlen(searchKey);
		if (len > 0 && searchKey[len - 1] == '\n') {
			searchKey[len - 1] = '\0';
		}

		foundDrug = searchDrug(pharmacy, searchKey);
		break;
	}
	case eSerialId: {
		char searchKey[S_ID_MAX];
		printf("Enter serial ID to search: ");
		if (gets_s(searchKey, S_ID_MAX) == NULL) {
			printf("Error reading input.\n");
			return;
		}
		foundDrug = searchDrug(pharmacy, searchKey);
		break;
	}
	case eExpDate: {
		Date searchDate;
		if (initDate(&searchDate) != 0) {
			printf("Error initializing date.\n");
			return;
		}
		foundDrug = searchDrug(pharmacy, &searchDate);
		break;
	}
	default:
		printf("Invalid sort type.\n");
		return;
	}

	if (foundDrug) {
		printf("\nDrug found:\n");
		printDrug(foundDrug);  // Assume you have a function to print a drug
	}
	else {
		printf("Drug not found.\n");
	}

}

int findDrugByIdInPharmacy(Pharmacy* p, const char id[S_ID_MAX]) {
	return searchDrugById(p->drugs, id, p->numOfDrugs);
}
void freePharmacy(Pharmacy** pharmacy) {
	if (pharmacy == NULL || *pharmacy == NULL) {
		return;
	}

	// Free the address
	if ((*pharmacy)->address != NULL) {
		free((*pharmacy)->address);
	}

	// Free all drugs
	for (int i = 0; i < (*pharmacy)->numOfDrugs; i++) {
		if ((*pharmacy)->drugs[i] != NULL) {
			freeDrug(&((*pharmacy)->drugs[i]));
		}
	}
	if ((*pharmacy)->drugs != NULL) {
		free((*pharmacy)->drugs);
	}

	// Free all pharmacists
	for (int i = 0; i < (*pharmacy)->numOfPharmacists; i++) {
		if ((*pharmacy)->pharmacists[i] != NULL) {
			freePharmacist(&((*pharmacy)->pharmacists[i]));
		}
	}
	if ((*pharmacy)->pharmacists != NULL) {
		free((*pharmacy)->pharmacists);
	}

	// Free the pharmacy struct itself
	free(*pharmacy);
	*pharmacy = NULL;
}
#define _CRT_SECURE_NO_WARNINGS
#include "Drug.h"
#include "General.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* drugSortTypeStrings[4] = { "Name","SerialId","Exp. Date" };

int initDrug(Drug** drug) {
	if (!drug) {
		return -1;
	}
	Drug* dr = *drug;
	char buffer[MAX_STR_LEN];
	printf("\nEnter the drug's name:\n");
	gets_s(buffer, MAX_STR_LEN);
	dr->name = (char*)calloc((int)strlen(buffer) + 1, sizeof(char));
	if (!dr->name) {
		printf("\nallocation error\n");
		exit(-1);
	}
	printf("\nEnter the Exp. date:\n");
	initDate(&dr->exp_date);
	strcpy(dr->name, buffer);
	do {
		printf("\nPlease enter the drug's serial id (%d alphanumeric characters):\n", S_ID_MAX - 1);
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			free(dr->name);
			return -1;
		}
		buffer[strlen(buffer) - 1] = '\0';
	} while (isValidlicenceID(buffer) != 1);
	strncpy(dr->serial_id, buffer, sizeof(dr->serial_id) - 1);
	dr->serial_id[sizeof(dr->serial_id) - 1] = '\0';
	return 1;
}
void printDrug(const Drug* drug) {
	if (!drug) {
		printf("Error: Invalid drug pointer\n");
		return;
	}
	printf("\n--- Drug Information ---\n");
	printf("Name: %s\n", drug->name ? drug->name : "N/A");
	printf("Exp. Date:");
	printDate(&drug->exp_date);
	printf("\nDrug Serial ID: %s\n", drug->serial_id);
	printf("-------------------------\n");
}
const Drug* searchDrugBySerialId(const Drug** drugs, int numOfDrugs, char* serial_id) {
	if (drugs == NULL || numOfDrugs <= 0) {
		return NULL;
	}
	for (int i = 0; i < numOfDrugs; i++) {
		if ((drugs[i] != NULL) && (strcmp(drugs[i]->serial_id, serial_id) == 0)) {
			return drugs[i];
		}
	}
	return NULL;
}

int addDrugToArray(Drug*** drugs, Drug* newDrug, int size) {

	if (!drugs || !newDrug) {
		return -1;
	}
	for (int i = 0; i < size; i++) {
		if (strcmp(newDrug->serial_id, (*drugs)[i]->serial_id) == 0) {
			printf("\nThere are already drug with this id/licence id in the system!\nadding new drug canceled!\n");
			return 0;
		}
	}
	Drug** temp = (Drug**)realloc(*drugs, (size + 1) * sizeof(Drug*));
	if (!temp) {
		return -1;
	}

	*drugs = temp;  // Update the pointer to the reallocated memory
	(*drugs)[size] = newDrug;
	return 1;
}

void freeDrug(Drug** drug) {
	Drug* dr = *drug;
	if (dr == NULL) {
		return;
	}
	if (dr->name != NULL) {
		free(dr->name);
		dr->name = NULL;
	}
}

/////////////////////////////////
int compareByName(const void* a, const void* b) {
	const Drug* drugA = *(const Drug**)a;
	const Drug* drugB = *(const Drug**)b;
	return strcmp(drugA->name, drugB->name);
}

int compareBySerialId(const void* a, const void* b) {
	const Drug* drugA = *(const Drug**)a;
	const Drug* drugB = *(const Drug**)b;
	return strcmp(drugA->serial_id, drugB->serial_id);
}

int compareByExpDate(const void* a, const void* b) {
	const Drug* drugA = *(const Drug**)a;
	const Drug* drugB = *(const Drug**)b;
	return compareDate(&drugA->exp_date, &drugB->exp_date);
}


void printDrugs(const Drug** drugs, int size) {
	if (!drugs) {
		printf("\nerror can't print!\n");
		return;
	}
	if (size == 0)
		printf("\nDrugs stock is empty!\n");
	for (int i = 0; i < size; i++) {
		printf("\nDrug No. %d : ", i + 1);
		printDrug(drugs[i]);
	}
}
void printSortTypes() {

	for (int i = 0; i < NUM_OF_SORTS - 1; i++) {
		printf("%d-%s\n", i + 1, drugSortTypeStrings[i]);
	}
}

int searchDrugById(Drug** drugs, const char id[S_ID_MAX], int size) {

	if (!drugs)
		return -1;
	for (int i = 0; i < size; i++) {
		if (strcmp(drugs[i]->serial_id, id) == 0)
			return i;
	}
	return -1;

}





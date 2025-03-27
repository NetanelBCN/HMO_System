#define _CRT_SECURE_NO_WARNINGS

#include "Pharmacist.h"
#include "General.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int initPharmacist(Pharmacist** pharmacist) {
	Pharmacist* ph = *pharmacist;
	if (!ph) {
		return -1;
	}
	char buffer[MAX_STR_LEN];
	ph->id = getID();

	printf("\nEnter the pharmacist's full name:\n");
	gets_s(buffer, MAX_STR_LEN);
	ph->name = (char*)calloc((int)strlen(buffer) + 1, sizeof(char));
	if (!ph->name) {
		printf("\nallocation error\n");
		return -1;
	}
	strcpy(ph->name, buffer);

	do {
		printf("\nPlease enter the pharmacist's licence id (%d alphanumeric characters):\n", L_ID_MAX - 1);
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			free(ph->name);
			return -1;
		}
		buffer[strlen(buffer) - 1] = '\0';
	} while (isValidlicenceID(buffer) != 1);
	strncpy(ph->licence_id, buffer, sizeof(ph->licence_id) - 1);
	ph->licence_id[sizeof(ph->licence_id) - 1] = '\0';

	return 1;
}

void printPharmacist(const Pharmacist* pharmacist) {
	if (!pharmacist) {
		printf("Error: Invalid pharmacist !\n");
		return;
	}
	printf("\n--- pharmacist Information ---\n");
	printf("Name: %s\n", pharmacist->name ? pharmacist->name : "N/A");
	printf("ID: %d\n", pharmacist->id);
	printf("Licence ID: %s\n", pharmacist->licence_id);
	printf("-------------------------\n");
}

void printPharmacists(const Pharmacist** pharmacists, int size) {
	if (!pharmacists)
		return;
	for (int i = 0; i < size; i++)
		printPharmacist(pharmacists[i]);

}

int isPharmacistsEqual(const Pharmacist* p1, const Pharmacist* p2) {
	if (p1 == NULL || p2 == NULL) {
		return 1;
	}
	if (p1->id == p2->id || strcmp(p1->licence_id, p2->licence_id) == 0) {
		return 0;
	}
	return 1;
}



int addPharmacistToArray(Pharmacist*** pharmacists, Pharmacist* newP, int size) {
	if (!*pharmacists || !newP) {
		return -1;
	}
	for (int i = 0; i < size; i++) {
		if (newP->id == (*pharmacists)[i]->id || strcmp((*pharmacists)[i]->licence_id, newP->licence_id) == 0) {
			printf("\nThere are already pharmacist with this id/licence id in the system!\nadding new pharmacist canceled!\n");
			return 0;
		}
	}



	Pharmacist** temp = (Pharmacist**)realloc(*pharmacists, (size + 1) * sizeof(Pharmacist*));
	if (!temp) {
		return -1;  // realloc failed, original memory is untouched
	}
	(*pharmacists) = temp;  // Update the pointer to the reallocated memory
	(*pharmacists)[size] = newP;
	printf("\nPharmacist Added Successfully!\n");
	return 1;
}

void freePharmacist(Pharmacist** ph) {
	Pharmacist* p = *ph;
	if (p == NULL) {
		return;
	}
	if (p->name != NULL) {
		free(p->name);
		p->name = NULL;
	}
	free(p);
}

int findPharmacistById(const Pharmacist** p, int id, int size) {
	if (!p)
		return -1;
	for (int i = 0; i < size; i++) {
		if (p[i]->id == id)
			return i;
	}
	return -1;
}
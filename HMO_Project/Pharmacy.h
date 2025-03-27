#pragma once
#include "Drug.h"
#include "Pharmacist.h"
#include "General.h"




typedef struct {

	char* address;
	int responsible_pharmacist_id;
	Drug** drugs;
	int numOfDrugs;
	int numOfPharmacists;
	Pharmacist** pharmacists;
	DrugSortType eSort;
} Pharmacy;

int initPharmacy(Pharmacy** pharmacy);
int addPharmacistToPharmacy(Pharmacy** pharmacy, Pharmacist* newPharmacist);
int addDrugToPharmacy(Pharmacy** pharmacy, Drug* newDrug);
void printPharmacy(const Pharmacy* pharmacy);
void sortDrugs(Pharmacy** pharmacy, DrugSortType sortType);
void printPharmacyDrugs(const Pharmacy* pharmacy);
Drug* searchDrug(const Pharmacy* pharmacy, const void* key);
void searchAndPrintDrug(Pharmacy* pharmacy);
int findPharmacistByIdInPharmacy(Pharmacy* p, int id);
int findDrugByIdInPharmacy(Pharmacy* p, const char id[S_ID_MAX]);

void freePharmacy(Pharmacy** pharmacy);
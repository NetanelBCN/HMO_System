#pragma once
#include "Date.h"
#define S_ID_MAX 7

typedef struct {
	char* name;
	Date exp_date;
	char serial_id[S_ID_MAX];
} Drug;

char* drugSortTypeStrings[4];

typedef enum {
	eName,
	eSerialId,
	eExpDate,
	eNothing,
	NUM_OF_SORTS

} DrugSortType;

int initDrug(Drug** drug);
void printDrug(const Drug* drug);
void printDrugs(const Drug** drugs, int size);
const Drug* searchDrugBySerialId(const Drug** drugs, int numOfDrugs, char* serial_id);
int addDrugToArray(Drug*** drugs, Drug* newDrug, int size);
void freeDrug(Drug** drug);
int compareByName(const void* a, const void* b);
int compareBySerialId(const void* a, const void* b);
int compareByExpDate(const void* a, const void* b);
void printSortTypes();
int searchDrugById(Drug** drugs, const char id[S_ID_MAX], int size);
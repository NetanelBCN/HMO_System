#pragma once
#include "General.h"

typedef enum {
	GENERAL,
	EYES,
	CARDIOLOGY,
	NEUROLOGY,
	ONCOLOGY,
	SURGERY,
	PSYCHIATRY,
	DERMATOLOGY,
	ORTHOPEDICS,
	GYNECOLOGY,
	UROLOGY,
	GASTROENTEROLOGY,
	ENDOCRINOLOGY,
	ESIZE
}eSpecialization;


typedef struct {

	char* name;
	int id;
	char gender;
	char licence_id[L_ID_MAX];
	eSpecialization eS;

} Doctor;

const char* MedicalSpecializationStrings[];
int initDoctor(Doctor** doctor);
int addDoctorToArray(Doctor** doctors, Doctor* newD, int size);
void printDoctor(const Doctor* doctor);
void printDoctorBasicInfo(const Doctor* doctor);
void printAllSpecializations();
int isDoctorsEqual(const Doctor* d1, const Doctor* d2);
int isDoctorExist(const Doctor* doctors, int doctorsNum, const char* licenseId);
void printDoctorsBasicInfo(Doctor* doctors, int size);
int getSpecialization();
int getDoctorIdBylicenceId(char* licenceId, const Doctor* doctors, int size);
int printDoctorsBySpecialization(const Doctor* doctors, int size, eSpecialization spec);
const Doctor* findDoctorById(const Doctor* doctors, int numDoctors, int id);
void freeDoctor(Doctor* doc);
int searchDoctorById(const Doctor* doctors, int id, int size);

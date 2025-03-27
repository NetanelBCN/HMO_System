#pragma once
#include "General.h"
#include "Date.h"



typedef struct {

	char drug_serial_id[L_ID_MAX]; 
	int dose;
	Date prescription_exp;
	int patient_id;
	int doctor_id;
	int pharmacist_id;

}Prescription;

int isPrescriptionUsed(const Prescription* p);

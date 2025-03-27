#pragma once
#include "Doctor.h"
#include "Patient.h"
#include "General.h"
#include "Prescription.h"
#include "Pharmacy.h"
#include "Appointment.h"
#include "HMO_Admin.h"
#include <stdio.h>


typedef struct PrescriptionNode {
	Prescription data;
	struct PrescriptionNode* prev;
	struct PrescriptionNode* next;
} PrescriptionNode;

typedef struct {
	char* address;
	Doctor* doctors;
	Patient** patients;
	int numDoctors;
	int numPatients;
	NODE* appointmentList;
	int appointmentsNum;
	Pharmacy* pharmacy;
	HMO_Admin admin;
	PrescriptionNode* prescriptionListHead;
	PrescriptionNode* prescriptionListTail;
} HMO;


int initHMO(HMO** hmo);
void freeHMO(HMO** hmo);
void printHMO(HMO* hmo);
void printHMODoctors(HMO* hmo);
void printHMOPatients(const HMO* hmo);
int addNewDoctorToHMO(HMO* hmo, Doctor* doctor);
int addNewPatientToHMO(HMO* hmo, Patient* patient);
void printAppointmentsInHMO(const HMO* hmo);
int createAndAddAppointmentToHMO(HMO* hmo, Appointment* newAppointment);
int searchDoctorByIdInHMO(const HMO* hmo, int id);
int searchPatientByIdInHMO(const HMO* hmo, int id);
int initPrescription(const HMO* hmo, Prescription* pr);
void printPrescription(const Prescription* pr, HMO* hmo);

int addPrescriptionToList(HMO* hmo, Prescription* pr);
void printPrescriptionList(const HMO* hmo);
void freePrescriptionList(HMO* hmo);
void printPatientAppointments(const HMO* hmo, int id);

void buyDrugByPrescription(HMO* hmo);
void createAppointmentApprovalFile(HMO* hmo);


// Binary file IO

int writeHMOToBinaryFile(const HMO* hmo, const char* filename);
void writeDoctorToBinaryFile(const Doctor* doctor, FILE* file);
void writePatientToBinaryFile(const Patient* patient, FILE* file);
void writeAppointmentsToBinaryFile(NODE* appointmentList, FILE* file);
void writePharmacyToBinaryFile(const Pharmacy* pharmacy, FILE* file);
void writeDrugToBinaryFile(const Drug* drug, FILE* file);
void writePharmacistToBinaryFile(const Pharmacist* pharmacist, FILE* file);
void writePrescriptionsToBinaryFile(PrescriptionNode* head, FILE* file);
void writeHMOAdminToBinaryFile(const HMO_Admin* admin, FILE* file);

int readHMOFromBinaryFile(HMO** hmo, const char* filename);
void readDoctorFromBinaryFile(Doctor* doctor, FILE* file);
void readPatientFromBinaryFile(Patient* patient, FILE* file);
void readAppointmentsFromBinaryFile(NODE** appointmentList, FILE* file);
void readPharmacyFromBinaryFile(Pharmacy* pharmacy, FILE* file);
void readDrugFromBinaryFile(Drug* drug, FILE* file);
void readPharmacistFromBinaryFile(Pharmacist* pharmacist, FILE* file);
void readPrescriptionsFromBinaryFile(PrescriptionNode** head, PrescriptionNode** tail, FILE* file);
void readHMOAdminFromBinaryFile(HMO_Admin* admin, FILE* file);


// Text file IO
int writeHMOToTextFile(const HMO* hmo, const char* filename);
void writeDoctorToTextFile(const Doctor* doctor, FILE* file);
void writePatientToTextFile(const Patient* patient, FILE* file);
void writeAppointmentsToTextFile(NODE* appointmentList, FILE* file);
void writePharmacyToTextFile(const Pharmacy* pharmacy, FILE* file);
void writeDrugToTextFile(const Drug* drug, FILE* file);
void writePharmacistToTextFile(const Pharmacist* pharmacist, FILE* file);
void writePrescriptionsToTextFile(PrescriptionNode* head, FILE* file);
void writeHMOAdminToTextFile(const HMO_Admin* admin, FILE* file);

int readHMOFromTextFile(HMO** hmo, const char* filename);
void readDoctorFromTextFile(Doctor* doctor, FILE* file);
void readPatientFromTextFile(Patient* patient, FILE* file);
void readAppointmentsFromTextFile(NODE** appointmentList, FILE* file);
void readPharmacyFromTextFile(Pharmacy* pharmacy, FILE* file);
void readDrugFromTextFile(Drug* drug, FILE* file);
void readPharmacistFromTextFile(Pharmacist* pharmacist, FILE* file);
void readPrescriptionsFromTextFile(PrescriptionNode** head, PrescriptionNode** tail, FILE* file);
void readHMOAdminFromTextFile(HMO_Admin* admin, FILE* file);
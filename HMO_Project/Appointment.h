#pragma once
#include "Date.h"
#include "Time.h"
#include "Doctor.h"
#include "Patient.h"
#include "General.h"

#define CONFLICT_WINDOW 15 // 15 minutes


typedef struct {
	int doctor_id;
	int patient_id;
	Date date;
	Time time;

}Appointment;

int initAppointment(Appointment* appointment, const Doctor* doctors, const Patient** patients, int numPatients, int numDoctors);
void printAppointment(const Appointment* appointment, const Doctor* doctors, const Patient** patients, int numPatients, int numDoctors);
int areAppointmentsConflicting(const Appointment* a1, const Appointment* a2);

void addAppointment(NODE** list, Appointment* newAppointment);
void removeAppointment(NODE** list, int index);
void printAppointments(NODE* list);
void freeAppointmentList(NODE** list);

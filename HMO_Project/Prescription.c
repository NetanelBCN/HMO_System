#define _CRT_SECURE_NO_WARNINGS
#include "Prescription.h"



int isPrescriptionUsed(const Prescription* p) {
	if (!p)
		return -1;
	if (p->pharmacist_id != -1)
		return 1; //used
	return 0; // not used
}

#define _CRT_SECURE_NO_WARNINGS
#include "HMO_Admin.h"
#include "General.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int initHMOAdmin(HMO_Admin* admin) {
    if (!admin) {
        return -1;
    }
    char buffer[MAX_STR_LEN];
    printf("\nEnter the HMO admin's name:\n");
    if (fgets(buffer, MAX_STR_LEN, stdin) == NULL) {
        printf("\nError reading input\n");
        return -1;
    }
    // Remove newline character if present
    buffer[strcspn(buffer, "\n")] = 0;

    admin->name = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    if (!admin->name) {
        printf("\nallocation error\n");
        return -1;
    }
    strcpy(admin->name, buffer);  // Copy the buffer content to admin->name

    admin->id = getID();
    admin->gender = getGender();
    getSeniorityYears(&admin->seniority_years);
    return 1;
}

void printHMOAdmin(const HMO_Admin* admin) {
    if (!admin) {
        printf("Error: Invalid HMO admin pointer\n");
        return;
    }
    printf("\n--- HMO Admin Information ---\n");
    printf("Name: %s | Gender: ", admin->name ? admin->name : "N/A");
    if (admin->gender == 'M')
        printf("Male");
    else
        printf("Female");
    printf("\nID: %d\n", admin->id);
    printf("Seniority Years: %d\n", admin->seniority_years);
    printf("-------------------------\n");
}

void freeHMOAdmin(HMO_Admin* admin) {
    if (admin == NULL) {
        return;
    }
    if (admin->name != NULL) {
        free(admin->name);
        admin->name = NULL;
    }
}

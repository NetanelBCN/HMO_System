#define  _CRT_SECURE_NO_WARNINGS
#define ID_LENGTH 9
#define SERIALID_LENGTH 7
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"

char* getStrExactLength(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	getsStrFixSize(temp, MAX_STR_LEN, msg);

	str = getDynStr(temp);
	return str;
}
void generalArrayFunction(void* arr, int numElements, int elementSize, void(*f)(void* element)) {
	if (arr == NULL || numElements <= 0 || elementSize <= 0 || f == NULL) {
		printf("Invalid arguments.\n");
		return;
	}

	for (int i = 0; i < numElements; i++) {
		void* element = (void*)((char*)arr + i * elementSize);
		f(element);
	}
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;
	strcpy(theStr, str);
	return theStr;
}

char* getsStrFixSize(char* buffer, int size, const char* msg)
{
	puts(msg);
	return myGets(buffer, size);
}

char* myGets(char* buffer, int size)
{
	if (buffer != NULL && size > 0)
	{
		if (fgets(buffer, size, stdin))
		{
			buffer[strcspn(buffer, "\n")] = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char** splitCharsToWords(char* str, int* pCount, const char* del, int* pTotalLength)
{
	char* word;
	int count = 0;
	char* temp = _strdup(str);
	*pTotalLength = 0;
	char** wordsArray = NULL;

	word = strtok(temp, del);
	while (word != NULL)
	{

		char** temp = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (temp != NULL)
			wordsArray = temp;
		else
			return NULL;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, del);
	}
	*pCount = count;
	free(temp);
	return wordsArray;
}

float	getPositiveFloat(const char* msg)
{
	float val;
	do {
		puts(msg);
		if (scanf("%f", &val) != 1)
			return -1;
	} while (val < 0);
	return val;
}

int		getPositiveInt(const char* msg)
{
	int val;
	do {
		puts(msg);
		if (scanf("%d", &val) != 1)
			return -1;
	} while (val < 0);
	return val;
}

int		countCharInString(const char* str, char tav)
{
	int count = 0;
	while (*str)
	{
		if (*str == tav)
			count++;
		str++;
	}
	return count;
}

int		checkEmptyString(char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str++;
	}
	return 1;
}

int isSixDigitNumber(const char* str) {
	if (strlen(str) != 6) {
		return 0;
	}

	for (int i = 0; i < 6; i++) {
		if (str[i] < '0' || str[i] > '9') {
			return 0;
		}
	}
	return 1;
}
char getGender() {
	char gender;
	do {
		printf("\nPlease enter the gender (M/F): ");
		if (scanf(" %c", &gender) != 1)
			return -1;
		while (getchar() != '\n');
		gender = toupper(gender);
		if (gender != 'M' && gender != 'F') {
			printf("Invalid input. Please enter 'M/m' for male or 'F/f' for female.\n");
		}
	} while (gender != 'M' && gender != 'F');

	return gender;
}


int isValidlicenceID(const char* licence_id) {
	if (strlen(licence_id) != L_ID_MAX - 1) {
		return 0;
	}

	for (int i = 0; i < 5; i++) {
		if (!isalnum(licence_id[i])) {
			return 0;
		}
	}

	return 1;
}

int isValidID(const char* id_str) {
	// Check if the string is exactly 9 characters long
	if (strlen(id_str) != ID_LENGTH) {
		return 0;
	}

	// Check if the first character is not '0'
	if (id_str[0] == '0') {
		return 0;
	}

	// Check if all characters are digits
	for (int i = 0; i < ID_LENGTH; i++) {
		if (!isdigit(id_str[i])) {
			return 0;
		}
	}

	return 1;
}

int isValidSerialID(const char* id_str) {
	if (strlen(id_str) != SERIALID_LENGTH - 1) {
		return 0;
	}

	for (int i = 0; i < 5; i++) {
		if (!isalnum(id_str[i])) {
			return 0;
		}
	}

	return 1;
}


int getID() {
	char id_str[MAX_STR_LEN];
	int valid_input = 0;
	int id;
	do {
		printf("\nPlease enter the id (9 digits & first digit can't be 0): ");
		if (fgets(id_str, MAX_STR_LEN, stdin) == NULL) {
			printf("Error reading input. Please try again.\n");
			continue;
		}
		id_str[strcspn(id_str, "\n")] = 0;
		valid_input = isValidID(id_str);
		if (valid_input == 0) {
			printf("Invalid input. Please enter a 9-digit number not starting with 0.\n");
		}
	} while (!valid_input);
	id = atoi(id_str);
	return id;
}

char* askForAddress() {
	char buffer[MAX_STR_LEN], * city = NULL, * street = NULL, * address = NULL;
	printf("Enter street: ");
	if (!fgets(buffer, MAX_STR_LEN, stdin) || !(street = _strdup(buffer))) {
		return NULL;
	}
	street[strcspn(street, "\n")] = 0;
	street[0] = toupper(street[0]);
	printf("Enter city: ");
	if (!fgets(buffer, MAX_STR_LEN, stdin) || !(city = _strdup(buffer))) {
		free(street);
		return NULL;
	}
	city[strcspn(city, "\n")] = 0;
	for (int i = 0; city[i]; i++) city[i] = tolower(city[i]);
	address = malloc(strlen(street) + strlen(city) + 3); // +3 for ", " and null terminator
	if (!address) {
		free(city);
		free(street);
		return NULL;
	}
	if (address) sprintf(address, "%s, %s", street, city);
	free(city);
	free(street);
	return address;
}
int getRandomInt(int min, int max) {
	if (min > max) {
		int temp = min;
		min = max;
		max = temp;
	}
	return rand() % (max - min + 1) + min;
}

int getSeniorityYears(int* years) {
	char input[100];
	char* endptr;
	long value;

	while (1) {
		printf("\nPlease enter the HMO admin's seniority years: ");

		if (fgets(input, sizeof(input), stdin) == NULL) {
			return 0;  // Error or EOF
		}

		// Remove newline if present
		input[strcspn(input, "\n")] = 0;

		// Convert string to long
		value = strtol(input, &endptr, 10);

		// Check if conversion was successful and the entire string was used
		if (endptr != input && *endptr == '\0') {
			// Check if the value is in a valid range for int
			if (value >= 0 && value <= INT_MAX) {
				*years = (int)value;
				return 1;  // Success
			}
		}

		printf("Invalid input. Please enter a valid non-negative number.\n");
	}
}
#pragma once

#define MAX_STR_LEN 255
#define DATE_LEN 13
#define L_ID_MAX 7
#define BITS_PER_BYTE 8




typedef struct node {
	void* data;
	struct node* next;
} NODE;


char* getStrExactLength(const char* msg);
char* getsStrFixSize(char* buffer, int size, const char* msg);
char* myGets(char* buffer, int size);
char* getDynStr(char* str);
char** splitCharsToWords(char* str, int* pCount, const char* del, int* pTotalLength);
int isValidlicenceID(const char* licence_id);
float	getPositiveFloat(const char* msg);
int		getPositiveInt(const char* msg);
int		countCharInString(const char* str, char tav);
int		checkEmptyString(char* str);
int isSixDigitNumber(const char* str);
void generalArrayFunction(void* arr, int numElements, int elementSize, void(*f)(void* element));
char getGender();
int getID();
//int isValidSerialID(const char* id_str);

char* askForAddress();
int getRandomInt(int min, int max);
int getSeniorityYears(int* years);
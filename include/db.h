#ifndef DB_H
#define DB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "order.h"
// Generic CSV loader enum
typedef enum { CAR_PART, ORDER, CUSTOMER } CSVType;

// Function declarations in db.c
int csv_handler();//main csv reader function
int countRecords(int,char **);
int readCSV(const char*, void *, CSVType, int);
void parseDateTime(char *, char *, DateTime*);
void parseCarPart(char *, void *);
void parseOrder(char *, void *);
void parseCustomer(char *, void *);


#endif


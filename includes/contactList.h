#ifndef __contactListh__
#define __contactListh__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct contact {
  unsigned long phone_number;
  long first_name_posn;
  long last_name_posn;
  long company_name_posn;
  long email_posn;
  long next;
} contact;

typedef struct list {
  unsigned long phone;
  int flag;
  char *first;
  char *last;
  char *company;
  char *email;
} list;

int getData(FILE *fp);
int interface(FILE *fp);
int compElements(const void *first, const void *second);
int emailCheck(char mail[]);
void writeData(struct list *info, int numberOfContacts, FILE *fp);
list *getList(FILE *fp);
list *sort(struct list *contacts, int numElements);

#endif

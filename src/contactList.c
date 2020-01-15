#include "contactList.h"

/* This function provides the contact display and asks for actions */
int interface(FILE *fp) {
  int numberOfContacts = 0;
  int i, b,
  del, phoneError, emailError;
  char action[11];
  char num[21];
  char mail[101];
  contact temp;
  list *structArray;

  fseek(fp, 0, SEEK_SET);

  /* Read in every struct to determine the number of contacts */
  while (fread(&temp, sizeof(struct contact), 1, fp) == 1) {
    fseek(fp, temp.next - ftell(fp), SEEK_CUR);
    numberOfContacts++;
  }
  printf("Number of Contacts = %d\n", numberOfContacts);

  /* Get the structures in an array of struct format */
  structArray = getList(fp);

  /* Sort the array of structs */
  structArray = sort(structArray, numberOfContacts);

  /* Display all the contacts */
  for (i = 0; i < numberOfContacts; i++) {
    if (strcmp(structArray[i].first, "\n") != 0 && strcmp(structArray[i].last, "\n") != 0)
      printf("  %d. %s %s", i+1, structArray[i].first, structArray[i].last);
    else if (strcmp(structArray[i].company, "\n") != 0)
      printf("  %d. %s", i+1, structArray[i].company);
    else if (strcmp(structArray[i].first, "\n") != 0 && (strcmp(structArray[i].company, "\n") != 0))
      printf("  %d. %s", i+1, structArray[i].last);
    printf("\n");
  }

  /* Either add contact, select contact, or exit program */
  while (1) {
    printf("Action:");
    fgets(action, 10, stdin);

    /* Add a contact if user enters 'A' */
    if (strncmp(action, "A", 1) == 0) {
      printf("\n");
      b = getData(fp);
      for (i = 0; i <= numberOfContacts; i++) {
        if (structArray[i].first != NULL)
          free(structArray[i].first);
        if (structArray[i].last != NULL)
          free(structArray[i].last);
        if (structArray[i].company != NULL)
          free(structArray[i].company);
        if (structArray[i].email != NULL)
          free(structArray[i].email);
      }
      free(structArray);
      return b;
    }

    /* Exit the program if user enters 'X' */
    else if (strncmp(action, "X", 1) == 0)
      break;

      /* Display contact information for the number the user enters */
    else {
      for (i = 1; i <= numberOfContacts; i++) {
        if (atoi(action) == i) {
          printf("\nContact #%d\n", i);
          if (strcmp(structArray[i-1].first, "\n") != 0)
            printf("First Name:%s\n", structArray[i-1].first);
          else
            printf("First Name:\n");

          if (strcmp(structArray[i-1].last, "\n") != 0)
            printf("Last Name:%s\n", structArray[i-1].last);
          else
            printf("Last Name:\n");

          if (strcmp(structArray[i-1].company, "\n") != 0)
            printf("Company Name:%s\n", structArray[i-1].company);
          else
            printf("Company Name:\n");

          printf("Phone Number:%lu\n", structArray[i-1].phone);

          if (strcmp(structArray[i-1].email, "\n") != 0)
            printf("Email:%s\n", structArray[i-1].email);
          else
            printf("Email:\n");

          while (1) {
            printf("Action:");
            fgets(action, 10, stdin);

            /* Return to contact list interface without saving if user enters 'R' */
            if (strncmp(action, "R", 1) == 0) {
              printf("\n");
              for (i = 0; i < numberOfContacts; i++) {
                if (structArray[i].first != NULL)
                  free(structArray[i].first);
                if (structArray[i].last != NULL)
                  free(structArray[i].last);
                if (structArray[i].company != NULL)
                  free(structArray[i].company);
                if (structArray[i].email != NULL)
                  free(structArray[i].email);
              }
              free(structArray);
              return 2;
            }

            /* Edit contact phone number and email if user enters 'E' */
            else if (strncmp(action, "E", 1) == 0) {
              printf("\nContact #%d\n", i);
              if (strcmp(structArray[i-1].first, "\n") != 0)
                printf("First Name:%s\n", structArray[i-1].first);
              else
                printf("First Name:\n");

              if (strcmp(structArray[i-1].last, "\n") != 0)
                printf("Last Name:%s\n", structArray[i-1].last);
              else
                printf("Last Name:\n");

              if (strcmp(structArray[i-1].company, "\n") != 0)
                printf("Company Name:%s\n", structArray[i-1].company);
              else
                printf("Company Name:\n");

              do {
                phoneError = 0;
                printf("Phone Number(enter only numbers):");
                fgets(num, 20, stdin);
                for (i = 0; i < strlen(num) - 1; i++) {
                  if (!isdigit(num[i]))
                    phoneError++;
                }
                if ((strlen(num) - 1) != 7 && (strlen(num) - 1) != 10)
                  phoneError++;
              } while (phoneError > 0);

              structArray[i-1].phone = strtoul(num, NULL, 10);

              do {
                printf("Email:");
                fgets(mail, 100, stdin);
                if (mail[0] == '\n') {
                  break;
                }
                emailError = emailCheck(mail);
              } while (emailError == -1);

              structArray[i-1].email = realloc(structArray[i-1].email, sizeof(char) * strlen(mail) + 1);
              strcpy(structArray[i-1].email, mail);

              while (1) {
                printf("Action:");
                fgets(action, 10, stdin);

                /* Return to contact list interface without saving if user enters 'R' */
                if (strncmp(action, "R", 1) == 0) {
                  printf("\n");
                  for (i = 0; i < numberOfContacts; i++) {
                    if (structArray[i].first != NULL)
                      free(structArray[i].first);
                    if (structArray[i].last != NULL)
                      free(structArray[i].last);
                    if (structArray[i].company != NULL)
                      free(structArray[i].company);
                    if (structArray[i].email != NULL)
                      free(structArray[i].email);
                  }
                  free(structArray);
                  return 2;
                }
                else if (strncmp(action, "S", 1) == 0) {
                  printf("\n");
                  break;
                }
              }
              return 1;
            }

            /* Delete the contact if user enters 'D' */
            else if (strncmp(action, "D", 1) == 0) {
              del = i - 1;
              structArray[del].flag = 1;

              /* Create fresh file */
              fclose(fp);
              fp = fopen("bin/myContactList.db", "w");
              fclose(fp);
              fp = fopen("bin/myContactList.db", "a+");
              writeData(structArray, numberOfContacts, fp);
              return 1;
            }
          }
        }
      }
    }
  }

  /* Free all data */
  for (i = 0; i < numberOfContacts; i++) {
    if (structArray[i].first != NULL)
      free(structArray[i].first);
    if (structArray[i].last != NULL)
      free(structArray[i].last);
    if (structArray[i].company != NULL)
      free(structArray[i].company);
    if (structArray[i].email != NULL)
      free(structArray[i].email);
  }
  free(structArray);
  return 0;
}

/* This function takes all the structs and stores it into an array of structs */
list* getList(FILE *fp) {
  char firstname[101];
  char lastname[101];
  char compname[101];
  char email[101];
  int posn;
  int diff;
  int counter = 0;
  contact find;
  list *tempArray;

  tempArray = malloc(sizeof(struct list) * 1);
  fseek(fp, 0, SEEK_SET);

  /* Take positions to read each contact */
  while (fread(&find, sizeof(struct contact), 1, fp) == 1) {
    posn = find.next;
    diff = posn - find.email_posn;
    if (diff == posn)
      strcpy(email, "\n");
    else {
      fseek(fp, posn - diff, SEEK_SET);
      fread(email, 1, diff, fp);
    }

    if (diff != posn)
      posn = posn - diff;
    diff = posn - find.company_name_posn;
    if (diff == posn)
      strcpy(compname, "\n");
    else {
      fseek(fp, posn - diff, SEEK_SET);
      fread(compname, 1, diff, fp);
    }

    if (diff != posn)
      posn = posn - diff;
    diff = posn - find.last_name_posn;
    if (diff == posn)
      strcpy(lastname, "\n");
    else {
      fseek(fp, posn - diff, SEEK_SET);
      fread(lastname, 1, diff, fp);
    }

    if (diff != posn)
      posn = posn - diff;
    diff = posn - find.first_name_posn;
    if (diff == posn)
      strcpy(firstname, "\n");
    else {
      fseek(fp, posn - diff, SEEK_SET);
      fread(firstname, 1, diff, fp);
    }

    tempArray[counter].first = malloc(sizeof(char) * (strlen(firstname) + 1));
    tempArray[counter].last = malloc(sizeof(char) * (strlen(lastname) + 1));
    tempArray[counter].company = malloc(sizeof(char) * (strlen(compname) + 1));
    tempArray[counter].email = malloc(sizeof(char) * (strlen(email) + 1));

    /* Set the values of a struct to a contact */
    strcpy(tempArray[counter].first, firstname);
    strcpy(tempArray[counter].last, lastname);
    strcpy(tempArray[counter].company, compname);
    tempArray[counter].phone = find.phone_number;
    strcpy(tempArray[counter].email, email);

    counter++;
    tempArray = realloc(tempArray, sizeof(struct list) * (counter + 1));
    fseek(fp, find.next - ftell(fp), SEEK_CUR);
  }

  /* Return the array of structures */
  return tempArray;
}

/* Function that gets data from the user */
int getData(FILE *fp) {
  char fname[101];
  char lname[101];
  char cname[101];
  char mail[101];
  char num[21];
  char addAction[11];
  int one = 1;
  int i, phoneError, emailError;
  list info;

  /* Get input from user, and store it into a structure called info */
  printf("First Name:");
  fgets(fname, 100, stdin);
  info.first = malloc(sizeof(char) * strlen(fname) + 1);
  strcpy(info.first, fname);

  /* User must enter ether a last name or company name */
  do {
    printf("Last Name:");
    fgets(lname, 100, stdin);

    printf("Company Name:");
    fgets(cname, 100, stdin);

  } while (strlen(lname) == 1 && strlen(cname) == 1);

  info.last = malloc(sizeof(char) * strlen(lname) + 1);
  strcpy(info.last, lname);
  info.company = malloc(sizeof(char) * strlen(cname) + 1);
  strcpy(info.company, cname);

  /* Dummy proof so a proper phone number must be entered */
  do {
    phoneError = 0;
    printf("Phone Number(enter only numbers):");
    fgets(num, 20, stdin);
    for (i = 0; i < strlen(num) - 1; i++) {
      if (!isdigit(num[i]))
        phoneError++;
    }
    if ((strlen(num) - 1) != 7 && (strlen(num) - 1) != 10)
      phoneError++;
  } while (phoneError > 0);

  /* Convert string to unsigned long */
  info.phone = strtoul(num, NULL, 10);

  /* Email must follow proper email format */
  do {
    printf("Email:");
    fgets(mail, 100, stdin);
    if (strcmp(mail, "\n") == 0) {
      break;
    }
    emailError = emailCheck(mail);
  } while (emailError == -1);

  info.email = malloc(sizeof(char) * strlen(mail) + 1);
  strcpy(info.email, mail);

  /* Either save the contact or go back to interface */
  while(1) {
    printf("Action:");
    fgets(addAction, 10, stdin);

    if (strncmp(addAction, "S", 1) == 0) {
      printf("\n");
      break;
    }
    else if (strncmp(addAction, "R", 1) == 0) {
      printf("\n");
      free(info.first);
      free(info.last);
      free(info.company);
      free(info.email);
      return 2;
    }
  }

  writeData(&info, one, fp);

  /* Free allocated data */
  free(info.first);
  free(info.last);
  free(info.company);
  free(info.email);
  return 1;
}

/* This function takes the input and writes it into a file */
void writeData(struct list *info, int numberOfContacts, FILE *fp) {
  int len1, len2, len3, len4, i;
  contact contact;

  fseek(fp, 0, SEEK_SET);

  for (i = 0; i < numberOfContacts; i++) {

    /* Seek to end of file and calculate position of contact */
    if (info[i].flag != 1) {
      contact.phone_number = info[i].phone;
      if (strcmp(info[i].first, "\n") == 0) {
        contact.first_name_posn = 0;
        len1 = 0;
      }
      else {
        len1 = strlen(info[i].first);
        fseek(fp, sizeof(struct contact), SEEK_END);
        contact.first_name_posn = ftell(fp);
      }

      if (strcmp(info[i].last, "\n") == 0) {
        contact.last_name_posn = 0;
        len2 = 0;
      }
      else {
        len2 = strlen(info[i].last);
        fseek(fp, sizeof(struct contact) + len1, SEEK_END);
        contact.last_name_posn = ftell(fp);
      }

      if (strcmp(info[i].company, "\n") == 0) {
        contact.company_name_posn = 0;
        len3 = 0;
      }
      else {
        len3 = strlen(info[i].company);
        fseek(fp, sizeof(struct contact) + len1 + len2, SEEK_END);
        contact.company_name_posn = ftell(fp);
      }

      if (strcmp(info[i].email, "\n") == 0) {
        contact.email_posn = 0;
        len4 = 0;
      }
      else {
        len4 = strlen(info[i].email);
        fseek(fp, sizeof(struct contact) + len1 + len2 + len3, SEEK_END);
        contact.email_posn = ftell(fp);
      }
      fseek(fp, sizeof(struct contact) + len1 + len2 + len3 + len4, SEEK_END);
      contact.next = ftell(fp);

      /* Write the contact structure then write the input given by the user to the file */
      fwrite(&contact, sizeof(struct contact), 1, fp);

      /* Remove the newline from each string */
      info[i].first = strtok(info[i].first, "\n");
      info[i].last = strtok(info[i].last, "\n");
      info[i].company = strtok(info[i].company, "\n");
      info[i].email = strtok(info[i].email, "\n");

      /* If the input is NULL or '\n' then do not write the data */
      if (info[i].first != NULL)
        fwrite(info[i].first, strlen(info[i].first) + 1, sizeof(char), fp);
      if (info[i].last != NULL)
        fwrite(info[i].last, strlen(info[i].last) + 1, sizeof(char), fp);
      if (info[i].company != NULL)
        fwrite(info[i].company, strlen(info[i].company) + 1, sizeof(char), fp);
      if (info[i].email != NULL)
        fwrite(info[i].email, strlen(info[i].email) + 1, sizeof(char), fp);

    }
    fseek(fp, contact.next, SEEK_SET);
  }
}

/* Set up function to use qsort */
list *sort(struct list *contacts, int numElements) {
  int (*compPtr)(const void *a, const void *b);
  compPtr = &compElements;
  qsort(contacts, numElements, sizeof(struct list), compPtr);
  return contacts;
}

/* Sort array of structures by last name, then company name */
int compElements(const void *first, const void *second) {
  list *a = (struct list*)first;
  list *b = (struct list*)second;

  if (strcmp(a->last, "\n") != 0 && strcmp(b->last, "\n") != 0)
    return strcmp(a->last, b->last);
  if (strcmp(a->last, "\n") == 0 && strcmp(b->last, "\n") != 0)
    return strcmp(a->company, b->last);
  if (strcmp(a->last, "\n") != 0 && strcmp(b->last, "\n") == 0)
    return strcmp(a->last, b->company);
  if (strcmp(a->last, "\n") == 0 && strcmp(b->last, "\n") == 0)
    return strcmp(a->company, b->company);
  return 1;
}

/* Check peoper email input */
int emailCheck(char mail[]) {
  int emailError, atCounter, dotCounter, i, index;
  emailError = 0;
  atCounter = 0;
  dotCounter = 0;
  for (i = 0; i < strlen(mail) - 1; i++) {
    if (mail[i] == '@') {
      atCounter++;
      if (isalnum(mail[i - 1]) && isalnum(mail[i + 1])) {
        for (index = i; index < strlen(mail) - 1; index++) {
          if (mail[index] == '.') {
            if (isalnum(mail[index - 1])) {
              if (!isalnum(mail[index + 1])) {
                emailError++;
              }
            }
          }
          if (mail[index] == '.') {
            dotCounter++;
          }
        }
      }
    }
  }
  if (emailError == 0 && atCounter == 1 && dotCounter == 1)
    return 0;
  else
    return -1;
}

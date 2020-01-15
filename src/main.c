#include "contactList.h"

int main() {
  int a;
  FILE *fp;

  /* Open file */
  fp = fopen("bin/myContactList.db", "a+");
  if (fp == NULL) {
    fp = fopen("bin/myContactList.db", "a+");
  }

  /* Loop interface until user decides to exit program */
  do {
    a = interface(fp);
  } while (a != 0);

  /* Close file */
  fclose(fp);
  return 0;
}

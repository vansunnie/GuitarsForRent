#ifndef UTILS_H
#define UTILS_H

pgui readGuiFile(char *guiFile, int *vecSize);
pcli readCliFile(char *file, pcli start);

void saveVecToFile(char *file, pgui vec, int vecSize);
void saveListToFile(char *file, pcli listCli);
void deleteListClients(pcli listCli);
void transferClientBlackList(char name[SIZE], int id, char reason[SIZE]);

int daysOfRental(int initDay, int initMonth, int initYear, int expDay, int expMonth, int expYear);
void dayOfDelivery(int *initDay, int *initMonth, int *initYear, int *expDay, int *expMonth, int *expYear);

#endif /* UTILS_H */

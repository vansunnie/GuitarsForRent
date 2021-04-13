#ifndef GUITAR_H
#define GUITAR_H
#include <stdbool.h>

int checkGuitarExists(int id, pgui vec, int vecSize);
pgui addGuitar(pgui vec, int *vecSizeint, int id, int dailyPrice, int totalPrice, char name[SIZE]);
void showGuitars(pgui vec, int vecSize);
void showRentedGui(pgui vec, pcli listCli, int vecSize);
void showGuitarHistory(pgui vec, pcli listCli, int vecSize, int guitID);

#endif /* GUITAR_H */


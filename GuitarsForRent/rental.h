#ifndef RENTAL_H
#define RENTAL_H

pcli addRental(pcli listCli, pgui vec, int vecSize, int guitID, int cliID, int currentDay, int currentMonth,int currentYear);
pcli finishRental(pcli listCli, pgui vec, int vecSize, int cliID, int guitID, int guitState, int currentDay, int currentMonth,int currentYear);
void showRentedGuitars(pcli listCli, pgui vec, int vecSize, int currentDay, int currentMonth, int currentYear);

#endif /* RENTAL_H */

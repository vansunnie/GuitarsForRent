#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "storeStruct.h"
#include "utils.h"
#include "client.h"

int checkActiveRentals(pcli listCli, int cliID){
    prent tempRent = NULL;
    pcli tempCli = listCli;
    int activeRentals = 0;

    for(tempCli = listCli; tempCli; tempCli = tempCli->next){
        if(tempCli->ID == cliID){
            for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                //check if rental is on going
                if(tempRent->rentalState == 0)
                    activeRentals++;
            }
        }
    }
    return activeRentals;
}

void checkCheapExpensiveRentalsDone(pcli listCli, int cliID, pgui vec, int vecSize, int *cheapRentals, int *expensiveRentals){
    prent tempRent = NULL;
    pcli tempCli = listCli;
    (*cheapRentals) = 0;
    (*expensiveRentals) = 0;

    for(tempCli = listCli; tempCli; tempCli = tempCli->next){
        if(tempCli->ID == cliID){
            for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                //check if rental is on going
                if(tempRent->rentalState != 0)
                    for(int i = 0; i < vecSize; i++){
                        if(tempRent->IDGuitar == vec[i].ID){
                            //check if it was a cheap rental or expensive
                            if(vec[i].value < EXPENSIVEGUIT)
                                cheapRentals++;
                            else
                                expensiveRentals++;
                        }
                    }
            }
        }
    }
}

bool checkRentedByClient(pcli listCli, int cliID, pgui vec, int guitPos){
    prent tempRent = NULL;
    pcli tempCli = listCli;

    for(tempCli = listCli; tempCli; tempCli = tempCli->next){
        if(tempCli->ID == cliID){
            for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                //check if rental is on going
                if(tempRent->rentalState == 0)
                    if(vec[guitPos].ID == tempRent->IDGuitar)
                        return true;
            }
        }
    }

    return false;
}

int checkNumberDamagedDeliveries(pcli listCli, int cliID){
    int numberOfDamagedGuitars = 0;

    prent tempRent = NULL;
    pcli tempCli = listCli;

    for(tempCli = listCli; tempCli; tempCli = tempCli->next){
        if(tempCli->ID == cliID){
            for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                //check if rental was finished with damaged guitar
                if(tempRent->rentalState == 2)
                    numberOfDamagedGuitars++;
            }
        }
    }

    return numberOfDamagedGuitars;
}

int checkNumberDaysDeliveryDelay(pcli listCli, int cliID){
    int totalDaysDelay = 0;
    prent tempRent = NULL;
    pcli tempCli = listCli;

    for(tempCli = listCli; tempCli; tempCli = tempCli->next){
        if(tempCli->ID == cliID){
            for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                //check if rental was finished already
                if(tempRent->rentalState != 0){
                    int rentalDays = daysOfRental(tempRent->initialDate.day, tempRent->initialDate.month, tempRent->initialDate.year,
                                                  tempRent->deliveryDate.day, tempRent->deliveryDate.month, tempRent->deliveryDate.year);
                    if(rentalDays > MAXRENTALDAYS)
                        totalDaysDelay += (rentalDays - MAXRENTALDAYS);
                }
            }
        }
    }

    return totalDaysDelay;
}

pcli addRental(pcli listCli, pgui vec, int vecSize, int guitPos, int cliID, int currentDay, int currentMonth,int currentYear){
    int totalPriceToPay = 0, expensiveRentals = 0, cheapRentals = 0;
    pcli tempClient = listCli;
    prent pRental = NULL;
    int initDay = currentDay, initMonth = currentMonth, initYear = currentYear, expDay = 0, expMonth = 0, expYear = 0;

    //check if list is empty
    if(!listCli){
        printf("\nThere's no clients registered!\n\n");
        return listCli;
    }
    //check if vector is empty
    if(!vec || vecSize <= 0){
        printf("\nThere's no guitars registered");
        return listCli;
    }

    //check if client can make more rentals
    if(checkActiveRentals(listCli, cliID) >= MAXRENTALS){
        printf("\nThis client can't rent more guitars because already have %d guitars rented!\n\n", MAXRENTALS);
        return listCli;
    }

    //check cheapRentals done
    checkCheapExpensiveRentalsDone(listCli, cliID, vec, vecSize, &cheapRentals, &expensiveRentals);
    //if its an expensive guitar, check if client can rent it
    if(vec[guitPos].value >= EXPENSIVEGUIT && cheapRentals <= NUMBERCHEAPGUIT){
        printf("\nThis client still can't rent expensive guitars!\n\n");
        return listCli;
    }
    //check if guitar is available to rent
    if(vec[guitPos].stateGuitar != 0){
        printf("\nThis guitar is not available at the moment!\n\n");
        return listCli;
    }
    else{
        vec[guitPos].stateGuitar = 1; //guitar is rented
        totalPriceToPay = vec[guitPos].dailyPrice*MAXRENTALDAYS;

        prent newRental = malloc(sizeof(rentals));
        if(newRental == NULL){
            printf("\nError allocating memory!\n");
            exit(0);
        }
        newRental->next = NULL;

        for(tempClient = listCli; tempClient; tempClient = tempClient->next){
            printf("\nESTAMOS NO CLIENTE: %d", tempClient->ID);
            if(tempClient->ID == cliID){
                printf("\nENCONRAMOS O CLIENT: %d", tempClient->ID);
                tempClient->numbRentals = tempClient->numbRentals+1;
                newRental->IDGuitar = vec[guitPos].ID;
                newRental->rentalState = 0;
                newRental->initialDate.day = currentDay;
                newRental->initialDate.month = currentMonth;
                newRental->initialDate.year = currentYear;
                newRental->deliveryDate.day = 0;
                newRental->deliveryDate.month = 0;
                newRental->deliveryDate.year = 0;

                //add this newRental to client rentals list
                if(tempClient->start == NULL){ //if still no rentals
                    tempClient->start = newRental;
                    tempClient->start->next = NULL;
                } else{
                    pRental = tempClient->start;
                    while(pRental->next != NULL)
                        pRental = pRental->next;

                    pRental->next = newRental;
                }

                printf("\nNew Rental added successfully!!\n\n");

                dayOfDelivery(&initDay, &initMonth, &initYear, &expDay, &expMonth, &expYear);

                printf("\nDeadline for delivery: %d/%d/%d", expDay, expMonth, expYear);
                printf("\nMax value to pay: %d\n\n", totalPriceToPay);
            }
        }
    }

    return listCli;
}


pcli finishRental(pcli listCli, pgui vec, int vecSize, int cliID, int guitPos, int guitState, int currentDay, int currentMonth,int currentYear){

    char reason1[SIZE]= "Damaged Guitars";
    char reason2[SIZE] = "Late Delivery";

    pcli tempCli = listCli;
    prent tempRent = NULL;
    int rentalPrice = 0, rentalDamaged = 0, startDay = 0, startMonth = 0, startYear = 0, numberRentalDays = 0, extraPayment = 0, daysDelayDelivery = 0;

    //check if list is empty
    if(!listCli){
        printf("\nThere's no clients registered!\n\n");
        return listCli;
    }
    //check if vector is empty
    if(!vec){
        printf("\nThere's no guitars registered");
        return listCli;
    }

    //find client
    for(tempCli = listCli; tempCli; tempCli = tempCli->next){
        if(tempCli->ID == cliID){
                //find guitar in rentals list
                for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                    if(tempRent->IDGuitar == vec[guitPos].ID){
                        //if guitar delivered with no damage
                        if(guitState == 0){
                            tempRent->rentalState = 1; //delivered without damage
                            rentalPrice = vec[guitPos].dailyPrice;
                            vec[guitPos].stateGuitar = 0; //guitar available to rent once again

                            printf("\nGuitar delivered successfully!\n\n");
                        } else if(guitState == 1){ //if delivered damaged
                            tempRent->rentalState = 2; //delivered with damage
                            rentalPrice = vec[guitPos].dailyPrice;
                            rentalDamaged = checkNumberDamagedDeliveries(listCli, cliID);
                            vec[guitPos].stateGuitar = 2;
                            extraPayment = vec[guitPos].value;

                            printf("\nClient will have to pay an extra %d for damaged guitar!\n\n", extraPayment);

                            //check if delivered more than max of delivered guitars with damage
                            if(rentalDamaged > MAXDAMAGE){
                                printf("\nClient exceeded max number of guitars they can deliver with damage. Client will be transfered to the black list and can't do any more rentals!");
                                transferClientBlackList(tempCli->nameClient, cliID, reason1);

                                //deliver all guitars
                                for(tempRent = listCli->start; tempRent; tempRent = tempRent->next){
                                    if(tempRent->rentalState == 0){
                                        for(int i = 0; i < vecSize; i++){
                                            if(tempRent->IDGuitar == vec[i].ID)
                                                vec[i].stateGuitar = 0;
                                        }
                                    }
                                }
                                listCli = removeClient(listCli, cliID, vecSize, vec);

                                return listCli;
                            }
                        }

                        //update delivery date of rental
                        startDay = tempRent->initialDate.day;
                        startMonth = tempRent->initialDate.month;
                        startYear = tempRent->initialDate.year;
                        tempRent->deliveryDate.day = currentDay;
                        tempRent->deliveryDate.month = currentMonth;
                        tempRent->deliveryDate.year = currentYear;

                        numberRentalDays = daysOfRental(startDay, startMonth, startYear, currentDay, currentMonth, currentYear);
                        rentalPrice = numberRentalDays*rentalPrice;

                        printf("Total price to pay: %d\n", rentalPrice);

                        if(numberRentalDays > MAXRENTALDAYS){ //if exceed max number of days guitar can be rented
                            daysDelayDelivery = numberRentalDays - MAXRENTALDAYS;
                            printf("\nGuitar was delivered with %d day(s) of delay", daysDelayDelivery);
                            extraPayment = DELAYPENALTY * (daysDelayDelivery);
                            printf("\nClient will have to pay an extra of: %d\n", extraPayment);

                            //check if sum of all delays is already more than max
                            if(checkNumberDaysDeliveryDelay(listCli, cliID) > MAXDELIVERYDAYS){
                                printf("\nClient exceeded max number (%d days) of days that he can deliver guitars with delay!\n", MAXDELIVERYDAYS);
                                printf("\nThis client have to delivery all guitars he is currently renting and he cannot make new rentals, and client is being transfered to the blacklist!\n");

                                transferClientBlackList(tempCli->nameClient, cliID, reason2);
                                //deliver all guitars
                                for(tempRent = listCli->start; tempRent; tempRent = tempRent->next){
                                    if(tempRent->rentalState == 0){
                                        for(int i = 0; i < vecSize; i++){
                                            if(tempRent->IDGuitar == vec[i].ID)
                                                vec[i].stateGuitar = 0;
                                        }
                                    }
                                }

                                //remove client from list
                                listCli = removeClient(listCli, cliID, vecSize, vec);

                                return listCli;

                            }
                        }
                    }
                }
        }
    }

    printf("\nTotal price to pay: %d + %d = %d\n",rentalPrice, extraPayment, rentalPrice+extraPayment);

    return listCli;
}

void showRentedGuitars(pcli listCli, pgui vec, int vecSize, int currentDay, int currentMonth, int currentYear){

    pcli tempClient = NULL;
    prent tempRent = NULL;
    int rentalDay = 0, rentalMonth = 0, rentalYear = 0, deliveryDay = 0, deliveryMonth = 0, deliveryYear = 0, delayDeliveryDays = 0;

    if(!listCli){
        printf("\nThere's no registered clients!\n\n");
    } else if(!vec || vecSize <= 0){
        printf("\nThere's no registered guitars!\n\n");
    } else{
        for(tempClient = listCli; tempClient; tempClient = tempClient->next){
            for(tempRent = tempClient->start; tempRent; tempRent = tempRent->next){
                if(tempRent->rentalState == 0){
                    deliveryDay = 0;
                    deliveryMonth = 0;
                    deliveryYear = 0;
                    rentalDay = tempRent->initialDate.day;
                    rentalMonth = tempRent->initialDate.month;
                    rentalYear = tempRent->initialDate.year;
                    dayOfDelivery(&rentalDay, &rentalMonth, &rentalYear, &deliveryDay, &deliveryMonth, &deliveryYear);
                    printf("\nGuitar ID %d \n\tRented by %s with ID %d\n\tRented on %d/%d/%d\n\tShould be delivered on %d/%d/%d\n",
                           tempRent->IDGuitar, tempClient->nameClient, tempClient->ID,
                           rentalDay, rentalMonth, rentalYear, deliveryDay, deliveryMonth, deliveryYear);
                    delayDeliveryDays = daysOfRental(rentalDay, rentalMonth, rentalYear, currentDay, currentMonth, currentYear);
                    if(delayDeliveryDays > MAXRENTALDAYS){
                        printf("\n\tRental with %d day(s) of delay!\n\n", (delayDeliveryDays - MAXRENTALDAYS));
                    }
                    else
                        printf("\n\tRental still in day!\n\n");
                }
            }
        }
    }


}



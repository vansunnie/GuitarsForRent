#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storeStruct.h"

//function to read file of guitars a "save" it to a vector
pgui readGuiFile(char *guiFile, int *size){

    int id = 0, dailyPrice = 0, price = 0, state = 0;
    //id - guitar id; dailyPrice - guitar daily price; price - guitar price ; state - guitar state
    char name[SIZE];
    FILE *f = NULL;
    *size = 0;

    f = fopen(guiFile, "r");

    //check if file exists
    if(!f){
        printf("Error opening file %s\n", guiFile);
        return NULL;
    }

    //allocate memory for the guitars
    pgui vec = malloc(sizeof(guitars));

    //check if allocated correctly
    if(vec == NULL){
        printf("Error allocating memory");
        exit(0);
    }

    //while there's something in the file it will read it
    //guitar text file have the following format: ID DailyPrice Price State Name
    while(fscanf(f, "%d %d %d %d %49[^\n]", &id, &dailyPrice, &price, &state, name) == 5){

        (*size)++; //to increase size of vector
        pgui nvec = vec;

        //reallocate memory
        nvec = realloc(vec, sizeof(guitars)*(*size));

        //check if reallocate properly
        if(nvec == NULL){
            printf("Error reallocating memory");
            exit(0);
        }

        vec = nvec;

        //insert information from file to vector
        vec[*size-1].ID = id;
        vec[*size-1].dailyPrice = dailyPrice;
        vec[*size-1].value = price;
        vec[*size-1].stateGuitar = state;
        strcpy(vec[*size-1].nameGuitar, name);

        //DEBUG
        //printf("%d\t%d\t%d\t%d\t%s\n", vec[*size-1].ID, vec[*size-1].dailyPrice, vec[*size-1].value, vec[*size-1].stateGuitar, &vec[*size-1].nameGuitar);
    }

    fclose(f);

    return vec;
}

//function to read clients file and "save" it to a linked list
pcli readCliFile(char *file, pcli startClient){
    FILE *f = NULL;

    pcli currentClient = NULL; //pointer to client structure
    clients newCLient; //variable where initial clients data will be read to

    prent currentRental = NULL; //pointer to read rental structure
    rentals newRental; //variable where initial rentals data will be read to

    f = fopen(file, "r");
    if(!f){
        printf("Error opening file %s\n", file);
        exit(0);
    }
    while(fscanf(f, "%d %d %49[^\n]", &newCLient.ID, &newCLient.numbRentals, newCLient.nameClient)==3){

        //allocate memory for clients
        pcli tempClient = malloc(sizeof(clients)); //temporary variable to save information
        if(tempClient == NULL){
            printf("Error allocating memory.\n");
            exit(0);
        }

        //save in temporary variable what is reading from the file
        tempClient->ID = newCLient.ID;
        tempClient->numbRentals = newCLient.numbRentals;
        strcpy(tempClient->nameClient, newCLient.nameClient);

        //initialize pointers of next client and their list of rentals to NULL
        tempClient->next = NULL;
        tempClient->start = NULL;

        //check how many rentals the client has
        for(int i = 0; i < tempClient->numbRentals; i++){
            fscanf(f, "%d %d %d %d %d %d %d %d",
                   &newRental.IDGuitar,
                   &newRental.rentalState,
                   &newRental.initialDate.day,
                   &newRental.initialDate.month,
                   &newRental.initialDate.year,
                   &newRental.deliveryDate.day,
                   &newRental.deliveryDate.month,
                   &newRental.deliveryDate.year);

            prent tempRental = malloc(sizeof(rentals));

            if(tempRental == NULL){
                    printf("Error allocating memory.\n");
                    exit(0);
            }

            //put in the tempRental what read before to newRental
            tempRental->IDGuitar = newRental.IDGuitar;
            tempRental->rentalState = newRental.rentalState;
            tempRental->initialDate.day = newRental.initialDate.day;
            tempRental->initialDate.month = newRental.initialDate.month;
            tempRental->initialDate.year = newRental.initialDate.year;
            tempRental->deliveryDate.day = newRental.deliveryDate.day;
            tempRental->deliveryDate.month = newRental.deliveryDate.month;
            tempRental->deliveryDate.year = newRental.deliveryDate.year;

            //pointer of next rental to stay as NULL
            tempRental->next = NULL;

            //if there's still no rentals, this new one will be in the first position
            if(tempClient->start == NULL){
                    tempClient->start = tempRental;
                    currentRental = tempClient->start;
            } else{ //in case there's registered rentals already
                currentRental->next = tempRental;
                currentRental = currentRental->next;
            }

        }

        //if no clients in linked list, this client will stay in 1st pos
        if(startClient == NULL){
            startClient = tempClient;
            currentClient = startClient;
        } else{ //if there is, it will insert the new one in the free position
            currentClient->next = tempClient;
            currentClient = currentClient->next;
        }
    }

    fclose(f);

    //DEBUG
    /*
    for(currentClient = startClient; currentClient; currentClient = currentClient->next){

        printf("%d %d %s\n", currentClient->ID, currentClient->numbRentals, currentClient->nameClient);

        for(currentRental = currentClient->start; currentRental; currentRental = currentRental->next){
            printf("%d\t", currentRental->IDGuitar);
            printf("%d\t", currentRental->rentalState);
            printf("%d ", currentRental->initialDate.day);
            printf("%d ", currentRental->initialDate.month);
            printf("%d\t", currentRental->initialDate.year);
            printf("%d ", currentRental->deliveryDate.day);
            printf("%d ", currentRental->deliveryDate.month);
            printf("%d\n", currentRental->deliveryDate.year);
        }
        putchar('\n');
    }*/

    return startClient;

}

void saveVecToFile(char *file, pgui vec, int vecSize){
    FILE *f = fopen(file, "w");

    if(!f){
        printf("Error opening file %s\n", file);
        exit(0);
    }

    for(int i = 0; i < vecSize; i++){
        fprintf(f, "%d\t", vec[i].ID);
        fprintf(f, "%d\t", vec[i].dailyPrice);
        fprintf(f, "%d\t", vec[i].value);
        fprintf(f, "%d\t", vec[i].stateGuitar);
        fprintf(f, "%s\n", &vec[i].nameGuitar);
    }
    fclose(f);
    free(vec);
}


void saveListToFile(char *file, pcli listCli){
    FILE *f = fopen(file, "wt");

    if(!f){
        printf("\nError opening file %s\n", file);
        exit(0);
    }

    prent pRental = NULL;

    while(listCli != NULL){
        //write the client
        fprintf(f, "%d\t%d\t%s", listCli->ID, listCli->numbRentals, &listCli->nameClient);
        //write client rentals
        for(pRental = listCli->start; pRental; pRental = pRental->next){
            fprintf(f, "\n%d\t",pRental->IDGuitar);
            fprintf(f, "%d\t",pRental->rentalState);
            fprintf(f, "%d ",pRental->initialDate.day);
            fprintf(f, "%d ",pRental->initialDate.month);
            fprintf(f, "%d\t",pRental->initialDate.year);
            fprintf(f, "%d ",pRental->deliveryDate.day);
            fprintf(f, "%d ",pRental->deliveryDate.month);
            fprintf(f, "%d",pRental->deliveryDate.year);
        }
        fprintf(f, "\n\n");

        listCli = listCli->next;
    }

    fclose(f);
}


void deleteListClients(pcli listCli){
    pcli tempClient = NULL;

    while(!listCli){
        tempClient = listCli;
        listCli = listCli->next;
        free(tempClient);
    }
}

void transferClientBlackList(char name[SIZE], int id, char reason[SIZE]){
    FILE *f = fopen(binFile, "ab");
    if(!f){
        printf("\nError opening file %s", binFile);
        exit(0);
    }

    blacklist bannedClient;
    strcpy(bannedClient.name, name);
    bannedClient.ID = id;
    strcpy(bannedClient.reason, reason);


    fwrite(&bannedClient, sizeof(blacklist), 1, f);
    fclose(f);
}

int daysOfRental(int initDay, int initMonth, int initYear, int expDay, int expMonth, int expYear){
    int total=0;
    int totalInitDays = 0; //number of the day in that year that guitar was rented
    int totalExpDays = 0; //number of the day in that year that guitar was delivered

    initYear =(initYear-1)*365;
    expYear = (expYear-1)*365;

    //how many days depending on the month
    if(initMonth == 1){
        initMonth = 0;
    }
    else{
        if(initMonth==2){
            initMonth=31;
        }
        else{
            if(initMonth==3){
                initMonth=31+28;
            }
            else{
                if(initMonth==4){
                    initMonth=31+28+31;
                }
                else{
                    if(initMonth==5){
                        initMonth=31+28+31+30;
                    }
                    else{
                        if(initMonth==6){
                            initMonth=31+28+31+30+31;
                        }
                        else{
                            if(initMonth==7){
                                initMonth=31+28+31+30+31+30;
                            }
                            else{
                                if(initMonth==8){
                                    initMonth=31+28+31+30+31+30+31;
                                }
                                else{
                                    if(initMonth==9){
                                        initMonth=31+28+31+30+31+30+31+31;
                                    }
                                    else{
                                        if(initMonth==10){
                                            initMonth=31+28+31+30+31+30+31+31+30;
                                        }
                                        else{
                                            if(initMonth==11){
                                                initMonth=31+28+31+30+31+30+31+31+30+31;
                                            }
                                            else{
                                                initMonth=31+28+31+30+31+30+31+31+30+31+30;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(expMonth == 1){
        expMonth = 0;
    }
    else{
        if(expMonth==2){
            expMonth=31;
        }
        else{
            if(expMonth==3){
                expMonth=31+28;
            }
            else{
                if(expMonth==4){
                    expMonth=31+28+31;
                }
                else{
                    if(expMonth==5){
                        expMonth=31+28+31+30;
                    }
                    else{
                        if(expMonth==6){
                            expMonth=31+28+31+30+31;
                        }
                        else{
                            if(expMonth==7){
                                expMonth=31+28+31+30+31+30;
                            }
                            else{
                                if(expMonth==8){
                                    expMonth=31+28+31+30+31+30+31;
                                }
                                else{
                                    if(expMonth==9){
                                        expMonth=31+28+31+30+31+30+31+31;
                                    }
                                    else{
                                        if(expMonth==10){
                                            expMonth=31+28+31+30+31+30+31+31+30;
                                        }
                                        else{
                                            if(expMonth==11){
                                                expMonth=31+28+31+30+31+30+31+31+30+31;
                                            }
                                            else{
                                                expMonth=31+28+31+30+31+30+31+31+30+31+30;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    totalInitDays = initYear+initMonth+initDay;

    totalExpDays = expYear+expMonth+expDay;

    //total number of rental is number of the day that guitar was rented minus the number of the day guitar was delivered
    total = totalExpDays-totalInitDays;

    return total;
}

void dayOfDelivery(int *initDay, int *initMonth, int *initYear, int *expDay, int *expMonth, int *expYear){
    //if rented in a month with 30 days
    if((*initMonth)==4 || (*initMonth)==6 || (*initMonth)==9 || (*initMonth)==11){
        //if rented with more than 7 days to the end of the month
        if(*initDay<(31-MAXRENTALDAYS)){
            (*expDay)=(*initDay)+MAXRENTALDAYS; //delivery day = day of rent + max days of renting
            (*expMonth)=(*initMonth); //month will be the same
            (*expYear)=(*initYear); //year will be the same
        }
        else{
            (*expDay)= 30-(*initDay);
            (*expDay)=MAXRENTALDAYS-(*expDay);
            (*expMonth)=(*initMonth)+1;
            (*expYear)=(*initYear);
        }

    }
    //if its February - always counting as 28 days
    else{
        if((*initMonth)==2){
            if((*initDay)<29 - MAXRENTALDAYS){
                (*expDay)=(*initDay)+MAXRENTALDAYS;
                (*expMonth)=(*initMonth);
                (*expYear)=(*initYear);
        }
        else{
            (*expDay)=28-(*initDay);
            (*expDay)=MAXRENTALDAYS-(*expDay);
            (*expMonth)=(*initMonth)+1;
            (*expYear)=(*initYear);
            }
        }
        //if its not february or a 30 days month
        else{
            if((*initMonth)==12){
                if((*initDay)<25){
                    (*expDay)=(*initDay)+MAXRENTALDAYS;
                    (*expMonth)=(*initMonth);
                    (*expYear)=(*initYear);
                }
                else{
                    (*expDay)=31-(*initDay);
                    (*expDay)=MAXRENTALDAYS-(*expDay);
                    (*expMonth)=1;
                    (*expYear)=(*initYear)+1;
                }
            }
            else{
                if((*initDay)<25){
                    (*expDay)=(*initDay)+MAXRENTALDAYS;
                    (*expMonth)=(*initMonth);
                    (*expYear)=(*initYear);
                }
                    else{
                        (*expDay)=31-(*initDay);
                        (*expDay)=MAXRENTALDAYS-(*expDay);
                        (*expMonth)=(*initMonth)+1;
                        (*expYear)=(*initYear);
                    }
                }
            }
        }
}

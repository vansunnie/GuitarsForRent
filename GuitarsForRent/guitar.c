#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "storeStruct.h"

//function to check if guitar already exists, returns pos of guitar if it exists, or -1 if does not exist
int checkGuitarExists(int id, pgui vec, int vecSize){
    for(int i = 0; i < (vecSize); i++)
        if(vec[i].ID == id){
            return i;
        }
    return -1;
}

//function to add a guitar to text file of guitars
pgui addGuitar(pgui vec, int *vecSize, int id, int dailyPrice, int totalPrice, char name[SIZE]){

    pgui nvec = NULL;

    //reallocate memory for the new guitar
    nvec = realloc(vec, sizeof(guitars)*((*vecSize)+1));
    if(nvec == NULL){
        printf("\nError reallocating memory");
        exit(0);
    }

    vec = nvec;

    vec[*vecSize].ID = id;
    vec[*vecSize].dailyPrice = dailyPrice;
    vec[*vecSize].value = totalPrice;
    vec[*vecSize].stateGuitar = 0;
    strcpy(vec[*vecSize].nameGuitar, name);

    (*vecSize)++;

    return vec;
}

//function to show all the guitars
void showGuitars(pgui vec, int vecSize){

    if(!vec || vecSize <= 0){
        printf("\nNo registered guitars at the moment!");
    }else{
        for(int i = 0; i<vecSize; i++){
            printf("\nGuitar ID %d:\n",vec[i].ID);
            printf("\tName: %s\n", vec[i].nameGuitar);
            printf("\tState: %d\n", vec[i].stateGuitar);
            printf("\tDaily Price: %d\n", vec[i].dailyPrice);
            printf("\tPrice: %d\n", vec[i].value);
        }
    }

}

//function to show which guitars are rented
void showRentedGui(pgui vec, pcli listCli, int vecSize){
    int flag = 0; //control variable to check if there's any rental active
    pgui nvec = vec;
    pcli tempCli = NULL;
    prent tempRent = NULL;

    //go through vector of guitars
    for(int i = 0; i < vecSize; i++){
        //check if there's any guitar rented (state = 1)
        if(nvec[i].stateGuitar == 1){
            flag++;
            int id = nvec[i].ID;
            //go through list of clients
            for(tempCli = listCli; tempCli; tempCli = tempCli->next){
                //go through list of rentals of that client
                for(tempRent = tempCli->start; tempRent; tempRent = tempRent->next){
                    //check if there's a guitar with the same ID and if rental is on going
                    if(tempRent->IDGuitar == id && tempRent->rentalState == 0){
                        printf("\n\tGuitar ID: %d", id);
                        printf("\n\tGuitar Name: %s", nvec[i].nameGuitar);
                        printf("\n\tDaily Price: %d", nvec[i].dailyPrice);
                        printf("\n\tPRice: %d", nvec[i].value);
                        printf("\n\tClient Renting the Guitar: %s with ID %d\n\n", &tempCli->nameClient, &tempCli->ID);
                    }
                }
            }
        }
    }

    if(flag == 0)
        printf("\nThere's no active rentals at the moment!\n");
}

void showGuitarHistory(pgui vec, pcli listCli, int vecSize, int guitID){
    int flag = 0;
    pcli tempClient = NULL;
    prent tempRental = NULL;

    if(!vec){
        printf("\nNo guitars registered!\n\n");
    } else{
        for(int i = 0; i < vecSize; i++){
            if(vec[i].ID == guitID){
                for(tempClient = listCli; tempClient; tempClient = tempClient->next){
                    for(tempRental = tempClient->start; tempRental; tempRental=tempRental->next){
                        //if finds the guitar in the rental list and it was delivered already
                        if(tempRental->IDGuitar == guitID && tempRental->rentalState != 0){
                            flag++;
                            printf("\n\tClient name: %s", &tempClient->nameClient);
                            printf("\n\tRental day: %d/%d/%d", tempRental->initialDate.day, tempRental->initialDate.month, tempRental->initialDate.year);
                            printf("\n\tDelivery day: %d/%d/%d", tempRental->deliveryDate.day, tempRental->deliveryDate.month, tempRental->deliveryDate.year);
                            int daysLate = daysOfRental(tempRental->initialDate.day, tempRental->initialDate.month, tempRental->initialDate.year, tempRental->deliveryDate.day, tempRental->deliveryDate.month, tempRental->deliveryDate.year);
                            if( daysLate > MAXRENTALDAYS)
                                printf("\n\tDelivered %d days late!\n\n", daysLate - MAXRENTALDAYS);
                            else
                                printf("\n\tDelivered without delay!\n\n");
                        }
                    }
                }
                if(flag == 0)
                    printf("\nNo history for that guitar!\n\n");
            }
        }
    }
}

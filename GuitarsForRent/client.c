#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "storeStruct.h"
#include "utils.h"

bool checkClientExists(pcli clientsList, int id)
{

    while(clientsList != NULL)
    {
        if(id == clientsList->ID)
            return true;

        clientsList = clientsList->next;
    }

    return false;
}

//TODO
bool checkClientBlacklist(char* file, int id)
{
    FILE *f = fopen(file, "rb");
    blacklist bannedCli;

    if(!f){
        printf("Error opening file %s", file);
        exit(0);
    }

    while(fread(&bannedCli, sizeof(blacklist), 1, f)==1){
        if (id==bannedCli.ID){
            return true;
        }
    }
    return false;
}

pcli addClient(pcli startClient, int id, char name[SIZE])
{
    pcli currentClient = NULL;

    pcli newClientList = malloc(sizeof(clients));
    if(newClientList == NULL)
    {
        printf("\nError allocating memory");
        exit(0);
    }

    newClientList->ID = id;
    newClientList->numbRentals = 0;
    strcpy(newClientList->nameClient, name);

    newClientList->next = NULL; //next client NULL
    newClientList->start = NULL; //rentals NULL

    //if list of clients is empty
    if(startClient == NULL)
    {
        startClient = newClientList;
    }
    else
    {
        currentClient = startClient;
        while(currentClient->next != NULL)
            currentClient = currentClient->next;
        currentClient->next = newClientList;
    }

    return startClient;
}

pcli removeClient(pcli startClient, int cliID, int vecSize, pgui vec)
{
    pcli tempClient = startClient, pClient = NULL; //tempClient points to startClient, pClient goes behind tempClient
    prent tempRent = NULL;
    pgui tempGui = vec;

    if(startClient == NULL)
    {
        printf("\nThere's no registered clients\n");
    }
    else
    {
        while(tempClient != NULL && cliID != tempClient->ID)
        {
            pClient = tempClient;
            tempClient = tempClient->next;
        }

        //if can't find inserted ID
        if(tempClient == NULL)
        {
            printf("\nID %d does not exist!\n", cliID);
        }
        else
        {
            printf("\nClient %s with ID %d was deleted successfully!\n\n", tempClient->nameClient, tempClient->ID);

            //all guitars that client have will be delivered
            for(tempRent = tempClient->start; tempRent; tempRent = tempRent->next)
            {
                if(tempRent->rentalState == 0)  //if rental is on going
                {
                    for(int i = 0; i < vecSize; i++)
                    {
                        if(tempRent->IDGuitar == tempGui[i].ID)
                            tempGui[i].stateGuitar = 0;
                    }
                }
            }

            if(pClient == NULL)  //if previous is NULL - list is empty
            {
                startClient = tempClient->next;
            }
            else
            {
                pClient->next = tempClient->next;
            }

            while(tempClient->start != NULL)
            {
                tempRent = tempClient->start->next;
                free(tempClient->start);
                tempClient->start = tempRent;
            }

            free(tempRent);
            free(tempClient);
        }
    }

    return startClient;
}

void showClients(pcli listClient)
{
    pcli currentClient;

    if(!listClient)
    {
        printf("\nThere's no clients registered!\n");
    }
    else
    {
        for(currentClient = listClient; currentClient; currentClient = currentClient->next)
            printf("\nID: %d\tTotal number of rentals: %d\tName: %s\n\n", currentClient->ID, currentClient->numbRentals, currentClient->nameClient);
    }
}


void showBlacklist(char*file)
{
    FILE *f = fopen(file, "rb");
    blacklist bannedClients;

    if(!f)
    {
        printf("Error opening file %s\n\n", file);
    }
    else
    {
        if(fread(&bannedClients, sizeof(blacklist), 1, f) != 1)
            printf("\nThere's no clients in the blacklist!\n\n");
        else
        {
            while(fread(&bannedClients, sizeof(blacklist), 1, f) == 1)
                printf("\nClient %s with ID %d is banned for %s\n\n", &bannedClients.name, bannedClients.ID, &bannedClients.reason);
        }
    }

    fclose(f);
}

void showClientState(pcli listClient, int clientID)
{
    pcli tempClient = NULL;
    prent tempRental = NULL;
    int guitOnwned = 0, totalRented = 0, guitDamaged = 0, guitDelay = 0;
    int initDay, initMonth, initYear, expDay, expMonth, expYear;
    if(!listClient)
    {
        printf("\nThere's no clients registered\n\n");
    }
    else
    {
        for(tempClient = listClient; tempClient; tempClient = tempClient->next)
        {
            if(clientID == tempClient->ID)
            {
                for(tempRental = tempClient->start; tempRental; tempRental = tempRental->next)
                {
                    if(tempRental->rentalState == 0)  //if rental is on going
                    {
                        guitOnwned++;
                        totalRented++;
                    }
                    if(tempRental->rentalState == 1)  //if guitar was delivered
                    {
                        totalRented++;
                        initDay = tempRental->initialDate.day;
                        initMonth = tempRental->initialDate.month;
                        initYear = tempRental->initialDate.year;
                        expDay = tempRental->deliveryDate.day;
                        expMonth = tempRental->deliveryDate.month;
                        expYear = tempRental->deliveryDate.year;
                        if(daysOfRental(initDay, initMonth, initYear, expDay, expMonth, expYear) > MAXRENTALDAYS)
                            guitDelay++;

                    }
                    if(tempRental->rentalState == 2)  //if guitar was delivered damaged
                    {
                        guitDamaged++;
                        totalRented++;
                        initDay = tempRental->initialDate.day;
                        initMonth = tempRental->initialDate.month;
                        initYear = tempRental->initialDate.year;
                        expDay = tempRental->deliveryDate.day;
                        expMonth = tempRental->deliveryDate.month;
                        expYear = tempRental->deliveryDate.year;
                        if(daysOfRental(initDay, initMonth, initYear, expDay, expMonth, expYear) > MAXRENTALDAYS)
                            guitDelay++;
                    }
                }
                printf("\nClient %s with ID %d has %d guitar(s), already rented %d in total, already delivered %d guitar(s) with delay, which %d delivered damaged\n\n",
                       &tempClient->nameClient, clientID, guitOnwned, totalRented, guitDelay, guitDamaged);
            }
        }
    }
}



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "menu.h"
#include "storeStruct.h"
#include "guitar.h"
#include "utils.h"
#include "client.h"


int checkDigit(char a, int min, int max){
    if (isdigit(a) == 0){
        fflush(stdin);
        return 0;
    }
    else{
        int d = a - '0';
        if(d < min || d > max){
            fflush(stdin);
            return 0;
        }
        else {
            return d;
        }
    }
}

void run() {
	char input = ' ';
	int option, option1; //option1 is used only for the main menu
    pgui vec = NULL; //to create the vector of guitars
    int guiTotalNumber = 0; //total number of guitars in the store
    pcli listCli = NULL; //to create the list of clients
    int cliID;
    vec = readGuiFile(guiTextFile, &guiTotalNumber);
    listCli = readCliFile(cliTextFile, listCli);

	do {
        showMenu1();
        scanf(" %c", &input);
        option1 = checkDigit(input, 1, 4);
        switch (option1) {
            case 1:
                do {
                    putchar('\n');
                    showMenu11();
                    putchar('\n');
                    scanf(" %c", &input);
                    option = checkDigit(input, 1, 5);

                    switch (option) {
                        case 1:{
                            int id, dailyPrice, totalPrice;
                            char name[SIZE];
                            printf("\nInsert guitar ID: ");
                            scanf("%d", &id);

                            if(checkGuitarExists(id, vec, guiTotalNumber) == -1){
                                printf("\nInsert daily price of rental: ");
                                scanf("%d", &dailyPrice);

                                printf("\nInsert price of guitar: ");
                                scanf("%d", &totalPrice);

                                printf("\nInsert guitar name: ");
                                scanf(" %49[^\n]", name);

                                vec = addGuitar(vec, &guiTotalNumber, id, dailyPrice, totalPrice, name);
                            } else{
                                printf("The inserted guitar ID already exists!");
                            }
                        }
                            break;

                        case 2: showGuitars(vec, guiTotalNumber);
                            break;

                        case 3: showRentedGui(vec, listCli, guiTotalNumber);
                            break;

                        case 4:{
                            int guitID;
                            printf("\nInsert ID of guitar: ");
                            scanf("%d", &guitID);
                            if(checkGuitarExists(guitID, vec, guiTotalNumber) != -1){
                                showGuitarHistory(vec, listCli, guiTotalNumber, guitID);
                            } else{
                                printf("\nThe inserted guitar ID does not exist!\n\n");
                            }
                            break;
                        }
                    }

                } while (option != 5);
                break;

            case 2:
                do {
                    showMenu21();
                    scanf(" %c", &input);
                    option = checkDigit(input, 1, 6);
                    switch (option) {

                        case 1:{
                            char cliName[SIZE];

                            printf("\nInsert ID of new client: ");
                            scanf("%d", &cliID);
                            if(checkClientBlacklist(binFile, cliID)){
                                printf("\nThis client is in the blacklist. Can't be added!\n\n");
                                break;
                            }
                            if(!checkClientExists(listCli, cliID)){
                                printf("\nInsert the name of new client: ");
                                scanf(" %49[^\n]", cliName);

                                listCli = addClient(listCli, cliID, cliName);
                            } else{
                                printf("\nThe inserted client ID already exist!\n");
                            }
                        }
                            break;
                        case 2:{
                            printf("\nInsert ID of client you want to delete: ");
                            scanf("%d", &cliID);

                            listCli = removeClient(listCli, cliID, guiTotalNumber, vec);
                        }
                            break;
                        case 3:{
                            printf("\nInsert ID of client you want to see: ");
                            scanf("%d", &cliID);
                            if(checkClientBlacklist(binFile, cliID)){
                                printf("\nThis client is in the blacklist.\n\n");
                                break;
                            }
                            if(!checkClientExists(listCli, cliID)){
                                printf("\nThis client does not exist!\n\n");
                                break;
                            }

                            showClientState(listCli, cliID);

                        }
                            break;
                        case 4: showClients(listCli);
                            break;
                        case 5: showBlacklist(binFile);
                            break;
                    }
                } while (option != 6);
                break;

            case 3:
                do {
                    showMenu31();
                    scanf(" %c", &input);
                    option = checkDigit(input, 1, 4);
                    switch (option) {

                        case 1:{
                            int guiID = 0;
                            int d = 0, m = 0, y = 0;

                            printf("\nInsert ID of client: ");
                            scanf("%d", &cliID);
                            if(!checkClientExists(listCli, cliID)){
                                printf("\nThe inserted client ID does not exist!\n\n");
                                break;
                            }

                            printf("\nInsert Id of Guitar to rent: ");
                            scanf("%d", &guiID);
                            int guitPos = checkGuitarExists(guiID, vec, guiTotalNumber);
                            if(guitPos == -1){
                                printf("\nThe inserted guitar ID does not exist!");
                                break;
                            }

                            printf("\nInsert current day (dd)");
                            scanf("%d", &d);
                            printf("\nInsert current month (mm)");
                            scanf("%d", &m);
                            printf("\nInsert current year (yyyy)");
                            scanf("%d", &y);

                            listCli = addRental(listCli, vec, guiTotalNumber, guitPos, cliID, d, m, y);
                        }
                            break;

                        case 2:{
                            int guiID = 0;
                            int d = 0, m = 0, y = 0;

                            printf("\nInsert ID of client: ");
                            scanf("%d", &cliID);
                            if(!checkClientExists(listCli, cliID)){
                                printf("\nThe inserted client ID does not exist!\n\n");
                                break;
                            }

                            printf("\nInsert Id of Guitar to deliver: ");
                            scanf("%d", &guiID);
                            int guitPos = checkGuitarExists(guiID, vec, guiTotalNumber);
                            if(guitPos == -1){
                                printf("\nThe inserted guitar ID does not exist!");
                                break;
                            }
                            if(!checkRentedByClient(listCli, cliID, vec, guitPos)){
                                printf("\nThe inserted guitar is not beging rented by this client at the moment!\n\n");
                                break;
                            }

                            printf("\nInsert current day (dd)");
                            scanf("%d", &d);
                            printf("\nInsert current month (mm)");
                            scanf("%d", &m);
                            printf("\nInsert current year (yyyy)");
                            scanf("%d", &y);

                            int state = 0;
                            do{
                            printf("Insert the state of the guitar ( 0 - no damage, 1 - damage)");
                            scanf("%d", &state);
                            } while(state != 0 && state != 1);

                            listCli = finishRental(listCli, vec, guiTotalNumber, cliID, guitPos, state, d, m, y);
                        }
                            break;

                        case 3:{
                            int d = 0, m = 0, y = 0;
                            printf("\nInsert current day (dd)");
                            scanf("%d", &d);
                            printf("\nInsert current month (mm)");
                            scanf("%d", &m);
                            printf("\nInsert current year (yyyy)");
                            scanf("%d", &y);
                            showRentedGuitars(listCli, vec, guiTotalNumber, d, m, y);
                        }

                            break;
                    }
                } while (option != 4);
                break;

            case 4:
                saveListToFile(cliTextFile, listCli);
                deleteListClients(listCli);
                saveVecToFile(guiTextFile, vec, guiTotalNumber);
                break;
        }

    } while (option1 != 4);
}

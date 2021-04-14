#ifndef STORESTRUCT_H
#define STORESTRUCT_H

#define SIZE 50
//#define currentYear 2021
#define MAXRENTALDAYS 7        //max number of days that clients can rent a guitar
#define MAXRENTALS 5      //max number of rentals 1 client can have
#define MAXDAMAGE 3           //max number of guitars that a client can deliver damaged
#define MAXDELIVERYDAYS 20   //max number of delay days to deliver a guitar
#define DELAYPENALTY 10      //penalty to pay daily if client deliver guitar late
#define EXPENSIVEGUIT 500        //value of guitars considered expensive (clients can rent it if they rent a min number of cheap guitars before (numberCheapGuit))
#define NUMBERCHEAPGUIT 6         //min number o guitars that clients have to rent to be able to rent an expensive one after
#define binFile "blacklist.dat"    //binary file of banned clients
#define guiTextFile "guitars.txt"  //text file with guitars
#define cliTextFile "clients.txt"   //text file with clients


typedef struct date{
    int day, month, year;
}dates;

typedef struct guitar{
    int ID;     //id number
    char nameGuitar[SIZE];  //guitar name
    int dailyPrice;     //daily guitar rental price
    int value;  //guitar value
    int stateGuitar; //guitar state (0 - available; 1 - rented; 2 - damaged)
}guitars, *pgui;

typedef struct rental{
    //struct guitar IDGuitar; //ID of rented guitar
    int IDGuitar; //ID of rented guitar
    int rentalState; //0 - on going; 1 - delivered; 2 - delivered with damage
    struct date initialDate; //initial date of rental
    struct date deliveryDate; //date when guitar was delivered
    struct rental *next; //pointer to the next element in the list
}rentals, *prent;

typedef struct client{
    char nameClient[SIZE]; //client name
    int numbRentals; //number of rentals
    int ID; //client ID
    struct client *next; //pointer to the next element in the list
    prent start; //rental list
}clients, *pcli;

typedef struct banned{
    int ID; //id of banned client
    char name[SIZE]; // name of banned client
    char reason[SIZE]; // reason1 - damaged guitar; reason2 - guitars with delay
}blacklist;

#endif /* STORESTRUCT_H */

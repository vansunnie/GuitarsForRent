#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "menu.h"


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
        else
            return d;
    }
}

void run() {
	char input = ' ';
	int option;

	do{
        showMenu1();
        printf(">");
        scanf("%c", &input);
        option = checkDigit(input, 1, 4);
        switch(option){
            case 1:
                showMenu11();
                break;
            case 2:
                showMenu21();
                break;
            case 3:
                showMenu31();
                break;
            case 4:
                break;
		}
	} while (option == 0);

}

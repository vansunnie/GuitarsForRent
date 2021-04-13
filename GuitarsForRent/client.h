#ifndef CLIENT_H
#define CLIENT_H

bool checkClientExists(pcli clientsList, int id);
bool checkClientBlacklist(char* file, int id);
pcli addClient(pcli startClient, int id, char name[SIZE]);
pcli removeClient(pcli startClient, int cliID, int vecSize, pgui vec);
void showClientState(pcli listClient, int clientID);
void showClients(pcli listClient);
void showBlacklist(char *file);
int readBlacklist(char *file, int ID);

#endif /* CLIENT_H */


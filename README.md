# Guitars For Rent
Practical assignment for the Programming class (2017/2018) using CodeBlocks(IDE) and C language.

This project contains a text user interface.

## Introduction:

A store that rents guitars, “Guitars For Rent”, wants an application to manage their business. The rental of guitars can only be made by registered customers. After registration, each costumer can rent up to 5 guitars at a time (although store owners may change this value in the future). Each of the guitars available for rent has a specific value. Customers can only start renting expensive guitars (guitars worth more than 500€) when they have already successfully completed the rental of 6 cheap guitars. 

Each guitar can be rented for periods of up to one week (7 days), subject to restriction never having more than 5 guitars in their possession. At the time of return, each guitar is inspected. If it is damaged, a fine equal to the value of the guitar is charged. If a customer damages more than 3 guitars, membership is revoked, and this customer is blacklisted. From this point onwards, that customer cannot rent more guitars or become a member again. 
If a customer does not return a guitar in time, a 10€ fine will be charged for each day of delay. If a customer accumulates more than 20 days late delivery of guitars, their membership status is also revoked and blacklisted. 

The store wants to keep a complete record of guitars and customers. It also intends to maintain an updated list of customers that have been banned for breaching the rules. 


### Menu:
In the main menu the user can choose 1 out of 4 options, the first 3 will redirect the user to submenus, and the last one is to exit the program.

<img src="https://user-images.githubusercontent.com/45792504/114725779-14a73380-9d3d-11eb-8520-a95b06fb14fb.PNG" width="200"  />  

### Submenus:


Guitars options:

<img src="https://user-images.githubusercontent.com/45792504/114726098-589a3880-9d3d-11eb-92a5-a5256fedcc8a.png" width="300"  />  

- Option 1: User can add a new guitar to the stock
- Option 2: User can list all guitars in the store
- Option 3: User can see all guitars that are being rented
- Option 4: Show history of a guitar (it previous rentals)
- Option 5: Return to main menu


Clients options:

<img src="https://user-images.githubusercontent.com/45792504/114726151-664fbe00-9d3d-11eb-868f-bffb9c24a96d.png" width="300"  />  

- Option 1: User can add a new client
- Option 2: User can remove a client
- Option 3: See the status of a client (how mant guitars is renting, etc)
- Option 4: See all active clients
- Option 5: See all blacklisted clients
- Option 6: Return to main menu


Rentals options:

<img src="https://user-images.githubusercontent.com/45792504/114726202-736cad00-9d3d-11eb-9acf-5d0240f0e398.png" width="300"  />  

- Option 1: Create a new rental for a specific client and guitar
- Option 2: Finish a rental (when client wants to deliver a guitar)
- Option 3: See all current rentals
- Option 4: Return to main menu


More information and specifications about this assignment can be found [HERE](https://github.com/vansunnie/GuitarsForRent/blob/main/project_programming_1718.pdf).


/* iaed24 - ist1109955 - project */
/**
 * @file proj1.c
 * @author Diogo Carreira
 * @date March 2024
 * @brief Main program file for the parking management system.
 *
 * This file includes the main function and other functions for handling
 * different commands related to the parking management system. It uses
 * various data structures defined in "proj.h" and functions defined in
 * "auxiliary.h", "validation.h", and "movements.h".
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj.h"
#include "auxiliary.h"
#include "validation.h"
#include "movements.h"

/**
 * @brief Frees all allocated memory before program termination.
 *
 * @param parksTotal Array of Park structures.
 * @param parksCounter Count of parks.
 * @param head Head of the double linked list of Movements.
 * @param vehicles HashTable of vehicle movement information.
 * @param billing BillingHashTable of billing information.
 */
void command_q(Park *parksTotal, 
                int *parksCounter, 
                Movement **head, 
                HashTable *vehicles, 
                BillingHashTable *billing){

    hash_table_free(vehicles);
    bill_hash_table_free(billing);
    free_all_movements(*head);
    free_parks(parksTotal, *parksCounter);
}

/**
 * @brief Handles the 'p' command, which adds a new park or lists all parks.
 *
 * @param parksTotal Array of Park structures.
 * @param parksCounter Count of parks.
 */
void command_p(Park *parksTotal, int *parksCounter){
    char inputLine[BUFFSIZ], *namePark;

    /// Read Input 
    fgets(inputLine, sizeof(inputLine), stdin);
    namePark = get_park_name(inputLine);

    /// If a park name is provided, add a new park or list parks
    if(namePark != NULL)
        add_Park(parksTotal, namePark, inputLine, parksCounter);
    else {
        list_system_parks(parksTotal,parksCounter);
        free(namePark);
    }    
}

/**
 * @brief Handles the 'e' command, which registers a vehicle's entry into 
 * a park.
 *
 * @param parksTotal Array of Park structures.
 * @param parksCounter Count of parks.
 * @param head Head of the double linked list of Movements.
 * @param Vehicles HashTable of vehicle movement information.
 */
void command_e(Park *parksTotal,
                int *parksCounter, 
                Movement **head, 
                HashTable *Vehicles){

    char inputLine[BUFFSIZ], *namePark, *plateVehicle, command = COMMAND_E;
    char *currentPosition;

    /// Read Input 
    fgets(inputLine, sizeof(inputLine), stdin);

    currentPosition = inputLine;
    namePark = get_park_name(inputLine);
    plateVehicle = get_plate(inputLine);
    currentPosition += strlen(plateVehicle) + 1;
    Date *entryDate = get_date(currentPosition);


    /// Register entry and add to vehicles hash table if successful 
    Movement *newMovement = register_entry(parksTotal, 
                                            namePark, 
                                            plateVehicle, 
                                            entryDate,
                                            command, 
                                            parksCounter, 
                                            head, 
                                            Vehicles);
    free(entryDate);

    if(newMovement)
        hash_table_add(Vehicles, plateVehicle, newMovement);
        
    free(plateVehicle);
    free(namePark);
}

/**
 * @brief Handles the 's' command, registering a vehicle's exit from a park.
 *
 * @param parksTotal Array of Park structures.
 * @param parksCounter Count of parks.
 * @param head Head of the double linked list of Movements.
 * @param Vehicles HashTable of vehicle movement information.
 * @param billing BillingHashTable of billing information.
 */
void command_s(Park *parksTotal, int *parksCounter, Movement **head, HashTable *Vehicles, BillingHashTable *billing){

    char inputLine[BUFFSIZ], *namePark, *plateVehicle, command = COMMAND_S;
    char *currentPosition;

    /// Read Input 
    fgets(inputLine, sizeof(inputLine), stdin);

    currentPosition = inputLine;
    namePark = get_park_name(inputLine);
    plateVehicle = get_plate(inputLine);
    currentPosition += strlen(plateVehicle) + 1;
    Date *exitDate = get_date(currentPosition);

    /// Register exit and add to vehicles hash table if successful 
    Movement *entryMovement = find_entry_movement(*head, plateVehicle);
    char *parkEntry = NULL;
    if (entryMovement != NULL) {
        parkEntry = entryMovement->parkName;
    }
    Movement *exitMovement = register_exit(parksTotal, parkEntry, namePark, plateVehicle, exitDate, command, parksCounter, head, Vehicles);
    
    free(exitDate);

    if(exitMovement) {
        process_exit(parksTotal, parksCounter, namePark, entryMovement, exitMovement, billing);

        hash_table_add(Vehicles, plateVehicle, exitMovement); 
    }
    free(plateVehicle);
    free(namePark);
}

/**
 * @brief Handles the 'v' command, which prints the details of a vehicle's 
 * movements.
 *
 * @param vehicles HashTable of vehicle movements information.
 */
void command_v(HashTable *vehicles){
    char inputLine[BUFFSIZ];
    char *plateVehicle;

    /// Read Input 
    fgets(inputLine, sizeof(inputLine), stdin);
    plateVehicle = get_plate(inputLine);

    if (!handle_invalid_plate(plateVehicle)) {
        free(plateVehicle);
        return;
    } 

    /// Get vehicle movements from hash table
    Node *node = hash_table_get(vehicles, plateVehicle);

    /// Print movements for a vehicle if movements are found
    if (node == NULL) {
        printf("%s: %s%c", plateVehicle, ERROR_NO_ENTRIES_FOUND, NEW_LINE);
        free(plateVehicle);
        return;
    }

    print_movement_details(node);

    free(plateVehicle);
}

/**
 * @brief Handles the 'f' command, which shows the billing for a specific 
 * park and date.
 *
 * @param parksTotal Array of Park structures.
 * @param ParksCounter Count of parks.
 * @param billing BillingHashTable of billing information.
 * @param head Head of the double linked list of Movements.
 */
void command_f(Park *parksTotal, 
                int *ParksCounter, 
                BillingHashTable *billing, 
                Movement **head){

    char inputLine[BUFFSIZ];

    /// Read input line
    fgets(inputLine, sizeof(inputLine), stdin);

    char *namePark = get_park_name(inputLine);
    Park *park = find_park_by_name(parksTotal, *ParksCounter, namePark);

    /// If park not found, print error and return
    
    if (park == NULL) {
        free(namePark);
        return;
    }

    /// Get date to bill and last movement date
    Date *dateToBill = get_date_without_time(inputLine);
    Date dateToCheck = get_last_movement_date(*head);

    handle_billing(dateToBill, billing, namePark, dateToCheck);

    free(dateToBill);
    free(namePark);
}

/**
 * @brief Handles the 'r' command, which removes a park and its associated 
 * structures.
 *
 * @param parksTotal Array of Park structures.
 * @param ParksCounter Count of parks.
 * @param head Head of the double linked list of Movements.
 * @param vehicles HashTable of vehicle movement information.
 * @param billing BillingHashTable of billing information.
 */
void command_r(Park *parksTotal, 
                int *ParksCounter, 
                Movement **head, 
                HashTable *vehicles, 
                BillingHashTable *billing){

    char inputLine[BUFFSIZ], *namePark;

    /// Read input line
    fgets(inputLine, sizeof(inputLine), stdin);
    namePark = get_park_name(inputLine);
    Park *park = find_park_by_name(parksTotal, *ParksCounter, namePark);
    
    /// If park not found, if not, remove all associated structures
    if (park == NULL) {
        free(namePark);
        return;
    }

    remove_structures(parksTotal, 
                    ParksCounter, 
                    namePark, 
                    head, 
                    vehicles,billing);
    
    free(namePark);
}


/**
 * @brief Reads commands from the input and calls the corresponding function.
 *
 * @param parksTotal Array of Park structures.
 * @param ParksCounter Count of parks.
 * @param head Head of the double linked list of Movements.
 * @param vehicles HashTable of vehicle movement information.
 * @param billing BillingHashTable of billing information.
 * @return 0 if the 'q' command is read, 1 otherwise.
 */
int read_commands(Park *parksTotal, int *ParksCounter, Movement **head, HashTable *vehicles, BillingHashTable *billing){

    int c = getchar();
    switch (c)
    {
    case 'q':
        command_q(parksTotal,ParksCounter, head, vehicles, billing);
        return 0;
    case 'p':
        command_p(parksTotal,ParksCounter);
        return 1;
        
    case 'e':
        command_e(parksTotal, ParksCounter, head, vehicles);
        return 1;
        
    case 's':
        command_s(parksTotal,ParksCounter,head, vehicles, billing);
        return 1;
        
    case 'v':
        command_v(vehicles);
        return 1;
        
    case 'f':
        command_f(parksTotal, ParksCounter, billing, head);
        return 1;
    case 'r':
        command_r(parksTotal, ParksCounter, head, vehicles,billing);
        return 1;
         
    default:
        ///continue if another unknown command is read
        return 1;
    }
}

/**
 * @brief Initializes program structures.
 *
 * @param parksTotal Pointer to Park array.
 * @param ParksCounter Pointer to park count.
 * @param head Pointer to Movement list head.
 * @param vehicles Pointer to vehicle HashTable.
 * @param billing Pointer to billing HashTable.
 */
void initialize_program(Park **parksTotal, 
                        int *ParksCounter, 
                        Movement **head, 
                        HashTable **vehicles, 
                        BillingHashTable **billing){

    *parksTotal = malloc(PARK_MAX * sizeof(Park));
    *ParksCounter = 0;
    *head = NULL;
    *vehicles = hash_table_create(HASH_CAPACITY);
    *billing = bill_hash_table_create(HASH_CAPACITY);
}

/**
 * @brief Entry point of the program.
 *
 * This function initializes the necessary structures (parks, movements, 
 * vehicles, billing), then enters a loop where it reads and processes 
 * commands until the 'q' command is read.
 * After the loop, it frees the allocated memory and exits.
 */
int main(){
    Park *parksTotal;
    int ParksCounter;
    Movement *head;
    HashTable *vehicles;
    BillingHashTable *billing;

    initialize_program(&parksTotal, &ParksCounter, &head, &vehicles, &billing);

    while (read_commands(parksTotal, &ParksCounter, &head, vehicles, billing)){
    }
    return 0;
}

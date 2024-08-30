/**
 * @file auxiliary.c
 * @author Diogo Carreira
 * @date March 2024
 * @brief Auxiliary functions for the parking management system.
 *
 * This file includes functions that perform auxiliary tasks 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj.h"
#include "validation.h"
#include "auxiliary.h"
#include "movements.h"

/**
 * @brief Extracts the park name from the input line.
 * 
 * @param inputLine The input line from which to extract the park name.
 * 
 * @return A pointer to the extracted park name, or NULL if the input line is
 * empty or does not contain a park name.
 */
char *get_park_name(char *inputLine) {
    char parkNameBuffer[BUFFSIZ]; 
    char argumentInput[ARGUMENTS_SIZE_MAX]; 
    int bufferIndex = 0, inputIndex = 1, argumentIndex = 0;

    /// Return NULL if the input line is empty
    if (inputLine[0] == NEW_LINE || inputLine[0] == NULL_TERMINATOR) 
        return NULL;

    /// If the park name is enclosed in double quotes
    if (inputLine[inputIndex] == '"') {
        /// Copy characters until the closing quote or end of string
        while (inputLine[inputIndex + 1] != '"' && 
                inputLine[inputIndex + 1] != NULL_TERMINATOR) {

            parkNameBuffer[bufferIndex++] = inputLine[inputIndex + 1];
            inputIndex++;
        }
        parkNameBuffer[bufferIndex] = NULL_TERMINATOR;
        inputIndex += 2; 
    } 
    else { /// If the park name is not enclosed in double quotes
        sscanf(inputLine, "%s", parkNameBuffer);
        inputIndex = strlen(parkNameBuffer) + 1; 
    }

    /// Copy the rest of the input line
    while (inputLine[inputIndex] != NULL_TERMINATOR)
        argumentInput[argumentIndex++] = inputLine[inputIndex++];

    argumentInput[argumentIndex] = NULL_TERMINATOR;

    /// Allocate memory for the park name
    char *namePointer = malloc(strlen(parkNameBuffer) + 1);

    /// Return NULL if memory allocation failed
    if (namePointer == NULL)
        return NULL;

    /// Copy the park name to the allocated memory
    strcpy(namePointer, parkNameBuffer);
    /// Update the input line with the remaining arguments
    strcpy(inputLine, argumentInput);

    return namePointer;
}

/**
 * @brief Displays information about parks in the system.
 * 
 * @param parksTotal Pointer to an array of Park structures.
 * @param parksCounter Pointer to the counter variable storing the number 
 * of parks.
 */
void list_system_parks(Park *parksTotal, int *parksCounter){
    for(int i = 0; i<*parksCounter; i++){
        printf("%s %d %d",
         parksTotal[i].parkName, 
         parksTotal[i].capacity, 
         parksTotal[i].available);
        printf("%c",NEW_LINE);
    }
}

/**
 * @brief Adds a park to the total parks.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param namePark Name of the park to be added.
 * @param inputLine Input line containing park details.
 * @param parksCounter Pointer to the count of total parks.
 * 
 * @return 1 if the park is added successfully, 0 otherwise.
 */
int add_Park(Park *parksTotal, 
            char *namePark, 
            char *inputLine, 
            int *parksCounter) {

    int capacity, available;
    float preValue, afterValue, maxValue;

    int numItems = sscanf(inputLine, "%d %f %f %f", 
                        &capacity, 
                        &preValue, 
                        &afterValue, 
                        &maxValue);

    if (numItems < 4) 
        return 0;

    if (!can_add_park(parksTotal, 
                    namePark, 
                    capacity, 
                    preValue, 
                    afterValue, 
                    maxValue, 
                    *parksCounter)) 
        return 0;

    Charging charge = {preValue, afterValue, maxValue};
    available = capacity; 
    Park park = {namePark, capacity, charge, available};
    parksTotal[*parksCounter] = park;
    (*parksCounter)++; 
    return 1;
}

/**
 * @brief Removes a park from the total parks.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param ParksCounter Pointer to the count of total parks.
 * @param parkName Name of the park to be removed.
 */
void remove_park(Park *parksTotal, int *ParksCounter, char *parkName) {
    for (int i = 0; i < *ParksCounter; i++) {
        if (strcmp(parksTotal[i].parkName, parkName) == 0) {
            /// Free the parkName
            free(parksTotal[i].parkName);

            /// Move the remaining parks down in the array
            for (int j = i; j < *ParksCounter - 1; j++) {
                parksTotal[j] = parksTotal[j + 1];
            }

            /// Decrease the counter
            (*ParksCounter)--;

            /// Break the loop as we've found and removed the park
            break;
        }
    }
}

/**
 * @brief Frees the memory allocated for the parks and their names.
 * 
 * @param parks Pointer to the array of parks.
 * @param num_parks The number of parks in the array.
 */
void free_parks(Park *parks, int parksCounter) {
    /// Iterate over each park in the array
    for (int i = 0; i < parksCounter; i++) {
        // Free the park's name
        free(parks[i].parkName);
    }

    /// Free the array itself
    free(parks);
}

/**
 * @brief Formats and prints a date.
 * 
 * @param date The date to be formatted and printed.
 */
void format_date(Date date) {
    printf("%02d-%02d-%04d %02d:%02d", 
        date.day, 
        date.month, 
        date.year, 
        date.time.hour, 
        date.time.minute);
}

/**
 * @brief Extracts the plate from the input line.
 * 
 * @param inputLine The input line from which to extract the plate.
 * 
 * @return A pointer to the extracted plate.
 */
char *get_plate(char *inputLine) {
    char plate[PLATE_MAX], *validPlate; 

    /// Extract plate from input line
    sscanf(inputLine, "%s", plate);

    validPlate = malloc(strlen(plate) + 1);
    strcpy(validPlate, plate);

    return validPlate;
}

/**
 * @brief Extracts the date from the input line.
 * 
 * @param inputLine The input line from which to extract the date.
 * 
 * @return A pointer to the extracted date.
 */
Date *get_date(char *inputLine){
    Date *date;
    date = (Date*) malloc(sizeof(Date));

    /// Extract plate from input line
    sscanf(inputLine, "%d-%d-%d %d:%d", 
        &date->day, 
        &date->month, 
        &date->year, 
        &date->time.hour, 
        &date->time.minute);
    
    return date;
}

/**
 * @brief Extracts the date from the input line without considering time.
 * 
 * @param inputLine The input line from which to extract the date.
 * 
 * @return A pointer to the extracted date.
 */
Date *get_date_without_time(char *inputLine){
    Date *date;
    date = (Date*) malloc(sizeof(Date));
    if (date == NULL) {
        /// Handle malloc failure
        return NULL;
    }

    if (sscanf(inputLine, "%d-%d-%d", 
            &date->day, 
            &date->month, 
            &date->year) != 3) {

        /// If sscanf fails, set the date to a default value
        date->day = 0;
        date->month = 0;
        date->year = 0;
    }

    /// Set the time fields to zero or some default value
    date->time.hour = 0;
    date->time.minute = 0;

    return date;
}

/**
 * @brief Handles the case of an invalid vehicle plate.
 * 
 * @param plateVehicle The vehicle plate to be validated.
 * 
 * @return 1 if the plate is valid, 0 otherwise.
 */
int handle_invalid_plate(char *plateVehicle) {
    if(!(is_valid_plate(plateVehicle))){
        printf("%s: %s%c",plateVehicle, ERROR_INVALID_LICENSE_PLATE, NEW_LINE);
        return 0;
    }
    return 1;
}

/**
 * @brief Handles the case of an invalid date.
 * 
 * @param entryDate The date to be validated.
 * 
 * @return 1 if the date is valid, 0 otherwise.
 */
int handle_invalid_date(Date *entryDate) {
    if(!(is_valid_date(entryDate) && is_valid_time(entryDate->time))){
        printf("%s%c", ERROR_INVALID_DATE, NEW_LINE);
        return 0;
    }
    return 1;
}

/**
 * @brief Checks if it is possible to add a vehicle to a park.
 *
 * @param parksTotal Array of all parks.
 * @param namePark Name of the park to check availability for.
 * @param parksCounter Pointer to the total number of parks.
 * @return 1 if it is possible to add a vehicle, 0 otherwise.
 */
int check_park_availability(Park *parksTotal, 
                            char *namePark, 
                            int *parksCounter) {

    for(int i = 0; i < *parksCounter; i++){ /// Loop through all parks
        if(namePark != NULL  && strcmp(parksTotal[i].parkName, namePark) == 0){
            if(parksTotal[i].available > 0)
                return 1;
            else {
                printf("%s: %s%c",namePark, ERROR_PARKING_IS_FULL, NEW_LINE);
                return 0;
            }
        }
    }
    printf("%s: %s%c", namePark,ERROR_NO_SUCH_PARKING, NEW_LINE);
    return 0;
}

/**
 * @brief Updates the number of available parking spaces in a park.
 *
 * @param parksTotal Array of all parks.
 * @param namePark Name of the park to update availability for.
 * @param parksCounter Pointer to the total number of parks.
 * @param command The command that represents the type of movement 
 * (entry or exit).
 * @return 1 if the operation was successful, 0 otherwise.
 */
int update_park_availability(Park *parksTotal, 
                            char *namePark, 
                            int *parksCounter, 
                            char command) {

    for(int i = 0; i < *parksCounter; i++){ /// Loop through all parks
        if(namePark != NULL  && strcmp(parksTotal[i].parkName, namePark) == 0){
            if(command == COMMAND_E){ /// If the command is 'E' (entry)
                parksTotal[i].available--;
                printf("%s %d%c", 
                    parksTotal[i].parkName,parksTotal[i].available, NEW_LINE);
                return 1;
            }
            else if(command == COMMAND_S){ /// If the command is 'S' (exit)
                parksTotal[i].available++;
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Finds the last command for a given vehicle plate.
 * 
 * @param hash_table The hash table containing the movements.
 * @param plate The vehicle plate to find the last command for.
 * 
 * @return The last command for the given plate.
 */
char last_command_for_plate(HashTable *hashTable, char *plate) {
    /// Get the linked list from the hash table
    Node *node = hash_table_get(hashTable, plate);

    if (node == NULL) {
        /// Plate not found in the hash table
        return NULL_TERMINATOR;
    } else {
        /// Initialize the oldest date to the date of the first movement
        Date oldest_date = node->value->date;
        char oldest_command = node->value->command;

        /// Iterate over the list to find the command for the given plate
        Movement *current = node->value;
        while (current != NULL) {
            if (strcmp(current->plate, plate) == 0 && 
                is_previous_date_hour(oldest_date, current->date)) {

                oldest_date = current->date;
                oldest_command = current->command;
            }
            current = current->next;
        }

        return oldest_command;
    }
}

/**
 * @brief Registers an entry in the park.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param namePark Name of the park where the entry is to be registered.
 * @param plateVehicle The vehicle plate of the vehicle entering the park.
 * @param entryDate The date of entry.
 * @param command The command to be executed 
 * @param parksCounter Pointer to the count of total parks.
 * @param head Pointer to the head of the movement list.
 * @param vehicles Pointer to the hash table of vehicle movements.
 * 
 * @return A pointer to the new movement, or NULL if the entry could not 
 * be registered.
 */
Movement* register_entry(Park *parksTotal, char *namePark, char *plateVehicle, Date *entryDate, char command, int *parksCounter, Movement **head, HashTable *vehicles){

    /// Get the last command for the vehicle
    char lastCommand;  

    if(!park_name_exists(parksTotal, namePark, *parksCounter)){
        printf("%s: %s%c", namePark, ERROR_NO_SUCH_PARKING, NEW_LINE);
        return NULL;
    }

    /// Check if the park is available
    if(!check_park_availability(parksTotal, namePark, parksCounter)) 
        return NULL;

    /// Validate the vehicle plate
    if (!handle_invalid_plate(plateVehicle)) 
        return NULL;

    lastCommand = last_command_for_plate(vehicles, plateVehicle);

    /// Check if the last command was 'e' (entry)
    if (lastCommand == COMMAND_E) {
        printf("%s: %s%c",plateVehicle, ERROR_INVALID_VEHICLE_ENTRY, NEW_LINE);
        return NULL;
    }

    /// Validate the date
    if(!handle_invalid_date(entryDate)) 
        return NULL;

    /// Check if the entry date is valid
    if(!(is_valid_entry_date(*head, entryDate))){
        printf("%s%c", ERROR_INVALID_DATE, NEW_LINE);
        return NULL;
    }
    update_park_availability(parksTotal, namePark, parksCounter, command);
    /// Add the movement
    return add_movement(head, plateVehicle, namePark, *entryDate, command);
}

/**
 * @brief Finds a park by its name.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param parksCounter The total number of parks.
 * @param name The name of the park to find.
 * 
 * @return A pointer to the park if found, or NULL if no park with the 
 * given name exists.
 */
Park* find_park_by_name(Park *parksTotal, int parksCounter, char *name) {
    for (int i = 0; i < parksCounter; i++) {
        if (strcmp(parksTotal[i].parkName, name) == 0) {
            return &parksTotal[i];
        }
    }
    printf("%s: %s%c", name,ERROR_NO_SUCH_PARKING, NEW_LINE);
    return NULL;
}

/**
 * @brief Determines the number of days in a given month of a given year.
 * 
 * @param month The month to check (1-12).
 * @param year The year to check.
 * 
 * @return The number of days in the month, or -1 if the month is invalid.
 */
int daysInMonth(int month, int year) {
    switch(month) {
        case MONTH_JANUARY: 
        case MONTH_MARCH: 
        case MONTH_MAY: 
        case MONTH_JULY: 
        case MONTH_AUGUST: 
        case MONTH_OCTOBER: 
        case MONTH_DECEMBER:
            return MAX_DAY_31;
        case MONTH_APRIL: 
        case MONTH_JUNE: 
        case MONTH_SEPTEMBER: 
        case MONTH_NOVEMBER:
            return MAX_DAY_30;
        case MONTH_FEBRUARY:
            if (is_leap_year(year))
                return MAX_DAY_29;
            else
                return MAX_DAY_28;
        default:
            return -1; /// Invalid month
    }
}



/**
 * @brief Calculates the minutes from the beginning of the year 1 to the date.
 * 
 * @param date The date to calculate the total minutes for.
 * 
 * @return The total minutes from the beginning of the year 1 to the given 
 * date.
 */
int totalMinutes(Date date) {
    int total = 0;

    // Add minutes from previous years
    total+=(date.year - 1) * DAYS_PER_YEAR * HOURS_PER_DAY * MINUTES_PER_HOUR;
    total+=((date.year - 1) / 4) * HOURS_PER_DAY * MINUTES_PER_HOUR;

    // Add minutes from previous months
    for (int m = 1; m < date.month; m++)
        total += daysInMonth(m, date.year) * HOURS_PER_DAY * MINUTES_PER_HOUR;

    // Add minutes from previous days
    total += (date.day - 1) * HOURS_PER_DAY * MINUTES_PER_HOUR;

    // Add minutes from current day
    total += date.time.hour * MINUTES_PER_HOUR + date.time.minute;

    return total;
}

/**
 * @brief Calculates the difference in minutes between two dates.
 * 
 * @param start The start date.
 * @param end The end date.
 * 
 * @return The difference in minutes between the start and end dates.
 */
int calculate_minutes(Date start, Date end) {
    int startMinutes = totalMinutes(start);
    int endMinutes = totalMinutes(end);

    return endMinutes - startMinutes;
}

/**
 * Calculates the payment for a vehicle's stay in a parking lot.
 * 
 * @param entryMovement Pointer to the vehicle's entry movement.
 * @param exitMovement Pointer to the vehicle's exit movement.
 * 
 * @return The payment amount.
 */
double calculate_payment(Park *park, 
                        Movement *entryMovement, 
                        Movement *exitMovement) {

    // Calculate the duration of the stay in minutes
    int duration = calculate_minutes(entryMovement->date, exitMovement->date);
    // Calculate the number of complete days
    int days = duration / (24 * 60);
    duration -= days * 24 * 60;

    // Calculate the payment for the complete days
    double payment = days * park->charge.maxValue; // Z for each complete day

    // Calculate the payment for the remaining time
    if (duration <= 60) 
        payment += ((duration + 14) / 15) * park->charge.preValue; 
    else {
        int firstHour = 60;
        int remainingDuration = duration - firstHour;
        payment += 4 * park->charge.preValue; 
        payment += ((remainingDuration + 14) / 15) * park->charge.afterValue; 
    }

    // If the payment for the remaining time exceeds a day, set it to max
    if (payment > days * park->charge.maxValue + park->charge.maxValue) 
        payment = (days + 1) * park->charge.maxValue; 

    return payment;
}

/**
 * @brief Registers an exit from the park.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param namePark Name of the park where the exit is to be registered.
 * @param plateVehicle The vehicle plate of the vehicle exiting the park.
 * @param exitDate The date of exit.
 * @param command The command to be executed (E for entry, S for exit).
 * @param parksCounter Pointer to the count of total parks.
 * @param head Pointer to the head of the movement list.
 * @param vehicles Pointer to the hash table of vehicles.
 * 
 * @return pointer to the new movement, or NULL if the exit is not registered.
 */
Movement* register_exit(Park *parksTotal,
                        char *nameParkToCheck, 
                        char *namePark, 
                        char *plateVehicle, 
                        Date *exitDate, 
                        char command, 
                        int *parksCounter, 
                        Movement **head, 
                        HashTable *vehicles){
    char lastCommand;
    if(!park_name_exists(parksTotal, namePark, *parksCounter)){
        printf("%s: %s%c", namePark, ERROR_NO_SUCH_PARKING, NEW_LINE);
        return NULL;
    }
    
    /// Validate the vehicle plate
    if (!handle_invalid_plate(plateVehicle)) return NULL;

    /// Get the last command for the vehicle
    lastCommand = last_command_for_plate(vehicles, plateVehicle);
    /// Check if the last command was 'S' (exit)
    if (lastCommand == NULL_TERMINATOR || 
        lastCommand == command || 
        strcmp(nameParkToCheck, namePark) != 0) {

        printf("%s: %s%c",plateVehicle, ERROR_INVALID_VEHICLE_EXIT, NEW_LINE);
        return NULL; 
    }

    /// Validate the date
    if(!handle_invalid_date(exitDate)) return NULL;

    /// Check if the exit date is valid
    if(!(is_valid_entry_date(*head, exitDate))){
        printf("%s%c", ERROR_INVALID_DATE, NEW_LINE);
        return NULL;
    }

    update_park_availability(parksTotal, namePark, parksCounter, command);

    /// Add the movement
    return add_movement(head, plateVehicle, namePark, *exitDate, command);
}

/**
 * @brief "Processes park exit, calculates payment, adds to billing table."
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param parksCounter The total number of parks.
 * @param namePark The name of the park where the exit is to be processed.
 * @param entryMovement The movement record for the vehicle's entry.
 * @param exitMovement The movement record for the vehicle's exit.
 * @param billing The billing hash table to add the payment to.
 */
void process_exit(Park *parksTotal,
                 int *parksCounter, 
                 char *namePark, 
                 Movement *entryMovement, 
                 Movement *exitMovement, 
                 BillingHashTable *billing) {

    Park *park = find_park_by_name(parksTotal, *parksCounter, namePark);
    double payment = calculate_payment(park, entryMovement, exitMovement);

    bill_hash_table_add(billing,exitMovement->parkName,exitMovement,payment);

    print_movement_and_payment(entryMovement, exitMovement, payment);
}

/**
 * @brief Prints a movement record and payment.
 * 
 * @param entryMovement The entry movement record.
 * @param exitMovement The exit movement record.
 * @param payment The payment amount.
 */
void print_movement_and_payment(Movement *entryMovement, 
                                Movement *exitMovement, 
                                double payment) {

    printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n", 
            exitMovement->plate, 
            entryMovement->date.day,
            entryMovement->date.month,
            entryMovement->date.year,
            entryMovement->date.time.hour, 
            entryMovement->date.time.minute, 
            exitMovement->date.day,
            exitMovement->date.month, 
            exitMovement->date.year, 
            exitMovement->date.time.hour,
            exitMovement->date.time.minute, 
            payment);
}

/**
 * @brief Shows the daily billing for a given park.
 * 
 * @param billing The billing hash table.
 * @param namePark The name of the park to show the billing for.
 * @param dateToBill The date to show the billing for.
 */
void show_daily_billing(BillingHashTable *billing, 
                        char *namePark, 
                        Date *dateToBill){

    BillingNode *node = bill_hash_table_get(billing, namePark);
    
    while (node != NULL) { /// Iterates over the billing hashtable
        Movement *entryMovement = node->value;
        if (is_equal_dates(entryMovement->date, *dateToBill)) {
            printf("%s %02d:%02d %.2f\n", 
                entryMovement->plate, 
                entryMovement->date.time.hour, 
                entryMovement->date.time.minute, 
                node->bill);
        }
        node = node->next;
    }
}

/**
 * @brief Prints the total billing for a specific park.
 * 
 * @param billing The billing hash table.
 * @param namePark The name of the park.
 */
void show_billing(BillingHashTable *billing, char *namePark){
    BillingNode *node = bill_hash_table_get(billing, namePark);
    
    if (node == NULL) 
        return;
    
    Date currentDate = node->value->date;
    double currentTotal = 0.0;
    while (node != NULL) { /// Iterates over the billing hashtable
        if (!is_equal_dates(node->value->date, currentDate)) {
            printf("%02d-%02d-%04d %.2f\n", 
                currentDate.day, currentDate.month, currentDate.year, 
                currentTotal);

            currentDate = node->value->date;
            currentTotal = 0.0;
        }
        /// Adds the bill to the total in each node
        currentTotal += node->bill;
        node = node->next;
    }
    printf("%02d-%02d-%04d %.2f\n", 
        currentDate.day, 
        currentDate.month, 
        currentDate.year, 
        currentTotal);
}

/**
 * @brief Handles the billing for a specific date or for all dates if no 
 * specific date is provided.
 * 
 * @param dateToBill The specific date to bill, or the default date to bill 
 * all dates.
 * @param billing The billing hash table.
 * @param namePark The name of the park.
 * @param dateToCheck The date to check against the date to bill.
 */
void handle_billing(Date *dateToBill, 
                    BillingHashTable *billing, 
                    char *namePark, 
                    Date dateToCheck) {

    Date defaultDate = DEFAULT_DATE;

    /// If a specific date is provided, show daily billing for that date
    if (!is_equal_dates(defaultDate, *dateToBill)) {
        if (is_valid_date(dateToBill) && 
            is_previous_date(*dateToBill, dateToCheck)) {
            show_daily_billing(billing, namePark, dateToBill);
        } 
        else 
            printf("%s%c", ERROR_INVALID_DATE, NEW_LINE);
    } 
    /// If no specific date is provided, show total billing for all dates
    else {
        show_billing(billing, namePark);
    }
}

/**
 * @brief Sorts and prints the names of all parks.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param ParksCounter The total number of parks.
 */
void print_park_names(Park *parksTotal, int ParksCounter) {
    /// Bubble sort the parksTotal array in alphabetical order
    for (int i = 0; i < ParksCounter - 1; i++) {
        for (int j = 0; j < ParksCounter - i - 1; j++) {
            if (strcmp(parksTotal[j].parkName,parksTotal[j + 1].parkName) > 0){
                // Swap parksTotal[j] and parksTotal[j + 1]
                Park temp = parksTotal[j];
                parksTotal[j] = parksTotal[j + 1];
                parksTotal[j + 1] = temp;
            }
        }
    }

    /// Print the park names
    for (int i = 0; i < ParksCounter; i++) {
        printf("%s\n", parksTotal[i].parkName);
    }
}

/**
 * @brief Removes all structures related to a specific park.
 * 
 * @param parksTotal Pointer to the array of parks.
 * @param ParksCounter The total number of parks.
 * @param parkName The name of the park to remove.
 * @param head Pointer to the head of the movement list.
 * @param vehicles Pointer to the hash table of vehicles.
 * @param billing Pointer to the billing hash table.
 */
void remove_structures(Park *parksTotal, 
                        int *ParksCounter, 
                        char *parkName,  
                        Movement **head, 
                        HashTable *vehicles, 
                        BillingHashTable *billing) {

     hash_table_remove(vehicles, parkName); 
     bill_hash_table_remove(billing, parkName);
     remove_park(parksTotal, ParksCounter, parkName);
     remove_movements(head, parkName);
     print_park_names(parksTotal,*ParksCounter);
}
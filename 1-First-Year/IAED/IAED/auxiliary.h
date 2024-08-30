/**
 * @file auxiliary.h
 * @author Diogo Carreira
 * @date March 2024
 * @brief Auxiliary functions for the park management system.
 */

#ifndef AUXILIARY_H
#define AUXILIARY_H
#include "movements.h" 

char *get_park_name(char *inputLine);
void list_system_parks(Park *parksTotal, int *parksCounter);
void remove_park(Park *parksTotal, int *ParksCounter, char *parkName);
void free_parks(Park *parks, int parksCounter);
char *get_plate(char *inputLine);
void format_date(Date date);
Date *get_date(char *inputLine);
Date *get_date_without_time(char *inputLine);
int add_Park(Park *parksTotal, char *namePark, char *inputLine, int *parksCounter);
int handle_invalid_plate(char *plateVehicle);
int handle_invalid_date(Date *entryDate);
int check_park_availability(Park *parksTotal, char *namePark, int *parksCounter);
int update_park_availability(Park *parksTotal, char *namePark, int *parksCounter, char command);
char last_command_for_plate(HashTable *hashTable, char *plate);
Movement* register_entry(Park *parksTotal, char *namePark, char *plateVehicle, Date *entryDate, char command, int *parksCounter, Movement **head, HashTable *vehicles);
Park* find_park_by_name(Park *parksTotal, int parksCounter, char *name);
int daysInMonth(int month, int year);
int totalMinutes(Date date);
int calculate_minutes(Date start, Date end);
double calculate_payment(Park *park, Movement *entryMovement, Movement *exitMovement);
Movement* register_exit(Park *parksTotal,char *nameParkToCheck, char *namePark, char *plateVehicle, Date *exitDate, char command, int *parksCounter, Movement **head, HashTable *vehicles);
void process_exit(Park *parksTotal, int *parksCounter, char *namePark, Movement *entryMovement, Movement *exitMovement, BillingHashTable *billing);
void print_movement_and_payment(Movement *entryMovement, Movement *exitMovement, double payment);
void show_daily_billing(BillingHashTable *billing, char *namePark, Date *dateToBill);
void show_billing(BillingHashTable *billing, char *namePark);
void handle_billing(Date *dateToBill, BillingHashTable *billing, char *namePark, Date dateToCheck);
void remove_structures(Park *parksTotal, int *ParksCounter, char *parkName,  Movement **head, HashTable *vehicles, BillingHashTable *billing);
void print_park_names(Park *parksTotal, int ParksCounter);

#endif 
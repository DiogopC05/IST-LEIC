/**
 * @file validation.h
 * @author Diogo Carreira
 * @date March 2024
 * @brief Validation definitions for the park management system.
 */
#ifndef VALIDATION_H
#define VALIDATION_H

#define LEAP_YEAR_CONDITION (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
#define MIN_DAY 1
#define DAYS_IN_MONTH {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
#define MIN_MONTH 1
#define MAX_MONTH 12
#define FEBRUARY 2
#define LAST_DAY_FEBRUARY 29
#define MAX_HOUR 23
#define MAX_MINUTE 59
#include "movements.h" 


int park_name_exists(Park *parks, char *namePark, int numParks);
int is_invalid_capacity(int capacity);
int is_invalid_cost(float preValue, float afterValue, float maxValue);
int is_too_many_parks(int parksCounter);
int can_add_park(Park *parksTotal, char *namePark, int capacity, float preValue, float afterValue, float maxValue, int parksCounter);
int is_valid_plate(char *plateToCheck);
int is_equal_dates(Date d1, Date d2);
int is_previous_date(Date date1, Date date2);
int is_previous_date_hour(Date date1, Date date2);
int is_leap_year(int year);
int is_valid_date(Date *date);
int is_valid_time(Time time);
int is_valid_entry_date(Movement *head, Date *entryDate);
int includes_feb29(Date entryDate, Date exitDate);

#endif 
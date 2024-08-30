/**
 * @file validation.c
 * @author Diogo Carreira
 * @date March 2024
 * @brief Validation functions for the park management system.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj.h"
#include "validation.h"
#include "movements.h"


/**
 * @brief Checks if a park name exists in the vector of parks.
 *
 * @param parks         Vector of parks.
 * @param namePark      Pointer to the the number of parks.
 * @param numParks      Name of the park to check.
 * @return              Returns 1 if the park name exists, 0 otherwise.
 */
int park_name_exists(Park *parks, char *namePark, int numParks) {
    for (int i = 0; i < numParks; i++) {
        if (strcmp(namePark, parks[i].parkName) == 0) {
            return 1; /// Park name found 
        }
    }
    return 0; /// Park name not found 
}

/**
 * @brief Checks if a given capacity is invalid (less than or equal to zero).
 * 
 * @param capacity The capacity to check.
 * @return 1 if the capacity is invalid, 0 otherwise.
 */
int is_invalid_capacity(int capacity) {
        if(capacity <= 0)
            return 1;
    return 0; 
}

/**
 * @brief Checks if a given capacity is invalid (less than or equal to zero).
 * 
 * @param capacity The capacity to check.
 * @return 1 if the capacity is invalid, 0 otherwise.
 */
int is_invalid_cost(float preValue, float afterValue, float maxValue) {

    if (preValue <= 0 || afterValue <= 0 || maxValue <= 0) 
        return 1; /// any of the values are negative  
    
    if (preValue >= afterValue || afterValue >= maxValue) 
        return 1; /// is not increasing or exceeds the maximum value 

    return 0; /// cost is valid 
}

/**
 * @brief Checks if the number of parks has reached the maximum limit.
 * 
 * @param parksCounter The current number of parks.
 * @return 1 if the number of parks has reached the maximum limit, 
 * 0 otherwise.
 */
int is_too_many_parks(int parksCounter) {
        if(parksCounter == PARK_MAX)
            return 1;
    return 0; 
    }

/**
 * @brief Checks if the park can be added.
 * 
 * @param parksTotal The total parks.
 * @param namePark The name of the park.
 * @param capacity The capacity of the park.
 * @param preValue The pre value.
 * @param afterValue The after value.
 * @param maxValue The max value.
 * @param parksCounter The parks counter.
 * 
 * @return 1 if the park can be added, 0 otherwise.
 */
int can_add_park(Park *parksTotal, 
                char *namePark, 
                int capacity, 
                float preValue, 
                float afterValue, 
                float maxValue, 
                int parksCounter) {

    if (park_name_exists(parksTotal, namePark, parksCounter)) {
        printf("%s: %s%c", namePark, ERROR_PARKING_ALREADY_EXISTS, NEW_LINE);
        return 0;
    }

    if (is_invalid_capacity(capacity)) {
        printf("%d: %s%c", capacity, ERROR_INVALID_CAPACITY, NEW_LINE);
        return 0;
    }

    if (is_invalid_cost(preValue, afterValue, maxValue)) {
        printf("%s%c", ERROR_INVALID_COST, NEW_LINE);
        return 0;
    }

    if (is_too_many_parks(parksCounter)) {
        printf("%s%c", ERROR_TOO_MANY_PARKS, NEW_LINE);
        return 0;
    }

    return 1;
}

/**
 * @brief Checks if a given vehicle plate is valid.
 * 
 * @param plateToCheck The vehicle plate to check.
 * @return 1 if the plate is valid, 0 otherwise.
 */
int is_valid_plate(char *plateToCheck){
    char pairs[3][3];
    int digitPairCount = 0;
    int alphaPairCount = 0;
    
    if (sscanf(plateToCheck, "%2s-%2s-%2s", 
        pairs[0], pairs[1], pairs[2]) != 3) {
        /// Failed to extract three pairs 
        return 0;
    }

    for(int i = 0; i < 3; i++) {
        if(IS_UPPERCASE_LETTER(pairs[i][0])&&IS_UPPERCASE_LETTER(pairs[i][1]))
            alphaPairCount++;

        if(IS_DIGIT(pairs[i][0])&&IS_DIGIT(pairs[i][1])) 
            digitPairCount++;
    }

    if ((digitPairCount == 1 && alphaPairCount == 2) || 
        (digitPairCount == 2 && alphaPairCount == 1)) 
        return 1; /// Plate is valid 
    
    return 0; /// Plate is not valid 
}

/**
 * @brief Checks if two dates are equal.
 * 
 * @param d1 The first date.
 * @param d2 The second date.
 * @return 1 if the dates are equal, 0 otherwise.
 */
int is_equal_dates(Date d1, Date d2) {
    if (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Checks if the first date is <= to the second date.
 * 
 * @param date1 The first date.
 * @param date2 The second date.
 * @return 1 if the first date is earlier than or equal to the second date, 
 * 0 otherwise.
 */
int is_previous_date(Date date1, Date date2) {
    if (date1.year < date2.year)
        return 1;
    if (date1.year > date2.year)
        return 0;

    /* Years are equal, compare months */
    if (date1.month < date2.month)
        return 1;
    if (date1.month > date2.month)
        return 0;

    /* Months are equal, compare days */
    if (date1.day <= date2.day)
        return 1;
    if (date1.day > date2.day)
        return 0;

    /* Dates are equal */
    return 0;
}


/**
 * @brief Checks if date1 is earlier than or equal to date2, 
 * considering hours.
 * 
 * @param date1 First date.
 * @param date2 Second date.
 * @return 1 if date1 <= date2, else 0.
 */

int is_previous_date_hour(Date date1, Date date2) {
    if (date1.year < date2.year)
        return 1;
    if (date1.year > date2.year)
        return 0;

    /* Years are equal, compare months */
    if (date1.month < date2.month)
        return 1;
    if (date1.month > date2.month)
        return 0;

    /* Months are equal, compare days */
    if (date1.day < date2.day)
        return 1;
    if (date1.day > date2.day)
        return 0;

    /* Days are equal, compare hours */
    if (date1.time.hour < date2.time.hour)
        return 1;
    if (date1.time.hour > date2.time.hour)
        return 0;

    /* Hours are equal, compare minutes */
    if (date1.time.minute <= date2.time.minute)
        return 1;
    if (date1.time.minute > date2.time.minute)
        return 0;

    /* Dates are equal */
    return 0;
}

/**
 * @brief Checks if a given year is a leap year.
 *
 * @param year  year to be checked
 * @return  1 if the year is a leap year, 0 otherwise
 */
int is_leap_year(int year) {
    if (LEAP_YEAR_CONDITION) {
        return 1; /// is leap year 
    } else {
        return 0;
    }
}

/**
 * @brief Checks if a given date is valid.
 * 
 * @param date The date to check.
 * @return 1 if the date is valid, 0 otherwise.
 */
int is_valid_date(Date *date) {
    const int days_in_month[] = DAYS_IN_MONTH;
    if (date->month < MIN_MONTH || date->month > MAX_MONTH) {
        return 0; /// invalid month 
    }

    /// Verify day for month 
    if (date->day < MIN_MONTH || date->day > days_in_month[date->month - 1]) {
        /// handling february 
        if (date->month == FEBRUARY && 
            date->day == LAST_DAY_FEBRUARY && 
            is_leap_year(date->year)) {
            return 0; /// invalid month 
        }
        return 0; /// invalid month 
    }

    return 1; /// valid month 
}

/**
 * @brief Checks if a given time is valid.
 * 
 * @param time The time to check.
 * @return 1 if the time is valid, 0 otherwise.
 */
int is_valid_time(Time time) {
    
    if (time.hour < 0 || time.hour > MAX_HOUR) {
        return 0; /// invalid hour 
    }
    if (time.minute < 0 || time.minute > MAX_MINUTE) {
        return 0; /// invalid minute 
    }
    return 1; /// valid hour 
}

/**
 * @brief Checks if a given entry date is valid.
 * 
 * @param head Pointer to the head of the movement list.
 * @param entryDate The entry date to check.
 * @return 1 if the entry date is valid, 0 otherwise.
 */
int is_valid_entry_date(Movement *head, Date *entryDate){
    Date prevDate = get_last_movement_date(head);
    
    if(is_previous_date_hour(prevDate,*entryDate))
        return 1;
    return 0;
}

/**
 * @brief Checks if the period between two dates includes February 29.
 * 
 * @param entryDate The entry date.
 * @param exitDate The exit date.
 * @return 1 if the period includes February 29, 0 otherwise.
 */
int includes_feb29(Date entryDate, Date exitDate) {
    for (int year = entryDate.year; year <= exitDate.year; year++) {

        Date feb29 = {29, 2, year, {23, 59}};

        if (is_leap_year(year) && 
            is_previous_date_hour(entryDate, feb29) && 
            is_previous_date_hour(feb29, exitDate))
            return 1;   
    }
    return 0;
}



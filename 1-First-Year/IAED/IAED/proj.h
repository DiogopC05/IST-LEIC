/**
 * @file proj.h
 * @author Diogo Carreira
 * @date March 2024
 * @brief Header file for the parking management system.
 *
 * This file defines various constants, error messages, 
 * and macros used in the parking management system.
 * 
 * It also defines the structures for Time, Date, Charging, Park, and Vehicle.
 */

/// Configuration Parameters
#define PARK_MAX 20
#define PLATE_MAX 20
#define PARK_NAME_SIZE_MAX 8192
#define ARGUMENTS_SIZE_MAX 8192
#define BUFFSIZ 8192
#define HASH_CAPACITY 10067
#define HASH_INIT 5381
#define DEFAULT_DATE {0, 0, 0, {0, 0}}

/// Character Constants
#define NEW_LINE '\n'
#define NULL_TERMINATOR '\0'

/// Command Chars
#define COMMAND_E 'e'
#define COMMAND_S 's'

/// Handling Command Errors 
#define ERROR_PARKING_ALREADY_EXISTS "parking already exists."
#define ERROR_INVALID_CAPACITY "invalid capacity."
#define ERROR_INVALID_COST "invalid cost."
#define ERROR_TOO_MANY_PARKS "too many parks."
#define ERROR_NO_SUCH_PARKING "no such parking."
#define ERROR_PARKING_IS_FULL "parking is full."
#define ERROR_INVALID_LICENSE_PLATE "invalid licence plate."
#define ERROR_INVALID_VEHICLE_ENTRY "invalid vehicle entry."
#define ERROR_INVALID_DATE "invalid date."
#define ERROR_INVALID_VEHICLE_EXIT "invalid vehicle exit."
#define ERROR_NO_ENTRIES_FOUND "no entries found in any parking."

// Function to check if a character is a digit
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
// Function to check if a character is an uppercase letter
#define IS_UPPERCASE_LETTER(c) ((c) >= 'A' && (c) <= 'Z')

/// Month and Day Constants
#define MONTH_JANUARY 1
#define MONTH_FEBRUARY 2
#define MONTH_MARCH 3
#define MONTH_APRIL 4
#define MONTH_MAY 5
#define MONTH_JUNE 6
#define MONTH_JULY 7
#define MONTH_AUGUST 8
#define MONTH_SEPTEMBER 9
#define MONTH_OCTOBER 10
#define MONTH_NOVEMBER 11
#define MONTH_DECEMBER 12

#define MAX_DAY_31 31
#define MAX_DAY_30 30
#define MAX_DAY_29 29
#define MAX_DAY_28 28

#define MINUTES_PER_HOUR 60
#define HOURS_PER_DAY 24
#define DAYS_PER_YEAR 365

/**
 * @brief Structure representing a time.
 */
typedef struct{
    int hour;   ///< The hour of the time.
    int minute; ///< The minute of the time.
}Time;

/**
 * @brief Structure representing a date.
 */
typedef struct{
    int day;   ///< The day of the date.
    int month; ///< The month of the date.
    int year;  ///< The year of the date.
    Time time; ///< The time of the date.
}Date;

/**
 * @brief Structure representing charging information.
 */
typedef struct{
    float preValue;   ///< The value before charging.
    float afterValue; ///< The value after charging.
    float maxValue;   ///< The maximum value that can be charged.
}Charging;

/**
 * @brief Structure representing a park.
 */
typedef struct{
    char *parkName;   ///< The name of the park.
    int capacity;     ///< The capacity of the park.
    Charging charge;  ///< The charging information for the park.
    int available;    ///< The number of available spots in the park.
}Park;


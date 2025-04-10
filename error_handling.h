#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>

// Define different error levels (optional)
typedef enum {
    ERROR_LEVEL_ERROR,
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_NOTE
} error_level_t;

// Function to report an error with location information
void report_error(error_level_t level, int line, int column, const char *format, ...);

// Function to report a generic error without specific location
void report_generic_error(error_level_t level, const char *format, ...);

// Function to get the total number of errors encountered (optional)
int get_error_count();

// Function to get the total number of warnings encountered (optional)
int get_warning_count();

#endif // ERROR_HANDLING_H
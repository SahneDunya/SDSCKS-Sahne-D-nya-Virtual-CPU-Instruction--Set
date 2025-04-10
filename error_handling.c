#include "error_handling.h"
#include <stdarg.h>

static int error_count = 0;
static int warning_count = 0;

void report_error(error_level_t level, int line, int column, const char *format, ...) {
    va_list args;
    va_start(args, format);

    switch (level) {
        case ERROR_LEVEL_ERROR:
            fprintf(stderr, "Error at line %d, column %d: ", line, column);
            error_count++;
            break;
        case ERROR_LEVEL_WARNING:
            fprintf(stderr, "Warning at line %d, column %d: ", line, column);
            warning_count++;
            break;
        case ERROR_LEVEL_NOTE:
            fprintf(stderr, "Note at line %d, column %d: ", line, column);
            break;
        default:
            fprintf(stderr, "Message at line %d, column %d: ", line, column);
            break;
    }

    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);

    if (level == ERROR_LEVEL_ERROR) {
        // Optionally, you might want to exit compilation after a certain number of errors
        // exit(EXIT_FAILURE);
    }
}

void report_generic_error(error_level_t level, const char *format, ...) {
    va_list args;
    va_start(args, format);

    switch (level) {
        case ERROR_LEVEL_ERROR:
            fprintf(stderr, "Error: ");
            error_count++;
            break;
        case ERROR_LEVEL_WARNING:
            fprintf(stderr, "Warning: ");
            warning_count++;
            break;
        case ERROR_LEVEL_NOTE:
            fprintf(stderr, "Note: ");
            break;
        default:
            fprintf(stderr, "Message: ");
            break;
    }

    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);

    if (level == ERROR_LEVEL_ERROR) {
        // Optionally, you might want to exit compilation after a certain number of errors
        // exit(EXIT_FAILURE);
    }
}

int get_error_count() {
    return error_count;
}

int get_warning_count() {
    return warning_count;
}
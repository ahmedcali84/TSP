#ifndef LOGGING_SYSTEM_H
#define LOGGING_SYSTEM_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    INFO,
    ERROR,
    DEBUG,
    WARNING,
} Log_Levels;

#define LOGDEF static __attribute__((unused))

LOGDEF const char *levels_as_cstr(Log_Levels level) {
    switch (level) {
        case ERROR: return "[ERROR]: ";
        case DEBUG: return "[DEBUG]: ";
        case WARNING: return "[WARNING]: ";
        case INFO:
        default: return "[INFO]: ";
    }
}

LOGDEF void Logging(FILE *stream, Log_Levels level, const char *fmt, ...) {
    fprintf(stream, "%s" , levels_as_cstr(level));

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);
}

#define Log_Out(level, fmt, ...) Logging(stdout, level, fmt, ##__VA_ARGS__)
#define Log_File(stream, level, fmt, ...) Logging(stream, level, fmt, ##__VA_ARGS__)

#endif /* LOGGING_SYSTEM_H */

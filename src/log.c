#include "log.h"

void log_info(const char *info) {
    printf("[I]: %s\n", info)
}

void log_error(const char *error) {
    printf("[E]: %s\n", error)
}

void log_debug(const char *debug) {
    printf("[D]: %s\n", msg)
}
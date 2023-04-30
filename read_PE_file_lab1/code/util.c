#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void print_str(const char line[]){
    printf("%s\n", line);
}

void print_err(const char* text, const uint8_t error_code){
    fprintf(stderr, "%s: %"PRId8"\n", text, error_code);
    if (error_code != 0){
        exit(error_code);
    }
}
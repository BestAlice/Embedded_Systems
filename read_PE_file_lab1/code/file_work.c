#include "file_work.h"
#include <stdint.h>
#include <stdio.h>


enum open_status open_file(const char *file_name, FILE** file){
    *file = fopen(file_name, "rb+");
    if (!(*file)){
        return OPEN_NOT_EXICTING;
    }
    return OPEN_OK;
}

enum create_status create_file(const char* file_name, FILE** file){
    if (open_file(file_name, file) == 0){
        return CREATE_OK;
    }

    *file = fopen(file_name, "wb+");
    if (!(*file)){
        return CREATE__INVALID_DICT_ACCESS;
    }
    return CREATE_OK;
}

enum close_status close_file(FILE** file){
    if (*file == NULL) {
        return CLOSE_NULL_FILE;
    }
    if (fclose(*file) != 0){
        return CLOSE_CLOSING_ERR;
    }
    return CLOSE_OK;
}

#include <stdio.h>
#include <stdint.h>

enum open_status  {
    OPEN_OK = 0,
    OPEN_NOT_EXICTING,
    OPEN_INVALID_DICT_ACCESS
};
enum close_status  {
    CLOSE_OK = 0,
    CLOSE_CLOSING_ERR,
    CLOSE_NULL_FILE
};
enum create_status  {
    CREATE_OK = 0,
    CREATE__INVALID_DICT_ACCESS
};

enum open_status open_file(const char *file_name, FILE** file);
enum create_status create_file(const char* file_name, FILE** file);
enum close_status close_file(FILE** file);
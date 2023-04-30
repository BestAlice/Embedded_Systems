#include "file_work.h"
#include "util.h"

struct section{
    char name[8];
    int virtual_size;
    int virtual_address;
    int size_data;
    int pointer_data;
    int pointer_relocations;
    int pointer_linenumbers;
    short number_relocations;
    short number_linenumbers;
    char characteristics[4];
};

int PE_analysis(FILE* file);
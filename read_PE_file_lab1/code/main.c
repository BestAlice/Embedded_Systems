#include <stdio.h>
#include "PE_work.h"
#include "util.h"
#include <windows.h>


int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8);

    (void) argc; (void) argv;

    if (argc < 2) {
        printf("File name didn't enter");
        return 1;
    }

    char* PE_name = argv[1];// = "Miner.exe";
    //printf("введите название файла: ");
    //scanf("%s", PE_name);
    FILE* PE_file;
    print_err("OPEN exe: ", open_file(PE_name, &PE_file) );
    PE_analysis(PE_file);


    /*FILE* header;
    create_file("header", header);
    FILE* code;
    create_file("code", code);
    close_file(header);
    close_file(code);*/
    print_err("CLOSE exe: ", close_file(&PE_file));
    return 0;
}

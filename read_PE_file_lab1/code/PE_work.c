#include <stdio.h>
#include <string.h>
#include "PE_work.h"
#include "util.h"
#include "stdlib.h"

int PE_analysis(FILE* file){
    fseek(file, 60, SEEK_SET);
    int pe_header_move;
    fread(&pe_header_move, sizeof (int), 1, file);
    //printf("%d\n", pe_header_move);

    fseek(file, pe_header_move, SEEK_SET);
    char signature[4];
    fread(signature, sizeof(char), 4, file);
    //printf("%s\n", signature);
    if (strcmp(signature, "PE\000\000")) {
        printf("Файл не явялется исполняемым PE\n");
        return 1;
    }

    short num_of_sections;
    fseek(file, sizeof (short), SEEK_CUR);
    fread(&num_of_sections, sizeof (short), 1, file);
    //printf("%d\n", num_of_sections);

    int size_of_code;
    fseek(file, 20, SEEK_CUR); //Переход через много секций до DWORD SizeOfCode
    fread(&size_of_code, sizeof(int), 1, file);
    //printf("%d\n", size_of_code);

    int address_of_entry_point;
    fseek(file, 8, SEEK_CUR);
    fread(&address_of_entry_point, sizeof(int), 1, file);
    //printf("%d\n", address_of_entry_point);
    fseek(file, 76+8*16, SEEK_CUR);
    struct section* sections = malloc(num_of_sections * sizeof(struct section));
    for (int i = 0; i < num_of_sections; ++i) {
        fread(sections+i, sizeof(struct section), 1, file);
    }

    FILE* header;
    print_err("CREATE HEADER: ", create_file("header.txt", &header)) ;
    FILE* code;
    print_err("CREATE CODE: ", create_file("data", &code));

    fprintf(header, "%d\n\n", address_of_entry_point);

    for (int i = 0; i < num_of_sections; i++) {
        fprintf(header, "name %s\n", sections[i].name);
        fprintf(header, "VirtualSize %x\n", sections[i].virtual_size);
        fprintf(header, "VirtualAddress %x\n", sections[i].virtual_address);
        fprintf(header, "SizeOfRawData %x\n", sections[i].size_data);
        fprintf(header, "PointerToRawData %x\n", sections[i].pointer_data);
        fprintf(header, "PointerToRelocations %x\n", sections[i].pointer_relocations);
        fprintf(header, "PointerToLinenumbers %x\n", sections[i].pointer_linenumbers);
        fprintf(header, "NumberOfRelocations %x\n", sections[i].number_relocations);
        fprintf(header, "NumberOfLinenumbers %x\n", sections[i].number_linenumbers);
        fprintf(header, "Characteristics ");
        for (int t = 3; t >= 0; t--)
            fprintf(header, "%.2hhx", sections[i].characteristics[t]);
        fprintf(header, "\n\n");
        if ( sections[i].characteristics[0]  == 0x20){
            fseek(file, sections[i].virtual_address, SEEK_SET);
            char symbol;
            for (int j = 0; j < sections[i].size_data; ++j) {
                fread(&symbol, sizeof (char), 1, file);
                fwrite(&symbol, sizeof(char), 1, code);
            }
        }
    }

    print_err("CLOSE HEADER: ", close_file(&header));
    print_err("CREATE CODE: ",close_file(&code));


    return 0;
}

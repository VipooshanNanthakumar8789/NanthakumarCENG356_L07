/*
 ============================================================================
 Name        : Lab8.c
 Author      : Austin Tian
 Revised by  :
 Version     :
 Copyright   : Copyright 2020
 Description : Lab 8 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"

// Menu for the testing.
char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " *    This is the memory operation menu (Lab 8)                     *\n" \
                " ********************************************************************\n" \
                " *    1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " *    2. Read a byte (8-bit) data from the memory                   *\n" \
                " *    3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " *    4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator()
{
    return rand()%255;  // generate a random number between 0 and 255.
}
//---------------------------------------------------------------
void free_memory(char *base_address)
{
    free(base_address);  // free memory after use to avoid memory leakage.
    return;
}
//---------------------------------------------------------------
char *init_memory()
{
    char *mem = malloc(MEM_SIZE);  // allocate the memory
    int i = 0;

    for (i = 0; i < MEM_SIZE; i++){
        *(mem + i) = 0;
    }
    return mem;
}

//---------------------------------------------------------------
void write_byte(const char *base_address, const int offset, const unsigned char byte_data){
    *(unsigned char *)(base_address + offset) = byte_data;
}

//---------------------------------------------------------------
void write_dword(const char *base_address, const int offset, const unsigned int dword_data){
    unsigned int *data = (unsigned int *)(base_address + offset);
    *data = dword_data;
}
//---------------------------------------------------------------
unsigned char read_byte(const char *base_address, int offset){
    unsigned char data;
    data = *(const unsigned char *)(base_address + offset);
    if (DEBUG_CODE)
        printf("-----The Byte at base: %p, offset %X: %02X ------\n", (void *)base_address, offset, data);
    return data;
}
//---------------------------------------------------------------
unsigned int read_dword(const char *base_address, int offset){
    const unsigned int *dataptr = (const unsigned int *)(base_address + offset);
    if (DEBUG_CODE)
        printf("-----The 32-bit word at base: %p, offset %X is: %08X ------\n", (void *)base_address, offset, *dataptr);
    return *dataptr;
}
//---------------------------------------------------------------
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize){
    unsigned char printout;
    int i = 0, j = 0;

    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;

    printf("\n");

    for (i = 0; i < dumpsize / DUMP_LINE; i++)
    {
        printf("%p: ", (void *)(base_address + offset + i * DUMP_LINE));
        for (j = 0; j < DUMP_LINE; j++)
        {
            printout = (unsigned char)(*(base_address + offset + i * DUMP_LINE + j));
            printf("%02X ", printout);
        }
        printf(" --- ");
        for (j = 0; j < DUMP_LINE; j++)
        {
            printout = (unsigned char)(*(base_address + offset + i * DUMP_LINE + j));
            if (printout > 0x20 && printout < 0x7E)
                printf("%c ", printout);
            else
                printf(". ");
        }
        printf("\n");
    }
    getchar();
    return;
}

/*
void clean_stdin(void)
{
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}*/

//---------------------------------------------------------------
void setup_memory()
{
    char *mem = init_memory();  // initialize the memory.
    char options = 0;
    unsigned int offset, dumpsize;
    char byte_data;
    unsigned int dword_data;

    do{
        if (options != 0x0a)
        {
            puts(menu);
            printf("\nThe base address of your memory is: %p (HEX)\n", (void *)mem);
            puts("Please make a selection:");
        }
        options = getchar();

        switch (options)
        {
            case '1':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", (unsigned int*)&offset);
                puts("Please input your DOUBLE WORD data to be written (in HEX):");
                scanf("%x", (unsigned int*)&dword_data);
                write_dword(mem, offset, dword_data);
                continue;
            case '2':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_byte(mem, offset);
                continue;
            case '3':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_dword(mem, offset);
                continue;
            case '4':
                puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                scanf("%x", &offset);
                puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                scanf("%d", &dumpsize);
                memory_dump(mem, offset, dumpsize);
                continue;
            case 'e':
            case 'E':
                puts("Code finished, exit now");
                free_memory(mem);
                getchar();
                return;
            default:
                continue;
        }
    }while (1);
    return;
}
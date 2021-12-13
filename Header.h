#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#define LINE_SIZE 80
#define MACHINE_CODE_SIZE 32
#define FILE_NAME_SIZE 30
#define COMMAND_SIZE 15
#define LABEL_SIZE 15
#define OFF -1
#define ON 1


/*STRUCTURES*/

/*bitfield structs*/
/*each bitfield fits its command type R/I/G
the G_bit will store general guide values(char\number)
the GRENRAL bit field will be used in translation the other bitfields in to 8 bit hevxadecimal numbers.
*/
typedef struct
{
    unsigned int n1 : 8;
    unsigned int n2 : 8;
    unsigned int n3 : 8;
    unsigned int n4 : 8;

}General_bit;

typedef struct
{
    unsigned int num : 32;
}G_bit;

typedef struct
{
    unsigned int empty : 6;
    unsigned int function : 5;
    unsigned int rd : 5;
    unsigned int rt : 5;
    unsigned int rs : 5;
    unsigned int opcode : 6;
}R_bit;

typedef struct
{
    int immed : 16;
    unsigned int rt : 5;
    unsigned int rs : 5;
    unsigned int opcode : 6;
}I_bit;

typedef struct
{
    unsigned int address : 25;
    unsigned int is_register : 1;
    unsigned int opcode : 6;
}J_bit;

/*linked list structs */

/*the item data type is used to hols all needed data befor converting
a line of input in to a mchine coode, it will hold bouth the data image an coode image
in two different lists*/
typedef struct Item* pointer_to_item;
typedef struct Item 
{
    int address;
    int register_s;
    int register_t;
    int register_p;
    int immed;
    int is_label; /*label indicator ON\OFF*/
    int is_command;
    int is_reg;  /*register indicator ON\OFF*/
    int command_flag;/*stores the commang\guide number(location in assemblers list)*/

    char immed_symbole[LABEL_SIZE]; /*will hold the label inserted to commands beq... call...*/
    char source_code[LINE_SIZE];  /*stores the line the values is collecte from*/
    char command[COMMAND_SIZE];  /*command name*/
    char symbole[LABEL_SIZE];  /*label name*/
    char char_val; /*char in case of asciz guide*/

    pointer_to_item next;
}Item;

/*the Data struct will hold the symboles tabel*/
typedef struct Data* pointer_to_data;
typedef struct Data
{ 
    char symbole[LABEL_SIZE];
    int address;
    char attributes[20]; /*holds the data type of info code or data*/
    int is_entry;
    int is_external;
    int command_flag; /*stores the commang\guide number(location in assemblers list)*/
    pointer_to_data next;
}Data;

/*the Entry struct will hold the entry or extern tabel*/
typedef struct Entry* pointer_to_entry;
typedef struct Entry 
{
    int is_entry;
    char symbole[LABEL_SIZE];
    int address;
    pointer_to_entry next;
}Entry;

/*bit field will stoe the machine codeafter translation*/
typedef struct Bitfield* pointer_to_bitfield;
typedef struct Bitfield
{
    int RIJ_G; /*holds a number that indicates to functions using it whiche type of info theres in it
               or where to lacte info (in which of the bitfields)*/
    int address;
    R_bit R;  
    I_bit I;
    J_bit J;
    G_bit G;
    pointer_to_bitfield next;
}Bitfield;


/*FIRST ROUND*/

/*GETTING LINE LABEL COMMAD\GUIDE FROM A LINE OF TEXT*/

int get_line(char* string, FILE* file_pointer1, fpos_t position, int* pt_error, int line_counter);

int check_for_label(char* string, int* pt_error, int line_counter);

int get_label(char* line, char* lable, int counter);

int label_correctness_checker(char* label, Data* head_of_data, int line_counter, int* pointer_error);

int check_for_point(char* string, int* pt_error, int line_counter);

int get_guide(char* line, char* command, int line_counter, int* pt_error);

int guide_check(char* command, int line_counter, int* pointer_error);

int get_command(char* line, char* command, int label_flag, int line_counter, int* pt_error);

int comand_number_check(char* command, int line_counter, int* pointer_error);



/*SETTING INFO INTO STRUCT LINKED LIST TO STORE DATA COLLECTED ABOVE*/

int set_info_to_item(pointer_to_item head_of_list, char* line, char* label, int label_flag, char* command, int IC, int command_flag);

int set_info_to_data(pointer_to_data head_of_data, char* label, int address, char* attributes, int command_flag);


/*GETTING REGISTERS & IMMED VALUES FOLLOWING THE COMMAND INSERTAED*/

int get_number_val(pointer_to_item temp, int counter, int line_counter, int* pointer_error, int guide_flag);

int get_immed(pointer_to_item temp, int guid_flag, int line_counter, int* pointer_error);

int get_immed_label(pointer_to_item temp, int command_flag);

int expected_amount_of_registers(int command_flag);

int check_for_number_of_registers(char* string)/*Counting number of '$' in a given line*/;

int get_one_register(char* string, int counter, int line_counter, int* pointer_error);

int get_registers(Item* item, int expected_registers, int line_counter, int* pointer_error);

int set_registers_to_item(Item* item, int command_flag, int line_counter, int* pointer_error);

int set_immed_to_item(Item* item, int command_flag, int line_counter, int* pointer_error);

int set_jump(pointer_to_item head_of_list, int line_counter, int* pointer_error);

/*SETTING INFORMATION TO ITEMS FOLLOWING EACH GUIDE LINE INSERTED*/

int count_variables(char* string);

int DC_plus(int DC, int guid_number);

int get_char_from_guideline(pointer_to_item head_of_list, char* line, int value_number);

int get_number_from_guideline(pointer_to_item head_of_list, char* line, int value_number, int* pointer_error, int line_counter, int guid_flag);

int set_guideline_info_to_item(pointer_to_item head_of_list, char* line, char* label, int label_flag, char* command, int* DC, int value_number, int* pointer_error, int line_counter, int guide_flag);

int string_lengh(char* line, int* pointer_error, int line_counter);

int is_alpha(char c);

int total_information_from_guidline(pointer_to_item* head_of_list, pointer_to_data* head_of_data, char* line, char* label, int label_flag, char* command, int* DC, int guid_flag, int* pointer_error, int line_counter);

/*GETTING AND SETTING INFO FROM ENTRY OR EXTERN GUIDE LINE*/

int set_info_to_entry(pointer_to_entry head_of_entry, char* label, int guide_flag, int address);

int get_label_from_entry_extern(int label_flag, char* line, char* label);

int set_entry(int label_flag, char* line, char* label, pointer_to_entry* head_of_entry, pointer_to_entry* head_of_extern, int guid_flag, int* pointer_error, int line_counter);


/*SECOND ROUND*/

/*UPATING DATA AND CODE IMAGE*/

int fix_data_adress(int ICF, Data* head_of_data);

int is_entry_and_extern(Entry* entry, Entry* externy, int* pointer_error);

int is_extern_in_code(Entry* externy, pointer_to_data* ptp_data, int* pointer_error);

int add_extern_to_data(Entry* externy, pointer_to_data* ptp_data);

int add_entry_to_data(Entry* entry, Data* data);

int label_address_definition(Data* data, Item* item, int* pointer_error);

int set_address_to_external(pointer_to_entry* ptp_external, Entry* head_of_external, Item* item);

int set_address_to_entry(pointer_to_entry* ptp_entry, Entry* head_of_entry, Data* data);

int second_round_data_update(Item* item, Data** ptp_data, Entry* entry, Entry* external, Entry** ptp_entry, Entry** ptp_extern, int DCF, int ICF, int* pt_error);

/*TRANSLATING INFO TO BINARY, AND HEXADECIMAL*/

int transcription(pointer_to_bitfield* head_of_bit, Item* item, Item* data, int ICF);

int set_code_to_bitfield(Bitfield* head_of_bit, Item* item);

int set_data_to_bitfield(Bitfield* head_of_bit, Item* data, int ICF);

int print_bit(Bitfield* bit, Item* item, Item* data, int ICF, int IDF, FILE* file_pointer);

int print_data_bit(Bitfield* bit, Item* data, int ICF, int IDF, FILE* file_pointer);

int make_machine_code_files(Bitfield* bit, Item* item, Item* data, int ICF, int IDF, char* file_name);

int make_extern_files(Entry* external, char* file_name);

int make_entry_files(Entry* entry, char* file_name);

/*GENERAL FUNCTIONS*/

int input_files_correctness(int argc, char** argv);

int check_for_as(char* file_name);

void set_string_to_string(char* string1, char* string2);


/*LINKED LIST MANAGMENT*/

int add_item_to_data(pointer_to_data* pointer_to_pointer_to_data);

int add_item_to_list(pointer_to_item* pointer_to_pointer_to_item);

int add_item_to_entry(pointer_to_entry* pointer_to_pointer_to_entry);

int add_item_to_bitfield(pointer_to_bitfield* pointer_to_pointer_to_bitfield);

void print_entry(Entry* header, FILE* file_pointer);

void free_all(Item* item, Item* data_item, Data* data, Entry* entry, Entry* external, Entry* head_entry, Entry* head_external, Bitfield* bit);

#endif
#include "Header.h"

/*SECOND ROUND*/

/*UPATING DATA AND CODE IMAGE*/
/*
fix data address()
function adds the final instruction count(ICF),to each item in the data list.
*/
int fix_data_adress(int ICF, Data* head_of_data)
{
    while (head_of_data)
    {
        if (head_of_data->attributes[0] == 'd')
            head_of_data->address += ICF;
        head_of_data = head_of_data->next;
    }
    return 0;
}
/*
is entry and extern()
function checks if there is a matched item in bouth lists entry and extern
if there is, prints an error,change error flag to ON, and returns -1.
*/
int is_entry_and_extern(Entry* entry, Entry* externy, int* pointer_error)
{
    Entry* temp = externy;

    while (entry)    /*while the next items pointer is pointing to another item*/
    {
        externy = temp;
        while (externy)
        {
            if ((strcmp(entry->symbole, externy->symbole)) == 0)
            {
                printf("\nErroe: label %s marked as entry and as extern", entry->symbole);
                *pointer_error = 1;
                return -1;
            }
            else
                externy = externy->next;
        }
        entry = entry->next;
    }
    return 0;
}
/*
is_extern_in_code()
function scans the data table, and looks for a match with the extern items
if there is a match prints an error,change error flag to ON, and returns -1.
after scanning if there was no arror append the extern values to the data
list.
*/
int is_extern_in_code(Entry* externy, pointer_to_data* ptp_data, int* pointer_error)
{
    Entry* e_temp = externy;
    Data* temp = *ptp_data;
    Data* data = *ptp_data;
    while (externy)
    {
        data = temp;
        while (data)
        {
            if ((strcmp(data->symbole, externy->symbole)) == 0)
            {
                printf("\nErroe: label %s marked as extern, and also exists in the code.", externy->symbole);
                *pointer_error = 1;
                return -1;
            }
            else
                data = data->next;
        }
        externy = externy->next;
    }

    add_extern_to_data(e_temp, ptp_data);


    return 0;
}
/*
add_extern_to_data()
function append the extern values to the data tabel list.
*/
int add_extern_to_data(Entry* externy, pointer_to_data* ptp_data)
{
    Data* data = *ptp_data;

    while (data)
        data = data->next;

    while (externy)
    {
        add_item_to_data(ptp_data);
        set_info_to_data(*ptp_data, externy->symbole, 0, "external", -1);
        externy = externy->next;
    }
    return 0;
}
/*
add_entry_to_data()
function checks if there is a match between entry labels, and the data list,
if there is a match will torn on the is_entry flag inside the data struct.
*/
int add_entry_to_data(Entry* entry, Data* data)
{
    Entry* temp = entry;
    while (data)
    {
        entry = temp;
        while (entry)
        {
            if ((strcmp(entry->symbole, data->symbole)) == 0)
                data->is_entry = 1;
            entry = entry->next;
        }
        data = data->next;
    }
    return 0;
}
/*
label_address_definition()
function checks for the address of the immed labels that had been inputed in
instructions: bgt, blt, bne, beq OR jump la call.
function looks for the label in the data tabel and sets the distance from it Or the value of its address 
depends on the instruction
*/
int label_address_definition(Data* data, Item* item, int* pointer_error)
{
    int value = 0;
    Data* temp = data;
    while (item)
    {
        data = temp;
        if (((item->command_flag >= 13) && (item->command_flag <= 16)))
        {
            while (data)
            {
                if ((strcmp((data->symbole), (item->immed_symbole))) == 0)
                {
                    if (data->is_external == 1)
                    {
                        *pointer_error = 1;
                        printf("\nError: command %s, refers to an external label %s", item->command, data->symbole);
                    }

                    value = (data->address) - (item->address);
                    item->immed = value;
                    break;
                }
                data = data->next;
            }
        }
        if (((item->command_flag >= 23) && (item->command_flag <= 25)))
        {
            while (data)
            {
                if ((strcmp(data->symbole, item->immed_symbole)) == 0)
                {
                    if (data->is_external == 1)
                    {
                        item->immed = 0;
                    }

                    else
                        item->immed = data->address;
                }
                data = data->next;
            }
        }
        item = item->next;
    }
    return 0;
}
/*
set_address_to_external()
function matches between the extern labels stored in head_of_extern
and the adress that these labels had been mantioned inside instruction\guide list.
and append an Enrry item the a new list that stores each external and its address.
*/
int set_address_to_external(pointer_to_entry* ptp_external, Entry* head_of_external, Item* item)
{
    Item* temp = item;

    while (head_of_external)
    {
        item = temp;
        while (item)
        {
            if (((item->command_flag >= 23) && (item->command_flag <= 25)))
            {
                if ((strcmp(head_of_external->symbole, item->immed_symbole)) == 0)
                {
                    add_item_to_entry(ptp_external);
                    set_info_to_entry(*ptp_external, item->immed_symbole, 5, item->address);
                }
            }
            item = item->next;
        }
        head_of_external = head_of_external->next;

    }
    return 0;
}
/*
set_address_to_entry()
function matches between the entry labels stored in head_of_entry
and the adress that these labels had been mantioned inside data list.
and append an Enrry item the a new list that stores each entry and its address.
*/
int set_address_to_entry(pointer_to_entry* ptp_entry, Entry* head_of_entry, Data* data)
{
    Data* temp = data;

    while (head_of_entry)
    {
        data = temp;
        while (data)
        {
            if ((strcmp(head_of_entry->symbole, data->symbole)) == 0)
            {
                add_item_to_entry(ptp_entry);
                set_info_to_entry(*ptp_entry, data->symbole, 4, data->address);
            }
            data = data->next;
        }
        head_of_entry = head_of_entry->next;
    }

    return 0;
}
/*
second_round_data_update()
function that containes all functions above. so it will look beter in the main function.
*/
int second_round_data_update(Item* item, Data** ptp_data, Entry* entry, Entry* external, Entry** ptp_entry, Entry** ptp_extern, int DCF, int ICF, int* pt_error)
{
    Data* data = *ptp_data;
    fix_data_adress(ICF, data);
    is_entry_and_extern(entry, external, pt_error);
    is_extern_in_code(external, ptp_data, pt_error);
    add_extern_to_data(external, ptp_data);
    add_entry_to_data(entry, data);
    label_address_definition(data, item, pt_error);
    set_address_to_external(ptp_extern, external, item);
    set_address_to_entry(ptp_entry, entry, data);

    return 0;
}

/*TRANSLATING INFO TO BINARY, AND HEXADECIMAL*/
/*
transcription()
function takes the input stored in the struct Item, and translate it to binary
then adds it to the Bitfield struct with its decimal address.
*/
int transcription(pointer_to_bitfield* head_of_bit, Item* item, Item* data, int ICF)
{
    while (item)
    {
        add_item_to_bitfield(head_of_bit);
        set_code_to_bitfield(*head_of_bit, item);
        item = item->next;
    }

    while (data)
    {
        add_item_to_bitfield(head_of_bit);
        set_data_to_bitfield(*head_of_bit, data, ICF);
        data = data->next;
    }
    return 0;
}
/*
set_code_to_bitfield()/set_data_to_bitfield()
function sets the correct numeric values to each item in the instruction list, and guide list.
the Bitfield stuct holds three different bitfields ech for R I J type of command,
and one G for guidelines.
also the struct holds each item address, and an int RIJ_G anumber that indicates the type
of command used.
*/
int set_code_to_bitfield(Bitfield* head_of_bit, Item* item)
{
    int command_index = (item->command_flag) + 1;

    while (head_of_bit->next)
        head_of_bit = head_of_bit->next;

    head_of_bit->address = item->address;

    if (command_index >= 1 && command_index <= 5)
    {
        head_of_bit->RIJ_G = 0;
        head_of_bit->R.opcode = 0;
        head_of_bit->R.function = command_index;
        head_of_bit->R.rs = item->register_s;
        head_of_bit->R.rt = item->register_t;
        head_of_bit->R.rd = item->register_p;
    }

    if (command_index >= 6 && command_index <= 8)
    {
        head_of_bit->RIJ_G = 0;
        head_of_bit->R.opcode = 1;
        head_of_bit->R.function = (command_index - 5);
        head_of_bit->R.rs = item->register_s;
        head_of_bit->R.rd = 0;
        head_of_bit->R.rd = item->register_t;
    }

    if (command_index >= 9 && command_index <= 23)
    {
        head_of_bit->RIJ_G = 1;
        head_of_bit->I.opcode = (command_index + 1);
        head_of_bit->I.rs = item->register_s;
        head_of_bit->I.rt = item->register_t;
        head_of_bit->I.immed = item->immed;
    }

    if (command_index >= 24 && command_index <= 26)
    {
        head_of_bit->RIJ_G = 2;
        head_of_bit->J.opcode = (command_index + 6);
        if (item->is_reg == 1)
        {
            head_of_bit->J.is_register = 1;
            head_of_bit->J.address = item->register_s;
        }
        else
        {
            head_of_bit->J.is_register = 0;
            head_of_bit->J.address = item->immed;
        }
    }

    if (command_index == 27)
    {
        head_of_bit->RIJ_G = 2;
        head_of_bit->J.opcode = 63;
        head_of_bit->J.is_register = 0;
        head_of_bit->J.address = 0;
    }
    return 0;
}

int set_data_to_bitfield(Bitfield* head_of_bit, Item* data, int ICF)
{
    int command_index;
    while (head_of_bit->next)
        head_of_bit = head_of_bit->next;

    head_of_bit->address = (data->address) + ICF;

    command_index = (data->command_flag) + 10;
    head_of_bit->RIJ_G = command_index;
    if (command_index == 10)
    {
        head_of_bit->G.num = data->immed;
    }

    if (command_index == 11)
    {
        head_of_bit->G.num = data->immed;
    }

    if (command_index == 12)
    {
        head_of_bit->G.num = data->immed;
    }

    if (command_index == 13)
    {
        head_of_bit->G.num = data->char_val;
    }
    return 0;
}
/*
print_bit()/print_data_bit()
the function assigen to a bitfiled called General_bit the number stored in each bitfield R I J G,
the general bit splits the number in to 8 byte pices, and then is printed to text.
*/
int print_bit(Bitfield* bit, Item* item, Item* data, int ICF, int IDF, FILE* file_pointer)
{
    unsigned* p;
    R_bit* r = NULL;
    I_bit* i = NULL;
    J_bit* j = NULL;
    General_bit* general = NULL;
    fprintf(file_pointer, "     %d  %d\n", ICF - 100, IDF);
    while (bit)
    {
        if (bit->RIJ_G == 0)
        {
            r = &(bit->R);
            p = (unsigned*)r;
            general = (General_bit*)p;
            fprintf(file_pointer, "%d  %02x  %02x  %02x  %02x\n", bit->address, general->n1, general->n2, general->n3, general->n4);
        }
        if (bit->RIJ_G == 1)
        {
            i = &(bit->I);
            p = (unsigned*)i;
            general = (General_bit*)p;
            fprintf(file_pointer, "%d  %02x  %02x  %02x  %02x\n", bit->address, general->n1, general->n2, general->n3, general->n4);
        }
        if (bit->RIJ_G == 2)
        {
            j = &(bit->J);
            p = (unsigned*)j;
            general = (General_bit*)p;
            fprintf(file_pointer, "%d  %02x  %02x  %02x  %02x\n", bit->address, general->n1, general->n2, general->n3, general->n4);
        }
        bit = bit->next;
    }
    return 0;
}

int print_data_bit(Bitfield* bit, Item* data, int ICF, int IDF, FILE* file_pointer)
{
    int i;
    int counter = 0;
    int* num_list = NULL;
    unsigned* p;
    G_bit* g = NULL;
    General_bit* general = NULL;
    num_list = calloc(IDF, sizeof(int));
    while ((bit != NULL) && (bit->RIJ_G < 10))
        bit = bit->next;
    if (num_list == NULL)
    {
        printf("\nError: memory allocation fail");
        exit(0);
    }

    while (bit)
    {
        g = &(bit->G);
        p = (unsigned*)g;
        general = (General_bit*)p;
        if (bit->RIJ_G == 10)
        {
            num_list[counter++] = general->n1;
            num_list[counter++] = general->n2;
        }
        if (bit->RIJ_G == 11)
        {
            num_list[counter++] = general->n1;
            num_list[counter++] = general->n2;
            num_list[counter++] = general->n3;
            num_list[counter++] = general->n4;
        }
        if (bit->RIJ_G > 11)
        {
            num_list[counter++] = general->n1;
        }
        bit = bit->next;
    }
    for (i = 0;i < counter;i++)
    {
        if (i == 0)
            fprintf(file_pointer, "%02d  ", (ICF + i));
        if ((i % 4 == 0) && (i != 0))
            fprintf(file_pointer, "\n%02d  ", ICF + i);
        fprintf(file_pointer, "%02x  ", num_list[i]);
       
    }
    return 0;
}






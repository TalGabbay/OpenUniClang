#include "Header.h"
/*GENERAL FUNCTIONS*/
/*
input file correctness()
checks if there is afile excluding the prodram itself.
checks if the file inserted in command line is correct(ends with .as)
*/
int input_files_correctness(int argc, char** argv)
{
    int file_counter = 1;
    if (argc == 1)
    {
        printf("\nNo files were identify, program exits");
        exit(0);
    }

    while (argv[file_counter])
    {
        check_for_as(argv[file_counter]);
        file_counter++;
    }
    return 0;
}

int check_for_as(char* file_name)
{
    int len = strlen(file_name);
    if ((file_name[len - 3] == '.') && (file_name[len - 2] == 'a') && (file_name[len-1] == 's'))
        return 1;

    else
    {
        printf("\nError: file name doesnt match file_name.as, program exits");
        exit(0);
    }

}

/*
set string to string()
setr string2 to string 1
*/
void set_string_to_string(char* string1, char* string2)
{
    int i = 0;
    while (string2[i])
    {
        string1[i] = string2[i];
        i++;
    }
    string1[i] = '\0';
    return;
}

/*LINKED LIST MANAGMENT*/

/*
add_item_to...()
all workes the same each for its own data type
function gets a pointer to apointer and add an item to the linked list that pointer
that the pointer is pointing to.
*/
int add_item_to_data(pointer_to_data* pointer_to_pointer_to_data)
{
    pointer_to_data temp, p1;
    p1 = temp = NULL;
    temp = calloc(1, sizeof(Data));
    if (!temp)
    {
        printf("\n1)Memory allocation error, exits program.");
        exit(0);
    }
    p1 = *pointer_to_pointer_to_data;
    if (p1 == NULL)
    {
        *pointer_to_pointer_to_data = temp;
        temp->next = NULL;
    }
    else
    {
        while (p1->next)
        {
            p1 = p1->next;
        }
        p1->next = temp;
    }
    return 0;
}

int add_item_to_list(pointer_to_item* pointer_to_pointer_to_item)
{
    /*locating memory for one item type. temp is pointing to it*/
    pointer_to_item temp, p1, p2;
    p1 = p2 = temp = NULL;
    temp = (pointer_to_item)calloc(1, sizeof(Item));
    if (!temp)
    {
        printf("\n1)Memory allocation error, exits program.");
        exit(0);
    }
    p1 = *pointer_to_pointer_to_item;           /*equals to the value of main pointer header.*/
    if (p1 == NULL)                  /*check if list is empty*/
    {
        *pointer_to_pointer_to_item = temp;     /*value of main header change to point the new allocated memory*/
        temp->next = NULL;             /*locating p1 value in item next filed, meaning that it will point to NULL.*/
    }
    else
    {
        while (p1->next)                    /*as long as p1 != NULL moving forward till getting to the last item on list*/
        {
            p1 = p1->next;           /*moving p1 to point to the followed item*/
        }
        p1->next = temp;               /*changing current last item to point to the new last item*/
        temp->next = NULL;               /*last item next field equals to NULL*/
    }
    return 0;
}

int add_item_to_entry(pointer_to_entry* pointer_to_pointer_to_entry)
{
    pointer_to_entry temp, p1;
    p1 = temp = NULL;
    temp = calloc(1, sizeof(Entry));
    if (!temp)
    {
        printf("\n1)Memory allocation error, exits program.");
        exit(0);
    }
    p1 = *pointer_to_pointer_to_entry;
    if (p1 == NULL)
    {
        *pointer_to_pointer_to_entry = temp;
        temp->next = NULL;
    }
    else
    {
        while (p1->next)
        {
            p1 = p1->next;
        }
        p1->next = temp;
    }
    return 0;
}

int add_item_to_bitfield(pointer_to_bitfield* pointer_to_pointer_to_bitfield)
{
    pointer_to_bitfield temp, p1;
    p1 = temp = NULL;
    temp = calloc(1, sizeof(Bitfield));
    if (!temp)
    {
        printf("\n1)Memory allocation error, exits program.");
        exit(0);
    }
    p1 = *pointer_to_pointer_to_bitfield;
    if (p1 == NULL)
    {
        *pointer_to_pointer_to_bitfield = temp;
        temp->next = NULL;
    }
    else
    {
        while (p1->next)
        {
            p1 = p1->next;
        }
        p1->next = temp;
    }
    return 0;
}

void free_all(Item* item, Item* data_item, Data* data, Entry* entry, Entry* external, Entry* head_entry, Entry* head_external, Bitfield* bit)
{
    free(item);
    free(data_item);
    free(data);
    free(entry);
    free(external);
    free(head_entry);
    free(head_external);
    free(bit);
}


/*MAKING FILES*/
/*
fix_file_name()
used in the making of files to make the correct output file name.
function changes the file end acording to the file needed.
*/
char* fix_file_name(char* string, char* name, int type)
{
    int i = 0;
    while (string[i] != '\0')
    {
        name[i] = string[i];
        if (string[i] == '.') {
            break;
        }
        i++;
    }

    if (type == 0)
    {
        name[i + 1] = 'o';
        name[i + 2] = 'b';
        name[i + 3] = '\0';
        return name;
    }

    if (type == 1)
    {
        name[i + 1] = 'e';
        name[i + 2] = 'x';
        name[i + 3] = 't';
        name[i + 4] = '\0';
        return name;
    }

    if (type == 2)
    {
        name[i + 1] = 'e';
        name[i + 2] = 'n';
        name[i + 3] = 't';
        name[i + 4] = '\0';
        return name;
    }
    return name;
}

/*
make_files()
all function activate other functions that had been discibed before.
*/
int make_machine_code_files(Bitfield* bit, Item* item, Item* data, int ICF, int IDF, char* file_name)
{
    char* fixed_name[FILE_NAME_SIZE];
    FILE* file_pointer;
    fix_file_name(file_name, (char*)fixed_name, 0);
    file_pointer = fopen((char*)fixed_name, "a");
    if (file_pointer == NULL)
    {
        printf("\nError: making ob file fail, program exit");
        exit(0);
    }
    print_bit(bit, item, data, ICF, IDF, file_pointer);
    print_data_bit(bit, data, ICF, IDF, file_pointer);
    return 0;
}

int make_extern_files(Entry* external, char* file_name)
{
    char* fixed_name[FILE_NAME_SIZE];
    FILE* file_pointer;
    fix_file_name(file_name, (char*)fixed_name, 1);
    file_pointer = fopen((char*)fixed_name, "a");
    if (file_pointer == NULL)
    {
        printf("\nError: making ob file fail, program exit");
        exit(0);
    }
    print_entry(external, file_pointer);
    return 0;
}

int make_entry_files(Entry* entry, char* file_name)
{
    char* fixed_name[FILE_NAME_SIZE];
    FILE* file_pointer;
    fixed_name[0] = '\0';
    fix_file_name(file_name, (char*)fixed_name, 2);
    file_pointer = fopen((char*)fixed_name, "a");
    if (file_pointer == NULL)
    {
        printf("\nError: making ob file fail, program exit");
        exit(0);
    }
    print_entry(entry, file_pointer);
    return 0;
}





/*DEBUG*/

void print_list(Item* header)
{
    while (header)
    {
        printf("\ncommand = %s", header->command);
        printf("\nregister1 = %d |register2 = %d |register3 = %d |", header->register_s, header->register_t, header->register_p);
        printf("\naddress = %d", header->address);
        printf("\nimmed = %d", header->immed);
        printf("\nchar = %c", header->char_val);
        if (header->immed_symbole)
            printf("\nimmed_symbole = %s\n", header->immed_symbole);

        header = header->next;
    }
}

void print_data(Data* header) /*function that prints every items data section following its item pointer*/
{
    while (header)    /*while the next items pointer is pointing to another item*/
    {
        /*printf("\nData Table:\n");*/
        printf("\ncommand_flag = %d |", header->command_flag);
        printf("\nlabel = %s |", header->symbole);
        printf("address = %d |", header->address);
        printf("attribute = %s |", header->attributes);
        if (header->is_entry == 1)
            printf("is antry = %d |", header->is_entry);
        if (header->is_external == 1)
            printf("is extern = %d |", header->is_external);
        header = header->next; /*posing header on to the next items location*/
    }
}

void print_entry(Entry* header, FILE* file_pointer) /*function that prints every items data section following its item pointer*/
{
    while (header)    /*while the next items pointer is pointing to another item*/
    {
        fprintf(file_pointer, "\n%s   %d", header->symbole, header->address);
        header = header->next; /*posing header on to the next items location*/
    }
}

void print_entry1(Entry* header) /*function that prints every items data section following its item pointer*/
{
    while (header)    /*while the next items pointer is pointing to another item*/
    {
        printf("\n%s   %d", header->symbole, header->address);
        header = header->next; /*posing header on to the next items location*/
    }
}

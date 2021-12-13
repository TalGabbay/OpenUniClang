#include "Header.h"

/*GETTING LINE LABEL COMMAD\GUIDE FROM A LINE OF TEXT*/
 /*
  get line()
  the function will store as global char* line, the line input from a file
  if the line is empty or contains a comment only, the function will return -1, else return 0.
 */
int get_line(char* string, FILE* file_pointer1, fpos_t position, int *pt_error, int line_counter)
{
    int note_flag = 0;
    int space_flag = 0;
    int i = 0;
    int len = 0;
    char c;
    while (((c = fgetc(file_pointer1)) != '\n') && (c != EOF))
    {
        if (i == (LINE_SIZE-1))
        {
            *pt_error = 1;
            printf("\nError: line %d is to long, passing line",line_counter);
            while (((c = fgetc(file_pointer1)) != '\n') && (c))
                continue;
            return -1;
        }
        /*ignoring white extra Tabs*/
        if (c == 13)
            continue;
        if ((c == '\t' || c == ' ') && (space_flag == 1))
        {
            continue;
        }
        space_flag = 0;
        if (c == '\t' || c == ' ')
        {
            space_flag = 1;
        }
        /*ignoring notes*/
        if (c == ';')
        {
            note_flag = 1;
            string[i] = '\0';
        }
        if (note_flag == 0)
            (string[i]) = c;
        i++;
    }
    string[i] = '\0';
    len = strlen(string);
    if ((len == 1) && ((string[0] == ' ')|| (string[0] == '\t')))
    {
        return -1;
    }   
    if (len < 1)
        return -1;
    fgetpos(file_pointer1, &position);
    return 0;
}
/*
Check for label()
the function looks for ':' if find one return the index of it in the global char* called line
if no ':' had been found function
*/
int check_for_label(char* string, int* pt_error, int line_counter)
{
    int counter = 0;
    int string_lengh = strlen(string);
    while (counter++ < string_lengh)
    {
        if (string[counter] == ':')
        {
            if (counter >= LABEL_SIZE-1)
            {
                *pt_error = 1;
                printf("\nError: line %d, label name is to long", line_counter);
                return -1;
            }
            return counter;
        }
            
    }
    return -1;
}
/*
get label()
uses the location of ':' from the "check for label" function, the global char* line,
gets the label from line and stores it in cha* label.
*/
int get_label(char* line, char* lable, int counter)
{
    int i;
    int j = 0;
    for (i = 0;i < counter;i++)
    {
        if (line[i] == ' ' || line[i] == '\t')
            continue;
        else
            lable[j++] = line[i];
    }
    lable[j] = '\0';
    return 0;
}
/*
label_correctness_checker()
function looks for match between command names, guide names, alredy in use labels
by scanning each of the lists, if find one match prints an error, erro_flag turn ON, and function will return -1.
*/
int label_correctness_checker(char* label, Data* head_of_data, int line_counter, int* pointer_error)
{
    int command_indicator = 0;
    int guid_indicator = 0;

    if ((label[0] < 97 && label[0] > 90) || (label[0] > 122) || (label[0] < 65))
    {
        return -1;
    }
    command_indicator = comand_number_check(label, -1, pointer_error);
    if (command_indicator != -1)
    {
        *pointer_error = 1;
        printf("\nError:line %d label name %s cannot be same as command name.", line_counter, label);
        return-1;
    }
    guid_indicator = guide_check(label, -1, pointer_error);
    if (guid_indicator != -1)
    {
        *pointer_error = 1;
        printf("\nError:line %d label name %s cannot be same as guide name.", line_counter, label);
        return-1;
    }

    while (head_of_data)
    {
        if ((strcmp(head_of_data->symbole, label) == 0))
        {
            *pointer_error = 1;
            printf("\nError:line %d label name %s already in use.", line_counter, label);
            return-1;
        }
        head_of_data = head_of_data->next;
    }

    return 0;
}
/*
Check for point()
function looks for '.' if find one return the index of it in the global char*  command
if no '.' had been found function will return -1.
*/
int check_for_point(char* string, int* pt_error, int line_counter)
{
    int counter = 0;
    while (string[counter])
    {
        if (string[counter] == '.')
        {
            if (counter >= COMMAND_SIZE - 1)
            {
                *pt_error = 1;
                printf("\nError: line %d, guide name is to long", line_counter);
                return -1;
            }
            return counter;
        }
            
        counter++;
    }
    return -1;
}
/*
get guide()
uses the location of '.' from the "check for point" function, the global char* line,
gets the guide from line and stores it in cha* command.

*/
int get_guide(char* line, char* command, int line_counter, int* pt_error)
{
    int i, j, flag, string_lengh;
    j = flag = 0;
    string_lengh = strlen(line);
    for (i = 0;i < string_lengh;i++)
    {
        if (line[i] == '.')
        {
            flag = 1;
            continue;
        }
        if (flag == 1)
        {
            if (j >= COMMAND_SIZE - 1)
            {
                *pt_error = 1;
                printf("\nError: line %d, guide name is to long", line_counter);
                return -1;
            }
            if ((line[i]) == ' ' || (line[i]) == '$' || (line[i]) == ',')
            {
                command[j++] = '\0';
                return 0;
            }
            command[j++] = line[i];
        }
    }
    command[j++] = '\0';
    return 0;
}
/*
guide check()
checks if there is amtch between the guide to one of the gudes in the assember guide list
if ther is amatch will return the location of match in the list(int), else return -1.
*/
int guide_check(char* command, int line_counter, int* pointer_error)
{
    int i = 0;
    char guide_list[6][8] = { "dh\0","dw\0","db\0","asciz\0","entry\0","extern\0" };
    char* str;
    while (i < 6)
    {
        str = (guide_list[i]);
        if ((strcmp(command, str)) == 0)
            return i;
        i++;
    }
    if (line_counter != -1)
    {
        *pointer_error = 1;
        printf("\nError:line %d guide name %s incorrect.", line_counter, command);
    }

    return (-1);
}
/*
get command()
will operate if ther is no '.' only, the function wiil get the command from input,
using the line, and label flag that will indicate if ther is a label that need to be passed first.
the command will be stred inside the global char* command.
*/
int get_command(char* line, char* command, int label_flag, int line_counter, int* pt_error)
{
    int i = 0;
    int j = 0;
    int flag = 0;

    if (label_flag != -1)
    {
        i = label_flag + 1;
    }

    while (line[i])
    {
        if (j >= COMMAND_SIZE - 1)
        {
            *pt_error = 1;
            printf("\nError: line %d, command name is to long", line_counter);
            return -1;
        }
        if ((flag == 0) && (line[i] != ' ') && (line[i] != '\t'))
            flag = 1;
        if ((flag == 1) && ((line[i] == ' ') || (line[i] == '\t')))
            break;
        if (flag == 1)
            command[j++] = line[i];
        i++;
    }
    command[j] = '\0';
    return 0;
}

/*
command number check()
checks if there is amtch between the commad to one of the commands in the assember command list
if ther is a match will return the location of match in the list(int), else will print an error
and will return -1.
*/
int comand_number_check(char* command, int line_counter, int* pointer_error)
{
    char command_list[28][5] = { {"add\0"},{"sub\0"},{"and\0"},{"or\0"},{"nor\0"},{"move\0"},{"mvhi\0"},{"mvlo\0"},{"addi\0"},{"subi\0"},{"andi\0"},{"ori\0"},{"nori\0"},{"bne\0"},{"beq\0"},{"blt\0"},{"bgt\0"},{"lb\0"},{"sb\0"},{"lwv"},{"sw\0"},{"lh\0"},{"sh\0"},{"jmp"},{"la\0"},{"call\0"},{"stop\0"},{'\0'} };
    int i = 0;
    char* str;
    while (i < 27)
    {
        str = (command_list[i]);
        if ((strcmp(command, str)) == 0)
            return i;
        i++;
    }
    if (line_counter != -1)
    {
        *pointer_error = 1;
        printf("\nError:line %d name %s does not match any guide or command.", line_counter, command);
        return (-1);
    }
    return (-1);
}

/*SETTING INFO INTO STRUCT LINKED LIST TO STORE DATA COLLECTED ABOVE*/
/*
set_info_to_item() 
function will act as following:
1. item pointer to point to last item in the linked list.
2.set number of command,source line,label (if ther is),instruction count,command,
and will turn on the command flag is_command.
*/
int set_info_to_item(pointer_to_item head_of_list, char* line, char* label, int label_flag, char* command, int IC, int command_flag)
{
    /*moving to the last chain in the linked list*/
    while (head_of_list->next)
        head_of_list = head_of_list->next;

    head_of_list->command_flag = command_flag;
    /*setting all availble info in to the current item*/
    set_string_to_string(head_of_list->source_code, line);
    if (label_flag != -1)
    {
        head_of_list->is_label = 1;
        set_string_to_string(head_of_list->symbole, label);
    }
    head_of_list->address = IC;
    head_of_list->is_command = 1;
    set_string_to_string(head_of_list->command, command);

    return 0;
}
/*
set_info_to_data()
function will act as following :
1. item pointer to point to last item in the linked list.
2.set all availbel info into the data structure.
*/
int set_info_to_data(pointer_to_data head_of_data, char* label, int address, char* attributes, int command_flag)
{
    while (head_of_data->next)
        head_of_data = head_of_data->next;
    head_of_data->command_flag = command_flag;
    set_string_to_string(head_of_data->symbole, label);
    set_string_to_string(head_of_data->attributes, attributes);
    if (attributes[0] == 'e')
        head_of_data->is_external = 1;
    head_of_data->address = address;
    return 0;
}

/*GETTING REGISTERS & IMMED VALUES FOLLOWING THE COMMAND INSERTAED*/
/*
function will get a number value from a string of chars, function will take +,- signes 
function is used in bouth function:
1.get immed() in order to get the value of immed inserted
2.get_number_from_guideline, in order to get avalue from dh,db,dw guidlines
*/
int get_number_val(pointer_to_item temp, int counter, int line_counter, int* pointer_error, int guide_flag)
{
    int minus_flag = 1;
    int number;
    int index = 0;
    char number_char[10];

    if (temp->source_code[counter] == ',' || temp->source_code[counter] == ' ')
    {
        counter++;
        if (temp->source_code[counter] == '+')
            counter++;
        if (temp->source_code[counter] == '-')
        {
            minus_flag = -1;
            counter++;
        }
        while (temp->source_code[counter] != ',' && temp->source_code[counter] != '\0')
        {
            if ((temp->source_code[counter] >= '0') || (temp->source_code[counter] <= '9'))
            {
                number_char[index++] = temp->source_code[counter];
                counter++;
            }

            else
            {
                printf("\nError:line %d, user inserted a non digit (%c) to immed value", line_counter, temp->source_code[counter]);
                *pointer_error = 1;
                return -1;
            }
        }

    }
    number_char[index++] = '\0';

    if ((strcmp(number_char, "0\0")) == 0)
    {
        number = 0;
        temp->immed = number;
        return 0;
    }
    else
    {
        number = minus_flag * atoi(number_char);
        if (number == 0)
        {
            number = '\0';
        }
    }
    if (guide_flag == -1)
    {
        temp->immed = number;
        return 0;
    }
    return number;

}
/*
get immed()
the function gets a number value from the global line string using the get_number_val function.
function return the value of number as int, and stores it in the current item.
*/
int get_immed(pointer_to_item temp, int guid_flag, int line_counter, int* pointer_error)
{
    int number;
    int counter = 0;
    while (temp->source_code[counter] != '$')
        counter++;
    while (temp->source_code[counter] != ',' && temp->source_code[counter] != '\0')
        counter++;
    number = get_number_val(temp, counter, line_counter, pointer_error, -1);

    if (number != '\0')
    {
        temp->immed = number;
        return 0;
    }
    else
        return -1;
}
/*
get immed_val()
the function gets the symbole value insertad in some of the commands, from the global line string using the get_number_val function.
will store the string inside the current item.
*/
int get_immed_label(pointer_to_item temp, int command_flag)
{
    int dollar_counter = 0;
    int i = 0;
    int j = 0;
    int flag = 0;
    int number_of_registers = check_for_number_of_registers(temp->source_code);


    if (temp->is_label == 1)
    {
        while (temp->source_code[i++])
            if (temp->source_code[i] == ':') {
                i++;
                break;
            }
    }

    if (command_flag >= 23)
    {
        while (((temp->source_code[i]) && ((temp->source_code[i] == ' ') || (temp->source_code[i] == '\t'))))
            i++;
        while (temp->source_code[i++])
        {
            if (temp->source_code[i] == ' ' || temp->source_code[i] == '\t')
            {
                i++;
                flag = 1;
            }
            if (flag == 1)
            {
                if (temp->source_code[i] != ' ')
                    temp->immed_symbole[j++] = temp->source_code[i];
            }

        }
    }

    else
    {
        while (temp->source_code[i++])
        {
            if (temp->source_code[i] == '$')
                dollar_counter++;
            if ((dollar_counter == number_of_registers) && (temp->source_code[i] == ','))
            {
                break;
            }
        }
        while (temp->source_code[i++] != '\0')
        {
            if (temp->source_code[i] == ',')
                break;
            if (temp->source_code[i] != ' ')
                temp->immed_symbole[j++] = temp->source_code[i];
        }
    }
    return 0;
}
/*
expected_amount_of_registers()
uses the value of command flag wich indicates the location of command in the commad list.
usig that number gives a single valued function,.
*/
int expected_amount_of_registers(int command_flag)
{
    if ((command_flag >= 0) && (command_flag <= 4))
        return 3;
    if ((command_flag >= 5) && (command_flag <= 22))
        return 2;
    else
        return 0;
}
/*
check_for_number_of_registers()
function counts '$' chars in the currnt inpute line.
*/
int check_for_number_of_registers(char* string)/*Counting number of '$' in a given line*/
{
    int counter = 0;
    int dollar_counter = 0;
    while (string[counter++])
    {
        if (string[counter] == '$')
            dollar_counter++;
    }
    return dollar_counter;
}
/*
get_one_register()
the function looks for the next $ char then gets the number that is written afterwards.
the function gets an int counter which indicates where the function should start looking for
the $ char, that way we can use it to get all three registers.
function return the value of number as int.
*/
int get_one_register(char* string, int counter, int line_counter, int* pointer_error)
{
    char number_befor_convertion[10];
    int index = 0;
    int number;
    while ((string[counter] != ',') && (string[counter] != '\0'))
    {
        if (string[counter] == '$')
        {
            counter++;
            continue;
        }

        number_befor_convertion[index++] = string[counter];
        counter++;
    }
    number_befor_convertion[index] = '\0';
    index = 0;

    if (strcmp(number_befor_convertion, "0\0") == 0)
        return 0;

    number = atoi(number_befor_convertion);
    if ((number >= 1) && (number <= 31))
        return number;
    else
    {
        printf("\nError:line %d register value is out of range", line_counter);
        *pointer_error = 1;
        return(-1);
    }
}
/*
get_registers()
the function uses the expected number of registers and gets with the function get_one_register()
the wanted amount of registers, and stores it in the correct register field inside the given item.
*/
int get_registers(Item* item, int expected_registers, int line_counter, int* pointer_error)
{
    int i = 0;
    int reg_count = 0;
    int number;
    while (reg_count < expected_registers)
    {
        if (item->source_code[i++] == '$')
        {
            reg_count++;
            number = get_one_register(item->source_code, i, line_counter, pointer_error);
            if (number == -1)
            {
                return -1;
            }
        }
        if (reg_count == 1)
            item->register_s = number;
        if (reg_count == 2)
            item->register_t = number;
        if (reg_count == 3)
            item->register_p = number;
    }
    return 0;
}
/*
set registers to item()
the function act as following
1.checks for a match between the wanted amount of registers, to the amount found in the input line.
2.if the above is correct, the function will look for the last item in the items linked list.
3.the function will activate the get registers.
*/
int set_registers_to_item(Item* item, int command_flag, int line_counter, int* pointer_error)
{


    int err_flag = 0;
    int expected_registers = expected_amount_of_registers(command_flag);
    int reg_count;

    if (command_flag == 23)
        return 23;

    while (item->next)
        item = item->next;

    err_flag = 0;
    expected_registers = expected_amount_of_registers(command_flag);
    reg_count = check_for_number_of_registers(item->source_code);



    if (expected_registers != reg_count)
    {
        printf("\nError:line %d command expects amount of %d registers, user insetred %d amount registers", line_counter, expected_registers, reg_count);
        *pointer_error = 1;
        return -1;
    }

    err_flag = get_registers(item, expected_registers, line_counter, pointer_error);
    return err_flag;

}
/*
set_immed_to_item()
the function act as following
accotding to the command number, the function will activate the right immed function(label/number)
with the last item in item linked list.
*/
int set_immed_to_item(Item* item, int command_flag, int line_counter, int* pointer_error)
{
    while (item->next)
        item = item->next;

    if (((command_flag >= 8) && (command_flag <= 12)) || ((command_flag >= 17) && (command_flag <= 22)))
    {
        get_immed(item, -1, line_counter, pointer_error);
        return 0;
    }
    if (((command_flag >= 13) && (command_flag <= 16)) || ((command_flag >= 24) && (command_flag <= 26)))
    {
        get_immed_label(item, command_flag);
        return 0;
    }
    else
        return 0;
}
/*
set_jmp()
the function act as following
1.check if there is a register, if there is, chaeck that there is only one (fitting the jmp command)
if there is only one function activates get registers, with last item in chain.
2.if there was no reg, function will activate get_immed_label function that will assign
the label name to the last item in chain.
*/
int set_jump(pointer_to_item head_of_list, int line_counter, int* pointer_error)
{
    int number_of_registers = 0;
    while (head_of_list->next)
        head_of_list = head_of_list->next;

    number_of_registers = check_for_number_of_registers(head_of_list->source_code);
    if (number_of_registers > 1)
    {
        printf("Error:line %d jump command expect one registers only", line_counter);
        *pointer_error = 1;
        return(-1);
    }
    if (number_of_registers == 1)
    {
        head_of_list->is_reg = 1;
        get_registers(head_of_list, number_of_registers, line_counter, pointer_error);
    }
    else
    {
        head_of_list->is_reg = 0;
        get_immed_label(head_of_list, 23);
    }
    return 0;
}

/*SETTING INFORMATION TO ITEMS FOLLOWING EACH GUIDE LINE INSERTED*/
/*
count_variables()
will activate only for dh,dw,db guidelines.
count ',' in a given line and figure out the number of variables.
*/
int count_variables(char* string)
{
    int counter = 0;
    int i = 0;
    while (string[i] != '.' && string[i] != '\0')
        i++;
    while (string[i] != ' ' && string[i] != '\0')
        i++;

    while (string[i] != '\0')
    {
        if (string[i] == ',')
        {
            counter++;
        }
        i++;
    }
    return (++counter);
}
/*
DC_plus()
increase the data counter acording to the data type inserted.
*/
int DC_plus(int DC, int guid_number)
{
    if (guid_number == 0)
        return (DC += 2);
    if ((guid_number == 2) || (guid_number == 3))
        return (DC += 1);
    else
        return(DC += 4);
}
/*
get_char_from_guideline()
used in the asciz guideline, will get one char and assign it to the last item in chain, 
using the value number will give the function the correct char location.
*/
int get_char_from_guideline(pointer_to_item head_of_list, char* line, int value_number)
{
    int i = 0;
    while (line[i++] != '"')
        continue;
    if ((line[i + value_number]) == '"')
        head_of_list->char_val = 0;
    else
        head_of_list->char_val = line[i + value_number];
    return 0;
}
/*
get_number_from_guideline()
1.using the value number the function gets the correct number in the string of number,.
2.function using the get number val function, in order to cinvert the char number to an int.
3.function check the size of number inserted and sees if  it matches the size of guide line.
if correct will inset it to the last item in the data chain.
*/
int get_number_from_guideline(pointer_to_item head_of_list, char* line, int value_number, int* pointer_error, int line_counter, int guid_flag)
{
    int counter = 0;
    int comma_count = 0;
    int number = '\0';
    while (head_of_list->next)
    {
        head_of_list = head_of_list->next;
    }

    if (value_number == 0)
    {
        while ((line[counter] != '\0') && (line[counter] != '.'))
            counter++;
        while ((line[counter] != '\0') && (line[counter] != ' '))
            counter++;
    }
    else
    {
        while ((line[counter] != '\0'))
        {
            if (line[counter] == ',')
                comma_count++;
            if (comma_count == (value_number))
                break;
            counter++;
        }
    }
    number = get_number_val(head_of_list, counter, line_counter, pointer_error, 1);


    if (number != '\0')
    {
        if (guid_flag == 0)
        {
            if ((number >= 65536) || (number <= -32768))
            {
                *pointer_error = 1;
                printf("\nError:line %d number is bigger OR smaller then the ability of dh(2^16 bit) to store", line_counter);
                return(-1);
            }
        }
        if (guid_flag == 2)
        {
            if ((number >= 256) || (number <= -128))
            {
                *pointer_error = 1;
                printf("\nError:line %d number is bigger OR smaller then the ability of db(2^8 bit) to store", line_counter);
                return(-1);
            }
        }

        head_of_list->immed = number;
        return 0;
    }
    else
    {
        *pointer_error = 1;
        printf("\nError:line %dguideline number value is illigal (not a number)", line_counter);
        return(-1);
    }
}
/*
set_guideline_info_to_item()
sets all info from command line to the last item in data chain,
activates the correct function number/char from guideline.
*/
int set_guideline_info_to_item(pointer_to_item head_of_list, char* line, char* label, int label_flag, char* command, int* DC, int value_number, int* pointer_error, int line_counter, int guide_flag)
{

    while (head_of_list->next)
    {
        head_of_list = head_of_list->next;
    }

    if (label_flag != -1)
        set_string_to_string(head_of_list->symbole, label);
    set_string_to_string(head_of_list->command, command);
    set_string_to_string(head_of_list->source_code, line);
    head_of_list->is_command = 0;
    head_of_list->address = *DC;
    head_of_list->command_flag = guide_flag;
    if (guide_flag <= 2)
        get_number_from_guideline(head_of_list, line, value_number, pointer_error, line_counter, guide_flag);
    if (guide_flag == 3)
        get_char_from_guideline(head_of_list, line, value_number);


    return 0;
}
/*
stsing lengh()
counts the number of chars in the "string" inserted to asciz guideline.
check for input correctness.
*/
int string_lengh(char* line, int* pointer_error, int line_counter)
{
    int counter = 0;
    int i = 0;
    int flag = 0;
    while (line[i++])
    {
        if (line[i] == '"')
        {
            flag++;
            i++;
        }

        if (flag == 1)
        {
            if (is_alpha(line[i]) == 1)
                counter++;
            if (is_alpha(line[i]) == -1)
            {
                *pointer_error = 1;
                printf("\nError:line %d asciz guide line allows only alphabet", line_counter);
                return -1;
            }
        }
    }
    if (flag == 2)
    {
        return counter;
    }
    if (flag == 1)
    {
        *pointer_error = 1;
        printf("\nError:line %d asciz guide line is missing a-\" ", line_counter);
        return -1;
    }
    if (flag == 0)
    {
        *pointer_error = 1;
        printf("\nError:line %d asciz guide line no string was found", line_counter);
        return -1;
    }
    if (flag != 2)
    {
        *pointer_error = 1;
        printf("\nError:line %d asciz guide linewrong amount of-\" ", line_counter);
        return -1;
    }
    return 0;
}
/*
is_alpha()
checks if the inserted char is alphabet or blank tab.
*/
int is_alpha(char c)
{
    if (((c < 123) && (c > 96)) || ((c < 91) && (c > 64)) || (c == 0) || (c == ' ') || (c = '\t'))
        return 1;
    else
        return -1;
}
/*
total_information_from_guidline()
function will activat the right function from above, according to the gudeline number.
function will loop in order to get string of char,int values.
*/
int total_information_from_guidline(pointer_to_item* head_of_list, pointer_to_data* head_of_data, char* line, char* label, int label_flag, char* command, int* DC, int guid_flag, int* pointer_error, int line_counter)
{
    int string_len = 0;
    int i = 0;
    int num_of_var_in_guideline = 0;
    if (guid_flag <= 2)
    {
        num_of_var_in_guideline = count_variables(line);
        for (i = 0; i < num_of_var_in_guideline; i++)
        {
            add_item_to_list(head_of_list);
            if (i == 0 && label_flag != -1) /*store in the data tabel*/
            {
                add_item_to_data(head_of_data);
                set_info_to_data(*head_of_data, label, *DC, "data", -1);
                set_guideline_info_to_item(*head_of_list, line, label, label_flag, command, DC, i, pointer_error, line_counter, guid_flag);
            }
            else
            {
                set_guideline_info_to_item(*head_of_list, line, label, label_flag, command, DC, i, pointer_error, line_counter, guid_flag);
            }
            *DC = DC_plus(*DC, guid_flag);
        }
        return 0;
    }
    if (guid_flag == 3)
    {
        string_len = string_lengh(line, pointer_error, line_counter);
        for (i = 0; i <= string_len; i++)
        {
            add_item_to_list(head_of_list);
            if (i == 0 && label_flag != -1)
            {
                add_item_to_data(head_of_data);
                set_guideline_info_to_item(*head_of_list, line, label, label_flag, command, DC, i, pointer_error, line_counter, guid_flag);
                set_info_to_data(*head_of_data, label, *DC, "data", -1);
            }
            else
            {
                set_guideline_info_to_item(*head_of_list, line, label, label_flag, command, DC, i, pointer_error, line_counter, guid_flag);
            }
            *DC = DC_plus(*DC, guid_flag);
        }
        return 0;
    }
    return 0;
}

/*GETTING AND SETTING INFO FROM ENTRY OR EXTERN GUIDE LINE*/
/*
set_info_to_entry()
sets all availnble info to the last Entry data type in chain.
if is entry equals 0, it means that the item is extern.
*/
int set_info_to_entry(pointer_to_entry head_of_entry, char* label, int guide_flag, int address)
{
    while (head_of_entry->next)
        head_of_entry = head_of_entry->next;
    set_string_to_string(head_of_entry->symbole, label);
    head_of_entry->address = address;
    if (guide_flag == 4)
        head_of_entry->is_entry = 1;
    else
        head_of_entry->is_entry = 0;
    return 0;
}
/*
get_label_from_entry_extern()
prints a warnning if there is a label to that type of command.
gets the label inserted for the command(not the command label).
*/
int get_label_from_entry_extern(int label_flag, char* line, char* label)
{
    int i = 0;
    int j = 0;
    int flag = 0;
    if (label_flag != (-1))
    {
        printf("\nWarnning guideline entry or extern cannot be labeled!");
        while (line[i++])
            if (line[i] == '.')
                break;
    }

    while (line[i++])
    {
        if (flag == 1)
            label[j++] = line[i];

        if (line[i] == ' ')
            flag = 1;

    }
    label[j] = '\0';
    if ((strcmp(label, "\0") == 0))
        return -1;
    return 0;
}

/*
set entry()
gets a label from guideline if there is no label returns an error.
checks for label correctness.
adds an item to the correct linked list(extern\entry).
*/
int set_entry(int label_flag, char* line, char* label, pointer_to_entry* head_of_entry, pointer_to_entry* head_of_extern, int guid_flag, int* pointer_error, int line_counter)
{
    int nothing = 0;
    int label_correctness;
    int is_label = get_label_from_entry_extern(label_flag, line, label);
    if (is_label == -1)
    {
        *pointer_error = 1;
        printf("\nError:line %d entry or extern label definition incorrect", line_counter);
        return -1;
    }

    label_correctness = label_correctness_checker(label, NULL, line_counter, &nothing);/*nothing pointer is assigned to the function so that it wont chande the error
                                                                                       flag, if it fing a match between label insertad to label in the data image*/

    if (label_correctness == 0)
    {
        if (guid_flag == 4)
        {
            add_item_to_entry(head_of_entry);
            set_info_to_entry(*head_of_entry, label, guid_flag, 0);
            return 0;
        }
        if (guid_flag == 5)
        {
            add_item_to_entry(head_of_extern);
            set_info_to_entry(*head_of_extern, label, guid_flag, 0);
            return 0;
        }
    }

    return -1;
}







#include "header.h"


/*File variables*/
FILE* file_pointer1;
fpos_t position;

/*label variables*/
int label_flag;
char label[LABEL_SIZE];
int label_correctness;

/*command/guide check variables*/
int guideline_point_index;
int command_flag = OFF;
int guide_flag = OFF;
int num_of_var_in_guideline;
char command[COMMAND_SIZE];

/*headers of linked list*/
Item* head_of_list = NULL;
Item* head_of_data_list = NULL;
Data* head_of_data = NULL;
Entry* head_of_entry = NULL;
Entry* head_of_extern = NULL;
Bitfield* head_of_bit = NULL;
Entry* external = NULL; /*store the final extern`s including its addresse in code*/
Entry* entry = NULL; /*store the final entry`s including its addresse in code*/

/*General variables*/
char line[LINE_SIZE];
int IC = 100;
int ICF = 0;
int DC = 0;
int IDF = 0;
int DCF = 0;
int line_counter = 0;
int line_is_empty = 0;
int error_flag = OFF;
int file_counter = 1;



int main(int argc, char* argv[])
{
    input_files_correctness(argc, argv);

    while (argv[file_counter])
    {
        /*file heandling:*/
        if ((file_pointer1 = fopen(argv[file_counter], "r")) == NULL)
        {
            printf("\nError: cannot open file number:%d ", file_counter);
            exit(0);
        }

        /*FIRST ROUND*/

        while (feof(file_pointer1) == 0)
        {
            /*Variables reset:*/
            label_flag = OFF;
            guide_flag = OFF;
            command_flag = OFF;

            /*
            get line()
            the function will store as global char* line, the line input from a file
            if the line is empty or contains a comment only, the function will return -1, else return 0.
            */
            line_is_empty = get_line(line, file_pointer1, position,&error_flag,line_counter);
            if (line_is_empty == OFF)
                continue;

            /*
            Check for label()
            function looks for ':' if find one return the index of it in the global char* called line
            if no ':' had been found function will return -1.
            */
            label_flag = check_for_label(line, &error_flag, line_counter);
            if (label_flag != OFF)
            {
                /*
                get label()
                uses the location of ':' from the "check for label" function, the global char* line,
                gets the label from line and stores it in cha* label.
                */
                get_label(line, label, label_flag);
                label_correctness = label_correctness_checker(label, head_of_data, line_counter, &error_flag);
            }
            /*
            Check for point()
            function looks for '.' if find one return the index of it in the global char*  command
            if no '.' had been found function will return -1.
            */

            guideline_point_index = check_for_point(line ,&error_flag, line_counter);
            
            if (guideline_point_index != -1)
            {
                /*
                get guide()
                uses the location of '.' from the "check for point" function, the global char* line,
                gets the guide from line and stores it in cha* command.

                guide check()
                checks if there is amtch between the guide to one of the gudes in the assember guide list
                if ther is amatch will return the location of match in the list(int), else return -1.
                */
                get_guide(line, command, line_counter ,&error_flag);
                guide_flag = guide_check(command, line_counter, &error_flag);
            }
            /*
            get command()
            will operate if ther is no '.' only, the function wiil get the command from input,
            using the line, and label flag that will indicate if ther is a label that need to be passed first.
            the command will be stred inside the global char* command.

            command number check()
            checks if there is amtch between the commad to one of the commands in the assember command list
            if ther is a match will return the location of match in the list(int), else will print an error
            and will return -1.
            */
            if (guideline_point_index == OFF)
            {
                get_command(line, command, label_flag, line_counter, &error_flag);
                command_flag = comand_number_check(command, line_counter, &error_flag);
            }

            /*meanning there is a match between command input and command list*/
            if (command_flag != OFF)
            {
                /*add info to tabel*/
                if (label_flag != -1 && label_correctness == 0)
                {
                    add_item_to_data(&head_of_data);
                    set_info_to_data(head_of_data, label, IC, "Code", command_flag);
                }
                /*add info to code tabel*/
                add_item_to_list(&head_of_list);
                set_info_to_item(head_of_list, line, label, label_flag, command, IC, command_flag);
                set_registers_to_item(head_of_list, command_flag, line_counter, &error_flag);
                set_immed_to_item(head_of_list, command_flag, line_counter, &error_flag);
                if (command_flag == 23)
                    set_jump(head_of_list, line_counter, &error_flag);
                IC += 4;
            }
            /*meanning there is a match between guide input and guide list*/
            if (guide_flag != OFF)
            {
                if (guide_flag < 4) /*match to guide line dh,dw,db,asciz*/
                    total_information_from_guidline(&head_of_data_list, &head_of_data, line, label, label_flag, command, &DC, guide_flag, &error_flag, line_counter);
                else /*match to entry, extern*/
                    set_entry(label_flag, line, label, &head_of_entry, &head_of_extern, guide_flag, &error_flag, line_counter);
            }
            line_counter++;
        }
        ICF = IC;
        DCF = DC;
        
        /*SECOND ROUND*/
        /*
        second round data apdate()
        function that containes multiple functions, that operate on the lists made on the first round
        */
        second_round_data_update(head_of_list, &head_of_data, head_of_entry, head_of_extern, &entry, &external, DCF, ICF, &error_flag);
        
        if (error_flag != 1)/*meanning that there was no error thrwe the whole file scan*/
        {
            transcription(&head_of_bit, head_of_list, head_of_data_list, ICF);
            make_machine_code_files(head_of_bit, head_of_list, head_of_data_list, ICF, DCF, argv[file_counter]);
            make_extern_files(external, argv[file_counter]);
            make_entry_files(entry, argv[file_counter]);
        }
        free_all(head_of_list, head_of_data_list, head_of_data, head_of_entry, head_of_extern, external, entry, head_of_bit);
        file_counter++;
    }
    return 0;
}





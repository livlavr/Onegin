#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "check_expression.h"
#include "struct_text.h"
#include "process_text.h"
#include "sort_and_strcmp.h"
//enum

// static functions
int  output_to_console      (text_parameters* text_and_parameters);
int  output_text_to_file    (const char* filename, const text_parameters* text_and_parameters, const char* mode); //TODO \0
int  free_memory            (text_parameters* text_and_parameters);
/*
flags -o
git sub module
strchr
get opt?
unicode
алгоритм qsort
процессор опций
планирование времени выполнения задач (не контролируешь себя по часам) - коэффициент (t_real / t_exp)
*/

int main()
{
    text_parameters onegin = {};

    const char* filename    = "texts/onegin.txt";
    const char* output_file = "texts/output_to_console.txt";

    init_text(filename, &onegin);

    my_sort            (onegin.text, onegin.number_of_lines, sizeof(strings), my_strcmp);
    output_text_to_file(output_file, &onegin, "w");
    output_to_console  (&onegin);

    my_sort            (onegin.text, onegin.number_of_lines, sizeof(strings), my_reverse_strcmp);
    output_text_to_file(output_file, &onegin, "a");
    output_to_console  (&onegin);

    qsort              (onegin.text, onegin.number_of_lines, sizeof(strings), return_text_to_original);
    output_text_to_file(output_file, &onegin, "a");
    output_to_console  (&onegin);

    free_memory(&onegin);
}

// TODO remove
int output_to_console(text_parameters* text_and_parameters)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    size_t number_of_line = 0;
    char*  line           = NULL;

    for (number_of_line = 0; number_of_line < text_and_parameters->number_of_lines; number_of_line++)
    {
        line = text_and_parameters->text[number_of_line].line;
        for (size_t number_of_char = 0; number_of_char < text_and_parameters->text[number_of_line].length_of_line;
             number_of_char++)
        {
            if (!(line[number_of_char] == '\n' && number_of_char == 0))
            {
                printf("%c", line[number_of_char]);
            }
        }
    }
    printf("\n");

    return 0;
}

// TODO use FILE* parameter (to be able to pass stdout)
// TODO read about stdout and stdin
int  output_text_to_file(const char* filename, const text_parameters* text_and_parameters, const char* mode)
{
    check_expression(filename            != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);
    check_expression(mode                != NULL, POINTER_IS_NULL);

    FILE* file            = fopen(filename, mode);
    size_t number_of_line = 0;
    char*  line           = NULL;

    for (number_of_line = 0; number_of_line < text_and_parameters->number_of_lines; number_of_line++)
    {
        line = text_and_parameters->text[number_of_line].line;
        for (size_t number_of_char = 0; number_of_char < text_and_parameters->text[number_of_line].length_of_line;
             number_of_char++)
        {
            if (!(line[number_of_char] == '\n' && number_of_char == 0))
            {
                putc(line[number_of_char], file); // TODO use something instead of putc
            }
        }
    }
    putc('\n', file);

    fclose(file);

    return 0;
}

int free_memory(text_parameters* text_and_parameters)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    free(text_and_parameters->buffer);
    text_and_parameters->buffer = NULL;

    free(text_and_parameters->text);
    text_and_parameters->text = NULL;

    text_and_parameters->number_of_lines = 0;
    text_and_parameters->size_of_text    = 0;

    return 0;
}


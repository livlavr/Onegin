#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "check_expression.h"
#include "struct_text.h"
#include "process_text.h"
#include "sort_and_strcmp.h"
#include "output.h"
//enum

int  free_memory            (text_parameters* text_and_parameters);
/*
static functions?
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

    const char* onegin_filename = "texts/onegin.txt";
    const char* output_filename = "texts/output_to_console.txt";
    FILE*       output          = fopen(output_filename, "w");

    init_text(onegin_filename, &onegin);

    my_sort            (onegin.text, onegin.number_of_lines, sizeof(strings), my_strcmp);
    output_text_to_file(output, &onegin);
    output_text_to_file(stdout, &onegin);

    my_sort            (onegin.text, onegin.number_of_lines, sizeof(strings), my_reverse_strcmp);
    output_text_to_file(output, &onegin);
    output_text_to_file(stdout, &onegin);

    qsort              (onegin.text, onegin.number_of_lines, sizeof(strings), return_text_to_original);
    output_text_to_file(output, &onegin);
    output_text_to_file(stdout, &onegin);

    fclose(output);

    free_memory(&onegin);
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


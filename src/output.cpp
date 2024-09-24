#include <stdio.h>

#include "check_expression.h"
#include "struct_text.h"
#include "output.h"

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

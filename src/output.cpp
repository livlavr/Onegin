#include <stdio.h>

#include "check_expression.h"
#include "struct_text.h"
#include "output.h"

int  output_text_to_file(FILE* file, const text_parameters* text_and_parameters)
{
    check_expression(file                != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    size_t number_of_line = 0;
    char*  line           = NULL;

    for (number_of_line = 0; number_of_line < text_and_parameters->number_of_lines; number_of_line++)
    {
        line = text_and_parameters->text[number_of_line].line;
        for (size_t number_of_char = 0; number_of_char < text_and_parameters->text[number_of_line].length_of_line;
             number_of_char++)
        {
            if (!((line[number_of_char] == '\n' && number_of_char == 0) || line[number_of_char] == '\0'))
            {
                putc(line[number_of_char], file); // TODO use something instead of putc
            }
        }
    }
    putc('\n', file);

    return 0;
}

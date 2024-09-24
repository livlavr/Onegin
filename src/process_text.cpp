#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "process_text.h"
#include "check_expression.h"
#include "struct_text.h"

int init_text(const char* filename, text_parameters* text_and_parameters)
{
    check_expression(filename            != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    scan_text       (filename, text_and_parameters);
    count_lines     (text_and_parameters);
    init_strings    (text_and_parameters);

    return 0;
}

int scan_text(const char* filename, text_parameters* text_and_parameters) //TODO mn
{
    check_expression(filename            != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    FILE* file      = fopen(filename, "r");
    struct stat buf = {};

    // BUG check fopen result with if (not ASSert)
    if (file == NULL)
    {
        printf("File with %s name doesn't exist", filename);
    }

    int stat_value_check = stat(filename, &buf);

    check_expression(stat_value_check != -1, STAT_ERROR); // BUG verify or asserted

    text_and_parameters->size_of_text = (size_t)buf.st_size + 1;
    text_and_parameters->buffer       = (char*)calloc(text_and_parameters->size_of_text, sizeof(char));

    size_t fread_value_check = fread(text_and_parameters->buffer, sizeof(char),
                                     text_and_parameters->size_of_text, file);

    check_expression(fread_value_check != 0, FILE_READ_ERROR);

    fclose(file);

    return 0;
}

int count_lines(text_parameters* text_and_parameters)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    char* current_char  = text_and_parameters->buffer;

    while (current_char < text_and_parameters->buffer + text_and_parameters->size_of_text)
    {
        if (*current_char == '\n')
        {
            text_and_parameters->number_of_lines++;
        }
        current_char++;
    }
    text_and_parameters->number_of_lines++;

    text_and_parameters->text = (strings*)calloc(text_and_parameters->number_of_lines, sizeof(strings));

    return 0;
}

int init_strings(text_parameters* text_and_parameters)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    size_t number_of_line = 0;
    size_t length_of_line = 0;
    char* current_char    = text_and_parameters->buffer;

    text_and_parameters->text[number_of_line].line = current_char;
    // TODO write your own debug macro (print like "expr: <expr_val>")
    length_of_line++;
    number_of_line++;

    while (number_of_line < text_and_parameters->number_of_lines)
    {

        if (*current_char == '\n')
        {
            text_and_parameters->text[number_of_line].line               = current_char + 1;
            text_and_parameters->text[number_of_line - 1].length_of_line = length_of_line;
            number_of_line++;
            length_of_line = 0;
        }
        current_char++;
        length_of_line++;
    }
    text_and_parameters->text[number_of_line - 1].line           = current_char;
    text_and_parameters->text[number_of_line - 1].length_of_line = length_of_line;

    return 0;
}

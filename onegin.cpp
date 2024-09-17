#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

#include "check_expression.h"

enum ASCII
{
    SPACE  = 32,
    A_CODE = 65,
    B_CODE = 90,
    a_CODE = 97,
    b_CODE = 122
};

struct lines_in_text
{
    char*  line           = NULL;
    size_t length_of_line = 0;
};

struct text_parameters
{
    lines_in_text* text                = NULL;
    char*          buffer              = NULL;
    size_t         number_of_lines     = 0;
    size_t         size_of_text        = 0;
};

typedef int my_strcmp_t (lines_in_text* line1, lines_in_text* line2);

int  swap(lines_in_text* text, size_t number_of_line);
int  skip_spaces_and_punctuation(char* pointer);
int  my_strcmp        (lines_in_text* line1, lines_in_text* line2);
int  my_reverse_strcmp(char* line1, char* line2);
int  sort             (text_parameters* text_and_parameters, my_strcmp_t my_strcmp);
int  free_buffer      (text_parameters* text_and_parameters);
int  count_lines      (text_parameters* text_and_parameters);
int  reverse_sort     (text_parameters* text_and_parameters);
int  init_index_mas   (text_parameters* text_and_parameters);
int  array_of_pointers(text_parameters* text_and_parameters);
int  text_scan(const char* filename, text_parameters* text_and_parameters);
int  text_init(const char* filename, text_parameters* text_and_parameters);
int  data_init(const char* filename, text_parameters* text_and_parameters);
int  output_to_console(text_parameters* text_and_parameters);
int  file_output      (const char* filename, const text_parameters* text_and_parameters,
                       const size_t* type_of_output, const char* mode);

/*
git sub module
custom free with define
symbols are important
custom_sort like qsort parameters

unicode
алгоритм qsort
*/

int main()
{
    text_parameters onegin = {};
    const char* filename = "onegin.txt";
    // const char* output_file = "output_to_console.txt";
    data_init(filename, &onegin);
    sort(&onegin, my_strcmp); //
    output_to_console(&onegin); //

    // reverse_sort(&onegin);


    // file_output(output_file, &onegin, onegin.sorted_text, "w");
    // file_output(output_file, &onegin, onegin.reverse_sorted_text, "a");
    // file_output(output_file, &onegin, onegin.text, "a");

    free_buffer(&onegin);
}

int data_init(const char* filename, text_parameters* text_and_parameters)
{
    check_expression(filename            != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    text_init(filename, text_and_parameters);

    return 0;
}

int text_init(const char* filename, text_parameters* text_and_parameters)
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    text_scan(filename, text_and_parameters);
    count_lines(text_and_parameters);
    text_and_parameters->text = (lines_in_text*)calloc(text_and_parameters->number_of_lines, sizeof(lines_in_text));
    array_of_pointers(text_and_parameters);

    return 0;
}

int text_scan(const char* filename, text_parameters* text_and_parameters) //TODO mn
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    FILE* file = fopen(filename, "r");
    struct stat buf = {};

    check_expression(file != NULL, FILE_OPEN_ERROR);

    int stat_value_check = 0;
    stat_value_check = fstat(fileno(file), &buf);

    check_expression(stat_value_check != -1, FSTAT_ERROR);

    text_and_parameters->size_of_text = (size_t)buf.st_size + 1;
    printf("buf.st_size - %lu\n", (size_t)buf.st_size);
    text_and_parameters->buffer = (char*)calloc(text_and_parameters->size_of_text, sizeof(char));

    size_t fread_value_check = 0;
    fread_value_check = fread(text_and_parameters->buffer, sizeof(char),
                              text_and_parameters->size_of_text, file);

    check_expression(fread_value_check != 0, FILE_READ_ERROR);

    fclose(file);
    printf("last sign - %d\n", text_and_parameters->buffer[text_and_parameters->size_of_text - 1]);
    printf("last last sign - %d\n", text_and_parameters->buffer[text_and_parameters->size_of_text - 2]);
    return 0;
}

int count_lines(text_parameters* text_and_parameters)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    char* current_char = text_and_parameters->buffer;
    while (current_char < text_and_parameters->buffer + text_and_parameters->size_of_text)
    {
        if (*current_char == '\n')
        {
            text_and_parameters->number_of_lines++;
            printf("char - %d\n", *current_char);
        }
        current_char++;
    }
    printf("number of lines - %lu\n", text_and_parameters->number_of_lines);
    return 0;
}

int array_of_pointers(text_parameters* text_and_parameters) //TODO rename
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    size_t number_of_line             = 0;
    size_t length_of_line             = 0;
    char* current_char                = text_and_parameters->buffer;

    text_and_parameters->text[number_of_line].line = current_char;
    printf ("<%c|%d|%lu|%lu>\n", *text_and_parameters->text[number_of_line].line, *text_and_parameters->text[number_of_line].line, number_of_line, length_of_line);
    number_of_line++;
    length_of_line++;

    for (current_char = text_and_parameters->buffer + 1; current_char < (text_and_parameters->buffer + text_and_parameters->size_of_text); current_char++)
    {
        length_of_line++;

        printf ("<%c|%d|%lu|%lu>\n", *current_char, *current_char, number_of_line, length_of_line);

        if (*current_char == '\n')
        {
            text_and_parameters->text[number_of_line].line = current_char + 1;
            text_and_parameters->text[number_of_line - 1].length_of_line = length_of_line;
            printf ("!<%lu>!\n", length_of_line);
            number_of_line++;
            length_of_line = 0;
        }
    }

    text_and_parameters->text[number_of_line - 1].length_of_line = length_of_line;
    printf("!<%lu>!\n", length_of_line);

    return 0;
}

int sort(text_parameters* text_and_parameters, my_strcmp_t my_strcmp)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    int count_changes = 0;
    do
    {
        count_changes = 0;
         for (size_t number_of_line = 0; number_of_line < text_and_parameters->number_of_lines - 1; number_of_line++)
        {
            printf("sort\n");
            printf("<%lu|%lu>\n", number_of_line, number_of_line + 1);
            printf("<%d>\n", (my_strcmp(&text_and_parameters->text[number_of_line], &text_and_parameters->text[number_of_line + 1])));
            if (my_strcmp(&text_and_parameters->text[number_of_line], &text_and_parameters->text[number_of_line + 1]) > 0)
            {
                printf("CHANGED\n");
                swap(text_and_parameters->text, number_of_line);
                count_changes += 1;
            }
        }
    } while (count_changes != 0);
    // printf("%c", text_and_parameters->text[0].line[0]);
    return 0;
}

// int reverse_sort(text_parameters* text_and_parameters)
// {
//     check_expression(text_and_parameters != NULL, POINTER_IS_NULL);
//
//     int count_changes = 0;
//     do
//     {
//         count_changes = 0;
//         for (size_t number_of_line = 0; number_of_line < text_and_parameters->number_of_lines - 1; number_of_line++)
//         {
//             if (my_reverse_strcmp((text_and_parameters->text)[(text_and_parameters->reverse_sorted_text)[number_of_line] + 1],
//                                   (text_and_parameters->text)[(text_and_parameters->reverse_sorted_text)[number_of_line + 1] + 1]) > 0)
//             {
//                 swap((text_and_parameters->reverse_sorted_text), number_of_line);
//                 count_changes += 1;
//             }
//         }
//     } while (count_changes != 0);
//
//     return 0;
// }

int my_strcmp(lines_in_text* line1, lines_in_text* line2)
{
    check_expression(line1 != NULL, POINTER_IS_NULL);
    check_expression(line2 != NULL, POINTER_IS_NULL);

    int  ascii_difference = 0;
    char char1 = 0, char2 = 0;
    char* pointer1        = line1->line;
    char* pointer2        = line2->line;
    size_t length1 = line1->length_of_line;
    size_t length2 = line2->length_of_line;
    size_t number_of_char = 0;

    skip_spaces_and_punctuation(pointer1);
    skip_spaces_and_punctuation(pointer2);

    for (number_of_char = 0; number_of_char < length1 && number_of_char < length2 && ascii_difference == 0; number_of_char++)
    {
        char1 = (char)tolower(*pointer1);
        char2 = (char)tolower(*pointer2);

        ascii_difference += char1 - char2;
        pointer1++;
        pointer2++;
    }

    if (ascii_difference == 0)
    {
        if (length1 < length2)
        {
            return -1;
        }
        else if (length1 > length2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    return ascii_difference;
}

int skip_spaces_and_punctuation(char* pointer)
{
    check_expression(pointer != NULL, POINTER_IS_NULL);

    while (!isalpha(*pointer))
    {
        pointer++;
    }

    return 0;
}

int swap(lines_in_text* text, size_t number_of_line)
{
    check_expression(text != NULL, POINTER_IS_NULL);

    char* char_transit    = text[number_of_line].line;
    size_t size_t_transit = text[number_of_line].length_of_line;
    text[number_of_line].line = text[number_of_line + 1].line;
    text[number_of_line].length_of_line = text[number_of_line + 1].length_of_line;
    text[number_of_line + 1].line = char_transit;
    text[number_of_line + 1].length_of_line = size_t_transit;
    // lines_in_text transit = {};
    // transit                       = text[number_of_line];
    // text[number_of_line]       = text[number_of_line + 1];
    // text[number_of_line + 1] = transit;



    return 0;
}


int my_reverse_strcmp(char* line1, char* line2)
{
    check_expression(line1 != NULL, POINTER_IS_NULL);
    check_expression(line2 != NULL, POINTER_IS_NULL);

    int ascii_difference = 0;
    char* pointer1       = line1 - 2;
    char* pointer2       = line2 - 2;
    char char1           = *pointer1;
    char char2           = *pointer2;

    skip_spaces_and_punctuation(pointer1);
    skip_spaces_and_punctuation(pointer2);

    while (ascii_difference == 0)
    {
        char1 = (char)tolower(*pointer1);
        char2 = (char)tolower(*pointer2);

        if (char1 == '\n' || char1 == EOF)
        {
            return (char2 == '\n' || char2 == EOF) ? 0 : -1;
        }
        if (char2 == '\n' || char2 == EOF)
        {
            return 1;
        }

        ascii_difference += char1 - char2;
        pointer1--;
        pointer2--;
    }

    return ascii_difference;
}

int output_to_console(text_parameters* text_and_parameters)
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    char* current_char = NULL;
    for (size_t number_of_line = 0; number_of_line < text_and_parameters->number_of_lines; number_of_line++)
    {
        current_char = text_and_parameters->text[number_of_line].line;
        for (size_t number_of_current_char = 0; number_of_current_char < text_and_parameters->text[number_of_line].length_of_line; number_of_current_char++)
        {
            printf("%c", *current_char);
            current_char++;
        }
    }
    printf("\n");

    return 0;
}

// int  file_output(const char* filename, const text_parameters* text_and_parameters, const size_t* type_of_output,
//                  const char* mode)
// {
//     check_expression(type_of_output != NULL, POINTER_IS_NULL);
//     check_expression(text_and_parameters != NULL, POINTER_IS_NULL);
//
//     char* current_char = NULL;
//     char* end_of_line      = NULL;
//     FILE* file             = fopen(filename, mode);
//
//     for (size_t number_of_line = 0; number_of_line < text_and_parameters->number_of_lines; number_of_line++)
//     {
//         current_char = (text_and_parameters->text)[(type_of_output[number_of_line])];
//         end_of_line      = (text_and_parameters->text)[(type_of_output[number_of_line] + 1)];
//         if (*current_char != '\n')
//         {
//             while (current_char < end_of_line)
//             {
//                 putc(*current_char, file);
//                 current_char++;
//             }
//         }
//     }
//     putc('\n', file);
//
//     fclose(file);
//
//     return 0;
// }

int free_buffer(text_parameters* text_and_parameters) //TODO clean pointers
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);
    free(text_and_parameters->buffer);
    text_and_parameters->number_of_lines = 0;
    text_and_parameters->size_of_text    = 0;

    return 0;
}


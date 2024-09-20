#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "check_expression.h"

//enum

struct strings
{
    char*  line           = NULL;
    size_t length_of_line = 0;
};

struct text_parameters
{
    strings*       text                = NULL;
    char*          buffer              = NULL;
    size_t         number_of_lines     = 0;
    size_t         size_of_text        = 0;
};

typedef int my_strcmp_t(const void* line_ptr1, const void* line_ptr2);

// TODO read about static functions
int  init_text              (const char* filename, text_parameters* text_and_parameters);
int  scan_text              (const char* filename, text_parameters* text_and_parameters);
int  count_lines            (text_parameters* text_and_parameters);
int  init_strings           (text_parameters* text_and_parameters);
int  my_sort                (void* ptr, size_t count, size_t size, my_strcmp_t my_strcmp);
int  return_text_to_original(const void* line_ptr1, const void* line_ptr2);
int  my_strcmp              (const void* line_ptr1, const void* line_ptr2);
int  my_reverse_strcmp      (const void* line_ptr1, const void* line_ptr2);
int  swap                   (void* ptr1, void* ptr2, size_t size);
int  output_to_console      (text_parameters* text_and_parameters);
int  output_text_to_file    (const char* filename, const text_parameters* text_and_parameters, const char* mode); //TODO \0
int  free_memory            (text_parameters* text_and_parameters);
int  skip_spaces_and_punctuation(char** pointer, int direction);

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

// TODO pass elem size
int my_sort(void* ptr, size_t count, size_t size, my_strcmp_t my_strcmp) //qsort parameters
{
    check_expression(ptr != NULL, POINTER_IS_NULL);

    strings* text     = (strings*)ptr;
    int count_changes = 0;
    do
    {
        count_changes = 0;

        for (size_t number_of_line = 0; number_of_line < count - 2; number_of_line++)
        {
            if (my_strcmp(&text[number_of_line], &text[number_of_line + 1]) > 0)
            {
                swap(&text[number_of_line], &text[number_of_line + 1], size); //blat
                count_changes += 1;
            }
        }
    } while (count_changes != 0);

    return 0;
}

int return_text_to_original(const void* line_ptr1, const void* line_ptr2)
{
    check_expression(line_ptr1 != NULL, POINTER_IS_NULL);
    check_expression(line_ptr2 != NULL, POINTER_IS_NULL);

    const strings* line1 = (const strings*)line_ptr1;
    const strings* line2 = (const strings*)line_ptr2;
    return (int)(line1->line - line2->line);
}

int my_strcmp(const void* line_ptr1, const void* line_ptr2)
{
    check_expression(line_ptr1 != NULL, POINTER_IS_NULL);
    check_expression(line_ptr2 != NULL, POINTER_IS_NULL);

    int  ascii_difference = 0;
    char char1 = 0, char2 = 0;
    const strings* line1 = (const strings*)line_ptr1;
    const strings* line2 = (const strings*)line_ptr2;
    char* pointer1        = line1->line;
    char* pointer2        = line2->line;
    size_t length1        = line1->length_of_line;
    size_t length2        = line2->length_of_line;
    size_t number_of_char = 0;

    skip_spaces_and_punctuation(&pointer1, 1);
    skip_spaces_and_punctuation(&pointer2, 1);

    for (number_of_char = 0; number_of_char < length1 && number_of_char < length2 && ascii_difference == 0;
         number_of_char++)
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

// TODO read about inline functions
int skip_spaces_and_punctuation(char** pointer, int direction) //TODO change len
{
    check_expression(pointer != NULL, POINTER_IS_NULL);


    while (!(isalpha(**pointer) || **pointer == '\t'))
    {
        *pointer += direction;
    }
    return 0;
}

int swap(void* ptr1, void* ptr2, size_t size)
{
    check_expression(ptr1 != NULL, POINTER_IS_NULL);
    check_expression(ptr2 != NULL, POINTER_IS_NULL);

    char* __ptr1        = (char*)ptr1;
    char* __ptr2        = (char*)ptr2;

    char __transit_ptr = 0;

    for (size_t number_of_ptr = 0; number_of_ptr < size; number_of_ptr++)
    {
        __transit_ptr = __ptr1[number_of_ptr];
        __ptr1[number_of_ptr] = __ptr2[number_of_ptr];
        __ptr2[number_of_ptr] = __transit_ptr;
    }

    return 0;
}

int my_reverse_strcmp(const void* line_ptr1, const void* line_ptr2)
{
    check_expression(line_ptr1 != NULL, POINTER_IS_NULL);
    check_expression(line_ptr2 != NULL, POINTER_IS_NULL);

    const strings* line1  = (const strings*)line_ptr1;
    const strings* line2  = (const strings*)line_ptr2;
    int ascii_difference  = 0;
    char* pointer1        = line1->line + line1->length_of_line - 1;
    char* pointer2        = line2->line + line2->length_of_line - 1;
    char char1            = *pointer1;
    char char2            = *pointer2;
    size_t number_of_char = 0;
    size_t length1 = line1->length_of_line;
    size_t length2 = line2->length_of_line;

    skip_spaces_and_punctuation(&pointer1, -1);
    skip_spaces_and_punctuation(&pointer2, -1);

    for (number_of_char = 0; number_of_char < length1 && number_of_char < length2 &&
         ascii_difference == 0; number_of_char++)
    {
        char1 = (char)tolower(*pointer1);
        char2 = (char)tolower(*pointer2);

        ascii_difference += char1 - char2;
        pointer1--;
        pointer2--;
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


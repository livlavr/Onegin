#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

#include "check_expression.h"

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

typedef int my_strcmp_t(void* line_ptr1, void* line_ptr2);

int  text_init         (const char* filename, text_parameters* text_and_parameters);
int  text_scan         (const char* filename, text_parameters* text_and_parameters);
int  count_lines       (text_parameters* text_and_parameters);
int  array_of_pointers (text_parameters* text_and_parameters);
int  my_sort           (void* ptr, size_t count, my_strcmp_t my_strcmp);
int  return_to_original(void* line_ptr1, void* line_ptr2);
int  my_strcmp         (void* line_ptr1, void* line_ptr2);
int  my_reverse_strcmp (void* line_ptr1, void* line_ptr2);
int  swap              (void* text_ptr, size_t number_of_line);
int  output_to_console (text_parameters* text_and_parameters);
int  file_output       (const char* filename, const text_parameters* text_and_parameters, const char* mode); //TODO \0???
int  free_buffer       (text_parameters* text_and_parameters);
int  skip_spaces_and_punctuation(char** pointer, int direction);

/*
flags -o
git sub module
strchr
get opt?
unicode
алгоритм qsort
процессор опций
*/

int main()
{
    text_parameters onegin = {};

    const char* filename = "texts/onegin.txt";
    const char* output_file = "texts/output_to_console.txt";

    text_init(filename, &onegin);

    my_sort    (onegin.text, onegin.number_of_lines, my_strcmp);
    file_output(output_file, &onegin, "w");
    output_to_console(&onegin);

    my_sort    (onegin.text, onegin.number_of_lines, my_reverse_strcmp);
    file_output(output_file, &onegin, "a");
    output_to_console(&onegin);

    my_sort    (onegin.text, onegin.number_of_lines, return_to_original);
    file_output(output_file, &onegin, "a");
    output_to_console(&onegin);

    free_buffer(&onegin);
}

int text_init(const char* filename, text_parameters* text_and_parameters)
{
    check_expression (filename            != NULL, POINTER_IS_NULL);
    check_expression (text_and_parameters != NULL, POINTER_IS_NULL);

    text_scan        (filename, text_and_parameters);
    count_lines      (text_and_parameters);
    array_of_pointers(text_and_parameters);

    return 0;
}

int text_scan(const char* filename, text_parameters* text_and_parameters) //TODO mn
{
    check_expression(filename            != NULL, POINTER_IS_NULL);
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    FILE* file      = fopen(filename, "r");
    struct stat buf = {};

    check_expression(file != NULL, FILE_OPEN_ERROR);

    int stat_value_check = 0;
    stat_value_check     = fstat(fileno(file), &buf);

    check_expression(stat_value_check != -1, FSTAT_ERROR);

    text_and_parameters->size_of_text = (size_t)buf.st_size + 1;
    // printf("buf.st_size - %lu\n", (size_t)buf.st_size);
    text_and_parameters->buffer       = (char*)calloc(text_and_parameters->size_of_text, sizeof(char));

    size_t fread_value_check = 0;
    fread_value_check        = fread(text_and_parameters->buffer, sizeof(char),
                                     text_and_parameters->size_of_text, file);

    check_expression(fread_value_check != 0, FILE_READ_ERROR);

    fclose(file);
    // printf("last sign - %d\n", text_and_parameters->buffer[text_and_parameters->size_of_text - 1]);
    // printf("last last sign - %d\n", text_and_parameters->buffer[text_and_parameters->size_of_text - 2]);

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
            // printf("char - %d\n", *current_char);
        }
        current_char++;
    }
    text_and_parameters->number_of_lines++;

    text_and_parameters->text = (strings*)calloc(text_and_parameters->number_of_lines, sizeof(strings));
    // printf("number of lines - %lu\n", text_and_parameters->number_of_lines);

    return 0;
}

int array_of_pointers(text_parameters* text_and_parameters) //TODO rename
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    size_t number_of_line = 0;
    size_t length_of_line = 0;
    char* current_char    = text_and_parameters->buffer;

    text_and_parameters->text[number_of_line].line = current_char;
    // printf ("<%c|%d|%lu|%lu>\n", *text_and_parameters->text[number_of_line].line, *text_and_parameters->text[number_of_line].line, number_of_line, length_of_line);
    length_of_line++;
    number_of_line++;
    // printf("%c\t", text_and_parameters->text[number_of_line - 1].line[0]);

    while (number_of_line < text_and_parameters->number_of_lines)
    {
        // printf("%lu\n", number_of_line);
        // printf ("<%c|%d|%lu|%lu>\n", *current_char, *current_char, number_of_line, length_of_line);

        if (*current_char == '\n')
        {
            text_and_parameters->text[number_of_line].line               = current_char + 1;
            text_and_parameters->text[number_of_line - 1].length_of_line = length_of_line;
            // printf("%lu\n", text_and_parameters->text[number_of_line - 1].length_of_line);
            // printf("%c\t", *text_and_parameters->text[number_of_line].line);
            number_of_line++;
            length_of_line = 0;
            // printf ("!<%lu>!\n", length_of_line);
        }
        current_char++;
        length_of_line++;
    }
    // printf("%lu\n", number_of_line);
    text_and_parameters->text[number_of_line - 1].line           = current_char;
    text_and_parameters->text[number_of_line - 1].length_of_line = length_of_line;
    // printf("%c\t", *text_and_parameters->text[number_of_line].line);
    // printf("%lu\n", text_and_parameters->text[number_of_line - 1].length_of_line);
    // printf("<%lu>\n", text_and_parameters->number_of_lines);
    // printf("!<%lu>!\n", length_of_line);

    return 0;
}

int my_sort(void* ptr, size_t count, my_strcmp_t my_strcmp) //qsort parameters
{
    check_expression(ptr != NULL, POINTER_IS_NULL);
    // printf("%d", *text_and_parameters->text[text_and_parameters->number_of_lines - 1].line);
    // printf("%lu", text_and_parameters->number_of_lines);

    strings* text     = (strings*)ptr;
    int count_changes = 0;
    do
    {
        count_changes = 0;

        for (size_t number_of_line = 0; number_of_line < count - 2; number_of_line++)
        {
            // printf("sort\n");
            // printf("<%lu|%lu>\n", number_of_line, number_of_line + 1);
            // printf("<%d>\n", (my_strcmp(&text_and_parameters->text[number_of_line], &text_and_parameters->text[number_of_line + 1])));
            // printf("!<%c|%c>!\n", text[number_of_line], text[number_of_line + 1]);
            if (my_strcmp(&text[number_of_line], &text[number_of_line + 1]) > 0)
            {
                // printf("CHANGED\n");
                swap(text, number_of_line);
                count_changes += 1;
            }
        }
    } while (count_changes != 0);
    // printf("%c", text_and_parameters->text[0].line[0]);

    return 0;
}

int return_to_original(void* line_ptr1, void* line_ptr2)
{
    check_expression(line_ptr1 != NULL, POINTER_IS_NULL);
    check_expression(line_ptr2 != NULL, POINTER_IS_NULL);
    // printf("%d", (line1->line - line2->line));

    strings* line1 = (strings*)line_ptr1;
    strings* line2 = (strings*)line_ptr2;
    return (int)(line1->line - line2->line);
}

int my_strcmp(void* line_ptr1, void* line_ptr2) //const?
{
    check_expression(line_ptr1 != NULL, POINTER_IS_NULL);
    check_expression(line_ptr2 != NULL, POINTER_IS_NULL);
    // printf("my_sort");
    int  ascii_difference = 0;
    char char1 = 0, char2 = 0;
    strings* line1        = (strings*)line_ptr1;
    strings* line2        = (strings*)line_ptr2;
    char* pointer1        = line1->line;
    char* pointer2        = line2->line;
    size_t length1        = line1->length_of_line;
    size_t length2        = line2->length_of_line;
    size_t number_of_char = 0;
    // printf("before <%c|%c>\n", *pointer1, *pointer2); //test

    skip_spaces_and_punctuation(&pointer1, 1); //TODO wtf nihua ne ponyal
    skip_spaces_and_punctuation(&pointer2, 1);
    // printf("after <%c|%c>\n", *pointer1, *pointer2); //test

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

int skip_spaces_and_punctuation(char** pointer, int direction) //TODO change len
{
    check_expression(pointer != NULL, POINTER_IS_NULL);

    // printf("pointer_before - %c\n", *pointer); //test

    while (!(isalpha(**pointer) || **pointer == '\t'))
    {
        // printf("%c>>", *pointer); //test
        *pointer += direction;
        // printf("%c\n", *pointer); //test
    }
    // printf("pointer_after - %c\n", *pointer); //test
    return 0;
}

int swap(void* text_ptr, size_t number_of_line)
{
    check_expression(text_ptr != NULL, POINTER_IS_NULL);

    strings* text = (strings*)text_ptr;
    char* char_transit                      = text[number_of_line].line;
    size_t size_t_transit                   = text[number_of_line].length_of_line;

    text[number_of_line].line               = text[number_of_line + 1].line;
    text[number_of_line].length_of_line     = text[number_of_line + 1].length_of_line;

    text[number_of_line + 1].line           = char_transit;
    text[number_of_line + 1].length_of_line = size_t_transit;

    return 0;
}


int my_reverse_strcmp(void* line_ptr1, void* line_ptr2) //TODO rename
{
    check_expression(line_ptr1 != NULL, POINTER_IS_NULL);
    check_expression(line_ptr2 != NULL, POINTER_IS_NULL);

    strings* line1 = (strings*)line_ptr1;
    strings* line2 = (strings*)line_ptr2;
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
        // printf("<%c|%c>\n", *pointer1, *pointer2);
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
            // printf("<%c|%d|%lu|%lu>\n", line[number_of_char], line[number_of_char], text_and_parameters->text[number_of_line].length_of_line, number_of_line);
        }
    }
    printf("\n");

    return 0;
}

int  file_output(const char* filename, const text_parameters* text_and_parameters, const char* mode)
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
                putc(line[number_of_char], file);
            }
            // printf("<%c|%d|%lu|%lu>\n", line[number_of_char], line[number_of_char], text_and_parameters->text[number_of_line].length_of_line, number_of_line);
        }
    }
    putc('\n', file);

    fclose(file);

    return 0;
}

int free_buffer(text_parameters* text_and_parameters) //TODO clean pointers
{
    check_expression(text_and_parameters != NULL, POINTER_IS_NULL);

    free(text_and_parameters->buffer);
    text_and_parameters->buffer = NULL;

    free(text_and_parameters->text);

    text_and_parameters->number_of_lines = 0;
    text_and_parameters->size_of_text    = 0;

    return 0;
}


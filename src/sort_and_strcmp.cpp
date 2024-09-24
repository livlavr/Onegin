#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "sort_and_strcmp.h"
#include "check_expression.h"
#include "struct_text.h"

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

// TODO read about inline functions (why not static?)
inline int skip_spaces_and_punctuation(char** pointer, int direction) //TODO change len
{
    check_expression(pointer != NULL, POINTER_IS_NULL);


    while (!(isalpha(**pointer) || **pointer == '\t'))
    {
        *pointer += direction;
    }
    return 0;
}

inline int swap(void* ptr1, void* ptr2, size_t size)
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


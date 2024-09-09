#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>

#include "check_expression.h"

enum ASKI
{
    SPACE  = 32,
    A_CODE = 65,
    B_CODE = 90,
    a_CODE = 97,
    b_CODE = 122
};

struct data
{
    size_t  size_of_file    = 0;
    size_t  number_of_lines = 0;
    size_t* sorted          = NULL;
    size_t* rsorted         = NULL;
    size_t* original        = NULL;
    char**  line            = NULL;
    char*   text            = NULL;
} onegin;
//why warning when i do onegin -> struct_element in function declaration?
//how free remember calloced memory if ptr1 = calloc; [ptr1........ptr2.........ptr3.....]

bool isLetter(int a);
bool isUppercase(int a);
int  sort(data* struct_element);
int  rsort(data* struct_element);
int  swap(size_t* mas, size_t index);
int  free_data(data* struct_element);
int  count_lines(data* struct_element);
int  my_strcmp(char* line1, char* line2);
int  init_index_mas(data* struct_element);
int  sort_and_rsort(data* struct_element);
int  index_array_init(data* struct_element);
int  array_of_pointers(data* struct_element);
int  my_reverse_strcmp(char* line1, char* line2);
int  text_scan(const char* filename, data* struct_element);
int  text_init(const char* filename, data* struct_element);
int  data_init(const char* filename, data* struct_element);
int  output(const data* struct_element, const size_t* type_of_output);
int  file_output(const char* filename, const data* struct_element,
                 const size_t* type_of_output, const char* mode);

/*
unicode
mmap?
алгоритм qsort
Бредогенератор :)
Обмен через буквенную переменную и указатель ull
Можно дополнять массивы до %8 = 0
*/

int main()
{
    const char* filename = "onegin.txt";
    const char* output_file = "output.txt";
    data_init(filename, &onegin);
    init_index_mas(&onegin);

    sort_and_rsort(&onegin);

    file_output(output_file, &onegin, onegin.sorted, "w");
    file_output(output_file, &onegin, onegin.rsorted, "a");
    file_output(output_file, &onegin, onegin.original, "a");

    free_data(&onegin);
}

bool isLetter(int a)
{
    return a == SPACE || (a >= A_CODE && a <= B_CODE) || (a >= a_CODE && a <= b_CODE);
}

bool isUppercase(int a)
{
    return a >= A_CODE && a <= B_CODE;
}

int text_scan(const char* filename, data* struct_element)
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    FILE* file = fopen(filename, "r");
    struct stat buf;
    fstat(fileno(file), &buf);
    struct_element->size_of_file = (size_t)buf.st_size + 1; // +1 - for optional '\n'
    struct_element->text = (char*)calloc(struct_element->size_of_file, sizeof(char));
    fread(struct_element->text, sizeof(char), struct_element->size_of_file, file);
    fclose(file);
    if (struct_element->text[struct_element->size_of_file - 3] != '\n')
    {
        struct_element->text[struct_element->size_of_file - 2] = '\n';
    }

    return 0;
}

int array_of_pointers(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* ptr = struct_element->text;
    struct_element->line  = (char**)calloc(struct_element->number_of_lines, sizeof(char**));
    *struct_element->line = struct_element->text;
    size_t i = 1;
    while (ptr < struct_element->text + struct_element->size_of_file)
    {
        ptr++;
        if (*ptr == '\n')
        {
            struct_element->line[i] = ptr + 1;
            i++;
        }
    }

    return 0;
}

int text_init(const char* filename, data* struct_element)
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    text_scan(filename, struct_element);
    array_of_pointers(struct_element);

    return 0;
}

int count_lines(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* ptr = struct_element->text;
    while (ptr < struct_element->text + struct_element->size_of_file)
    {
        if (*ptr == '\n')
        {
            struct_element->number_of_lines++;
        }
        ptr++;
    }
    ptr = struct_element->text;

    return 0;
}

int index_array_init(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    struct_element->sorted   = (size_t*)calloc(struct_element->number_of_lines * 3, sizeof(size_t*));
    struct_element->rsorted  = struct_element->sorted  + struct_element->number_of_lines;
    struct_element->original = struct_element->rsorted + struct_element->number_of_lines;

    return 0;
}

int data_init(const char* filename, data* struct_element)
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    text_init(filename, struct_element);
    count_lines(struct_element);
    index_array_init(struct_element);

    return 0;
}

int my_strcmp(char* line1, char* line2)
{
    check_expression(line1 != NULL, POINTER_IS_NULL);
    check_expression(line2 != NULL, POINTER_IS_NULL);

    int difference = 0;
    char asimb = 0, bsimb = 0;
    char* a = line1;
    char* b = line2;
    while (difference == 0)
    {
        asimb = *a;
        bsimb = *b;
        if (isUppercase(asimb))
        {
            asimb += 32;
        }
        if (isUppercase(bsimb))
        {
            bsimb += 32;
        }
        if (asimb == '\n' || asimb == EOF)
        {
            return (bsimb == '\n' || bsimb == EOF) ? 0 : -1;
        }
        if (bsimb == '\n' || bsimb == EOF)
        {
            return 1;
        }
        if (isLetter(asimb))
        {
            if (isLetter(bsimb))
            {
                        difference += asimb - bsimb;
                        a++;
                        b++;
            }
            else
            {
                b++;
            }
        }
        else
        {
            a++;
        }
    }

    return difference;
}

int my_reverse_strcmp(char* line1, char* line2) //TODO const?
{
    check_expression(line1 != NULL, POINTER_IS_NULL);
    check_expression(line2 != NULL, POINTER_IS_NULL);

    int difference = 0;
    char* a = line1 - 2;
    char* b = line2 - 2;
    char asimb = *a;
    char bsimb = *b;
    while (difference == 0)
    {
        asimb = *a;
        bsimb = *b;
        if (isUppercase(asimb))
        {
            asimb += 32;
        }
        if (isUppercase(bsimb))
        {
            bsimb += 32;
        }
        if (asimb == '\n' || asimb == EOF)
        {
            return (bsimb == '\n' || bsimb == EOF) ? 0 : -1;
        }
        if (bsimb == '\n' || bsimb == EOF)
        {
            return 1;
        }
        if (isLetter(asimb))
        {
            if (isLetter(bsimb))
            {
                        difference += asimb - bsimb;
                        a--;
                        b--;
            }
            else
            {
                b--;
            }
        }
        else
        {
            a--;
        }
    }

    return difference;
    return 0;
}

int swap(size_t* mas, size_t index)
{
    check_expression(mas != NULL, POINTER_IS_NULL);

    size_t transit = 0;
    transit        = mas[index];
    mas[index]     = mas[index + 1];
    mas[index + 1] = transit;

    return 0;
}

int sort(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    int count_changes = 0;
    do
    {
        count_changes = 0;
        for (size_t i = 0; i < struct_element->number_of_lines - 1; i++)
        {
            if (my_strcmp((struct_element->line)[(struct_element->sorted)[i]],
                          (struct_element->line)[(struct_element->sorted)[i + 1]]) > 0)
            {
                swap((struct_element->sorted), i);
                count_changes += 1;
            }
        }
    } while (count_changes != 0);

    return 0;
}

int rsort(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    int count_changes = 0;
    do
    {
        count_changes = 0;
        for (size_t i = 0; i < struct_element->number_of_lines - 1; i++)
        {
            if (my_reverse_strcmp((struct_element->line)[(struct_element->rsorted)[i] + 1],
                                  (struct_element->line)[(struct_element->rsorted)[i + 1] + 1]) > 0)
            {
                swap((struct_element->rsorted), i);
                count_changes += 1;
            }
        }
    } while (count_changes != 0);

    return 0;
}

int output(const data* struct_element, const size_t* type_of_output)
{
    check_expression(type_of_output != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* ptr         = NULL;
    char* end_of_line = NULL;
    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        ptr         = (struct_element->line)[(type_of_output[i])];
        end_of_line = (struct_element->line)[(type_of_output[i] + 1)];
        if (*ptr != '\n')
        {
            while (ptr < end_of_line)
            {
                printf("%c", *ptr);
                ptr++;
            }
        }
    }
    printf("\n");

    return 0;
}

int  file_output(const char* filename, const data* struct_element, const size_t* type_of_output,
                 const char* mode)
{
    check_expression(type_of_output != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* ptr         = NULL;
    char* end_of_line = NULL;
    FILE* file = fopen(filename, mode);

    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        ptr         = (struct_element->line)[(type_of_output[i])];
        end_of_line = (struct_element->line)[(type_of_output[i] + 1)];
        if (*ptr != '\n')
        {
            while (ptr < end_of_line)
            {
                putc(*ptr, file);
                ptr++;
            }
        }
    }
    putc('\n', file);

    fclose(file);

    return 0;
}

int init_index_mas(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        (struct_element->original)[i] = i;
        (struct_element->sorted  )[i] = i;
        (struct_element->rsorted )[i] = i;
    }

    return 0;
}

int sort_and_rsort(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    sort (struct_element);
    rsort(struct_element);

    return 0;
}

int free_data(data* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    free(struct_element->text);
    free(struct_element->line);
    free(struct_element->sorted);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "check_expression.h"

enum ASCII
{
    SPACE  = 32,
    A_CODE = 65,
    B_CODE = 90,
    a_CODE = 97,
    b_CODE = 122
};

struct text_parameters
{
    size_t  number_of_lines     = 0;
    size_t  size_of_file        = 0;
    char*   text                = NULL;
    char**  line                = NULL;
    size_t* sorted_text         = NULL;
    size_t* reverse_sorted_text = NULL;
    size_t* original_text       = NULL;
};

bool is_letter(int a);
bool is_uppercase(int a);
int  swap(size_t* mas, size_t index);
int  my_strcmp        (char* line1, char* line2);
int  my_reverse_strcmp(char* line1, char* line2);
int  sort             (text_parameters* struct_element);
int  free_buffer      (text_parameters* struct_element);
int  count_lines      (text_parameters* struct_element);
int  reverse_sort     (text_parameters* struct_element);
int  init_index_mas   (text_parameters* struct_element);
int  index_array_init (text_parameters* struct_element);
int  array_of_pointers(text_parameters* struct_element);
int  text_scan(const char* filename, text_parameters* struct_element);
int  text_init(const char* filename, text_parameters* struct_element);
int  data_init(const char* filename, text_parameters* struct_element);
int  output_to_console(const text_parameters* struct_element, const size_t* type_of_output);
int  file_output(const char* filename, const text_parameters* struct_element,
                 const size_t* type_of_output, const char* mode);

/*
delete mas index -> change pointers array + struct for line
make three callocs
git sub module
del sort and rsort
more printf
custom free
values of file_functions
check filename
fstat -> stat

unicode
mmap?
алгоритм qsort
Бредогенератор :)
Обмен через буквенную переменную и указатель ull
Можно дополнять массивы до %8 = 0
умный scanf
*/

int main()
{
    text_parameters onegin = {};
    const char* filename = "onegin.txt";
    // const char* output_file = "output_to_console.txt";
    data_init(filename, &onegin);
    init_index_mas(&onegin);

    sort(&onegin);
    reverse_sort(&onegin);

    output_to_console(&onegin, onegin.sorted_text);
    output_to_console(&onegin, onegin.original_text);
    output_to_console(&onegin, onegin.reverse_sorted_text);

    // file_output(output_file, &onegin, onegin.sorted_text, "w");
    // file_output(output_file, &onegin, onegin.reverse_sorted_text, "a");
    // file_output(output_file, &onegin, onegin.original_text, "a");

    free_buffer(&onegin);
}

int data_init(const char* filename, text_parameters* struct_element)
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    text_init(filename, struct_element);
    count_lines(struct_element);
    index_array_init(struct_element);

    return 0;
}

int text_init(const char* filename, text_parameters* struct_element)
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    text_scan(filename, struct_element);
    array_of_pointers(struct_element);

    return 0;
}

int text_scan(const char* filename, text_parameters* struct_element) //TODO check indexes and mn
{
    check_expression(filename != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    FILE* file = fopen(filename, "r");
    struct stat buf = {};

    fstat(fileno(file), &buf);

    struct_element->size_of_file = (size_t)buf.st_size;
    struct_element->text = (char*)calloc(struct_element->size_of_file + 1, sizeof(char));

    fread(struct_element->text, sizeof(char), struct_element->size_of_file, file);
    fclose(file);

    if (struct_element->text[struct_element->size_of_file - 3] != '\n')
    {
        struct_element->text[struct_element->size_of_file - 2] = '\n';
    }

    return 0;
}

int array_of_pointers(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* current_char_ptr = struct_element->text;
    struct_element->line   = (char**)calloc(struct_element->number_of_lines, sizeof(char*));
    *struct_element->line  = struct_element->text;
    size_t number_of_line  = 1;
    while (current_char_ptr < struct_element->text + struct_element->size_of_file)
    {
        current_char_ptr++;

        printf ("<%c|%d>", *current_char_ptr, *current_char_ptr);

        if (*current_char_ptr == '\n')
        {
            struct_element->line[number_of_line] = current_char_ptr + 1;
            number_of_line++;
        }
    }

    return 0;
}

int count_lines(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* current_char_ptr = struct_element->text;
    while (current_char_ptr < struct_element->text + struct_element->size_of_file)
    {
        if (*current_char_ptr == '\n')
        {
            struct_element->number_of_lines++;
        }
        current_char_ptr++;
    }
    current_char_ptr = struct_element->text;

    return 0;
}

int index_array_init(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    struct_element->sorted_text          = (size_t*)calloc(struct_element->number_of_lines * 3, sizeof(size_t*));
    struct_element->reverse_sorted_text  = struct_element->sorted_text         + struct_element->number_of_lines;
    struct_element->original_text        = struct_element->reverse_sorted_text + struct_element->number_of_lines;

    return 0;
}

int init_index_mas(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    for (size_t number_of_line = 0; number_of_line < struct_element->number_of_lines; number_of_line++)
    {
        (struct_element->original_text      )[number_of_line] = number_of_line;
        (struct_element->sorted_text        )[number_of_line] = number_of_line;
        (struct_element->reverse_sorted_text)[number_of_line] = number_of_line;
    }

    return 0;
}

int sort(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    int count_changes = 0;
    do
    {
        count_changes = 0;
        for (size_t number_of_line = 0; number_of_line < struct_element->number_of_lines - 1; number_of_line++)
        {
            printf("%lu\n", number_of_line);
            if (my_strcmp((struct_element->line)[(struct_element->sorted_text)[number_of_line]],
                          (struct_element->line)[(struct_element->sorted_text)[number_of_line + 1]]) > 0)
            {
                swap((struct_element->sorted_text), number_of_line);
                count_changes += 1;
            }
        }
    } while (count_changes != 0);

    return 0;
}

int my_strcmp(char* line1, char* line2) //TODO while
{
    check_expression(line1 != NULL, POINTER_IS_NULL);
    check_expression(line2 != NULL, POINTER_IS_NULL);

    printf ("<%s> vs <%s>\n", line1, line2);

    int ascii_difference  = 0;
    char char1 = 0, char2 = 0;
    char* pointer1        = line1;
    char* pointer2        = line2;
    while (ascii_difference == 0)
    {
        char1 = *pointer1;
        char2 = *pointer2;
        if (is_uppercase(char1))
        {
            char1 += 32;
        }
        if (is_uppercase(char2))
        {
            char2 += 32;
        }
        if (char1 == '\n' || char1 == EOF)
        {
            return (char2 == '\n' || char2 == EOF) ? 0 : -1;
        }
        if (char2 == '\n' || char2 == EOF)
        {
            return 1;
        }
        if (is_letter(char1))
        {
            if (is_letter(char2))
            {
                        ascii_difference += char1 - char2;
                        pointer1++;
                        pointer2++;
            }
            else
            {
                pointer2++;
            }
        }
        else
        {
            pointer1++;
        }
    }

    return ascii_difference;
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

int reverse_sort(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    int count_changes = 0;
    do
    {
        count_changes = 0;
        for (size_t number_of_line = 0; number_of_line < struct_element->number_of_lines - 1; number_of_line++)
        {
            if (my_reverse_strcmp((struct_element->line)[(struct_element->reverse_sorted_text)[number_of_line] + 1],
                                  (struct_element->line)[(struct_element->reverse_sorted_text)[number_of_line + 1] + 1]) > 0)
            {
                swap((struct_element->reverse_sorted_text), number_of_line);
                count_changes += 1;
            }
        }
    } while (count_changes != 0);

    return 0;
}

int my_reverse_strcmp(char* line1, char* line2) //TODO const?, mn
{
    check_expression(line1 != NULL, POINTER_IS_NULL);
    check_expression(line2 != NULL, POINTER_IS_NULL);

    int ascii_difference = 0;
    char* pointer1       = line1 - 2;
    char* pointer2       = line2 - 2;
    char char1           = *pointer1;
    char char2           = *pointer2;
    while (ascii_difference == 0)
    {
        char1 = *pointer1;
        char2 = *pointer2;
        if (is_uppercase(char1))
        {
            char1 += 32;
        }
        if (is_uppercase(char2))
        {
            char2 += 32;
        }
        if (char1 == '\n' || char1 == EOF)
        {
            return (char2 == '\n' || char2 == EOF) ? 0 : -1;
        }
        if (char2 == '\n' || char2 == EOF)
        {
            return 1;
        }
        if (is_letter(char1))
        {
            if (is_letter(char2))
            {
                        ascii_difference += char1 - char2;
                        pointer1--;
                        pointer2--;
            }
            else
            {
                pointer2--;
            }
        }
        else
        {
            pointer1--;
        }
    }

    return ascii_difference;
    return 0;
}

int output_to_console(const text_parameters* struct_element, const size_t* type_of_output)
{
    check_expression(type_of_output != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* current_char_ptr = NULL;
    char* end_of_line      = NULL;
    for (size_t number_of_line = 0; number_of_line < struct_element->number_of_lines; number_of_line++)
    {
        current_char_ptr = (struct_element->line)[(type_of_output[number_of_line])];
        end_of_line      = (struct_element->line)[(type_of_output[number_of_line] + 1)];
        if (*current_char_ptr != '\n')
        {
            while (current_char_ptr < end_of_line)
            {
                printf("%c", *current_char_ptr);
                current_char_ptr++;
            }
        }
    }
    printf("\n");

    return 0;
}

int  file_output(const char* filename, const text_parameters* struct_element, const size_t* type_of_output,
                 const char* mode)
{
    check_expression(type_of_output != NULL, POINTER_IS_NULL);
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    char* current_char_ptr = NULL;
    char* end_of_line      = NULL;
    FILE* file             = fopen(filename, mode);

    for (size_t number_of_line = 0; number_of_line < struct_element->number_of_lines; number_of_line++)
    {
        current_char_ptr = (struct_element->line)[(type_of_output[number_of_line])];
        end_of_line      = (struct_element->line)[(type_of_output[number_of_line] + 1)];
        if (*current_char_ptr != '\n')
        {
            while (current_char_ptr < end_of_line)
            {
                putc(*current_char_ptr, file);
                current_char_ptr++;
            }
        }
    }
    putc('\n', file);

    fclose(file);

    return 0;
}

int free_buffer(text_parameters* struct_element)
{
    check_expression(struct_element != NULL, POINTER_IS_NULL);

    free(struct_element->text);
    free(struct_element->line);
    free(struct_element->sorted_text);

    return 0;
}

bool is_letter(int a)
{
    return a == SPACE || (a >= A_CODE && a <= B_CODE) || (a >= a_CODE && a <= b_CODE);
}

bool is_uppercase(int a)
{
    return a >= A_CODE && a <= B_CODE;
}

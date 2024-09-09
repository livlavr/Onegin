#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

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
int  swap(size_t* mas, size_t index);
int  sort(data* struct_element);
int  rsort(data* struct_element);
int  sort_and_output(data* struct_element);
int  my_strcmp(const char* line1, const char* line2);
int  my_reverse_strcmp(const char* line1, const char* line2);
int  output(const data* struct_element, const size_t* type_of_output);
int  text_init(const char* filename, data* struct_element);
int  count_lines(data* struct_element);
int  index_array_init(data* struct_element);
int  data_init(const char* filename, data* struct_element);
int  free_data(data* struct_element);
/*
пустые строки
алгоритм qsort
сортировка с конца
mmap?
asserts
magic numbers
unicode
Бредогенератор :)
Обмен через буквенную переменную и указатель ull
Можно дополнять массивы до %8 = 0
*/

int main()
{
    const char* filename = "oneginEng.txt";
    data_init(filename, &onegin);

    sort_and_output(&onegin);
    output(&onegin, onegin.original);

    free_data(&onegin);
}

bool isLetter(int a)
{
    return a == 32 || (a >= 65 && a <= 90) || (a >= 97 && a <= 122);
}

bool isUppercase(int a)
{
    return a >= 65 && a <= 90;
}

int text_init(const char* filename, data* struct_element) //TODO separate function
{
    FILE* file = fopen(filename, "r");
    struct stat buf;
    fstat(fileno(file), &buf);
    struct_element->size_of_file = (size_t)buf.st_size + 1; // +1 - for optional '\n'
    struct_element->text = (char*)calloc(struct_element->size_of_file, sizeof(char));
    fread(struct_element->text, sizeof(char), struct_element->size_of_file, file);
    // fast but doesn't analyze "\t\n" and " \n" and "\n\n"
    fclose(file);
    if (struct_element->text[struct_element->size_of_file - 3] != '\n')
    {
        struct_element->text[struct_element->size_of_file - 2] = '\n';
    }
    //text scan

    char* ptr = struct_element->text;
    struct_element->line = (char**)calloc(struct_element->number_of_lines, sizeof(char**)); //int* == char** (size)
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
    // - making array of pointers
    return 0;
}

int count_lines(data* struct_element)
{
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
    struct_element->sorted = (size_t*)calloc(struct_element->number_of_lines * 3, sizeof(int*));
    struct_element->rsorted = struct_element->sorted + struct_element->number_of_lines;
    struct_element->original = struct_element->rsorted + struct_element->number_of_lines;
    // - sorted, rsorted and original init
    return 0;
}

int data_init(const char* filename, data* struct_element)
{
    text_init(filename, struct_element);
    count_lines(struct_element);
    index_array_init(struct_element);

    return 0;
}

int my_strcmp(const char* line1, const char* line2)
{
    int difference = 0;
    int a_ind = 0, b_ind = 0, a = 0, b = 0;
    while (difference == 0)
    {
        a = line1[a_ind];
        b = line2[b_ind];
        if (isUppercase(a))
        {
            a += 32;
        }
        if (isUppercase(b))
        {
            b += 32;
        }
        if (a == '\n' || a == EOF)
            return (b == '\n' || b == EOF) ? 0 : -1;
        if (b == '\n' || b == EOF)
            return 1;
        if (isLetter(a))
        {
            if (isLetter(b))
            {
                        difference += a - b;
                        ++a_ind;
                        ++b_ind;
            }
            else
                ++b_ind;
        }
        else
            ++a_ind;
    }
    return difference;
}

/*
WTF?
onegin.cpp:174:15: error: invalid conversion from 'const char*' to 'char*'
  174 |     a = line1 + a_ind;
      |         ~~~~~~^~~~~~~
      |               |
      |               const char*
onegin.cpp:175:15: error: invalid conversion from 'const char*' to 'char*'
  175 |     b = line2 + b_ind;
      |         ~~~~~~^~~~~~~
      |               |
      |               const char*
*/

int my_reverse_strcmp(const char* line1, const char* line2)
{
    int difference = 0, a_ind = -1, b_ind = -1;
    char* a = 0, *b = 0; //TODO WTF?
    a = line1 + a_ind;
    b = line2 + b_ind;
    if (a == '\n')
    {
        --a_ind;
    }
    else if (a == '\0')
    {
        a_ind -= 2;
    }

    if (b == '\n')
    {
        --a_ind;
    }
    else if (b == '\0')
    {
        a_ind -= 2;
    }

    while (difference == 0)
    {
        a = line1 + a_ind;
        b = line2 + b_ind;
        printf("%d %d", a_ind, b_ind);
        if (isUppercase(a))
        {
            a += 32;
        }
        if (isUppercase(b))
        {
            b += 32;
        }
        if (a == '\n' || a == EOF)
            return (b == '\n' || b == EOF) ? 0 : -1;
        if (b == '\n' || b == EOF)
            return 1;
        if (isLetter(a))
        {
            if (isLetter(b))
            {
                        difference += a - b;
                        --a_ind;
                        --b_ind;
            }
            else
                --b_ind;
        }
        else
            --a_ind;
    }
    return difference;
    return 0;
}

int swap(size_t* mas, size_t index)
{
    size_t transit = 0;
    transit = mas[index];
    mas[index] = mas[index + 1];
    mas[index + 1] = transit;

    return 0;
}

int sort(data* struct_element)
{
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
    char* ptr         = NULL;
    char* end_of_line = NULL;
    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        ptr         = (struct_element->line)[(type_of_output[i])];
        end_of_line = (struct_element->line)[(type_of_output[i] + 1)];
        while (ptr < end_of_line)
        {
            printf("%c", *ptr);
            ptr++;
        }
    }
    printf("\n");

    return 0;
}

int sort_and_output(data* struct_element) //TODO make init_index_mas function
{
    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        (struct_element->original)[i] = i;
        (struct_element->sorted)[i]   = i;
        (struct_element->rsorted)[i]  = struct_element->number_of_lines - i;
    }
    sort(struct_element);
    // rsort(struct_element);
    output(struct_element, struct_element->sorted);
    // output(struct_element, struct_element->rsorted);

    return 0;
}

int free_data(data* struct_element)
{
    free(struct_element->text);
    free(struct_element->line);
    free(struct_element->sorted);

    return 0;
}

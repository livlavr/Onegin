#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

struct data
{
    size_t size_of_file = 0;
    size_t number_of_lines = 0;
    char* text = NULL;
    char** line = NULL;
    size_t* original = NULL;
    size_t* sorted = NULL;
    size_t* rsorted = NULL;
} onegin;

bool isLetter(int a);
bool isUppercase(int a);
int  swap(size_t* mas, size_t);
int  sort(data* struct_element);
int  sort_and_output(data* struct_element);
int  my_strcmp(const char* line1, const char* line2);
int  output(const data* struct_element, const size_t* type_of_output);

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
    const char* filename = "onegin.txt";
    FILE* file = fopen(filename, "r");
    struct stat buf;
    fstat(fileno(file), &buf);
    onegin.size_of_file = (size_t)buf.st_size + 1; // +1 - for optional '\n'
    // //test
    // printf("%lld - buf.st_size\n\n", buf.st_size);
    // //
    onegin.text = (char*)calloc(onegin.size_of_file, sizeof(char));
    fread(onegin.text, sizeof(char), onegin.size_of_file, file); // fast but doesn't analyze "\t\n" and " \n" and "\n\n"
    fclose(file);
    if (onegin.text[onegin.size_of_file - 3] != '\n')
    {
        onegin.text[onegin.size_of_file - 2] = '\n';
    }
    // //test
    // printf("%s\n", onegin.text);
    // printf("%d - letter[size - 2]\n\n", onegin.text[onegin.size_of_file - 2]);
    // printf("%d - letter[size - 1]\n\n", onegin.text[onegin.size_of_file - 1]);
    // printf("%d - letter[size]\n\n", onegin.text[onegin.size_of_file]);
    // // - text_init

    char* ptr = onegin.text;
    while (ptr < onegin.text + onegin.size_of_file)
    {
        if (*ptr == '\n')
        {
            onegin.number_of_lines++;
        }
        ptr++;
    }
    ptr = onegin.text;
    // //test
    // printf("%lu - onegin.number_of_lines\n\n", onegin.number_of_lines);
    // // - number_of_lines counting

    //
    onegin.sorted = (size_t*)calloc(onegin.number_of_lines * 3, sizeof(int*));
    onegin.rsorted = onegin.sorted + onegin.number_of_lines;
    onegin.original = onegin.rsorted + onegin.number_of_lines;
    // - sorted, rsorted and original init

    onegin.line = (char**)calloc(onegin.number_of_lines, sizeof(char**)); //int* == char** (size)
    *onegin.line = onegin.text;
    size_t i = 1;
    while (ptr < onegin.text + onegin.size_of_file)
    {
        ptr++;
        if (*ptr == '\n')
        {
            onegin.line[i] = ptr + 1;
            i++;
        }
    }
    // //test
    // for (size_t j = 0; j < onegin.number_of_lines; j++)
    // {
    //     printf("%c - *onegin.line[%lu]\n", *onegin.line[j], j);
    // }
    // printf("\n");
    // // - making array of pointers

    sort_and_output(&onegin);
    output(&onegin, onegin.original);
    // //tests
    // for (int j = 0; j < onegin.number_of_lines; j++)
    // {
    //     printf("%lu - *onegin.sorted[%d]\n", onegin.sorted[j], j);
    // }
    // printf("\n");
    // //

    free(onegin.text);
}

bool isLetter(int a)
{
    return a == 32 || (a >= 65 && a <= 90) || (a >= 97 && a <= 122);
}

bool isUppercase(int a)
{
    return a >= 65 && a <= 90;
}

int my_strcmp(const char* line1, const char* line2)
{
    int difference = 0;
    int i = 0, j = 0, a = 0, b = 0;
    while (difference == 0)
    {
        a = line1[i];
        b = line2[j];
        if (isUppercase(a))
        {
            a = a + 32;
        }
        if (isUppercase(b))
        {
            b = b + 32;
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
                        ++i;
                        ++j;
            }
            else
                ++j;
        }
        else
            ++i;
    }
    return difference;
}

int swap(size_t* mas, size_t i)
{
    size_t transit = 0;
    transit = mas[i];
    mas[i] = mas[i + 1];
    mas[i + 1] = transit;
    return 0;
}

int sort(data* struct_element)
{
    int count_changes = 1;
    while (count_changes != 0)
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
    }
    return 0;
}

int  output(const data* struct_element, const size_t* type_of_output)
{
    char* ptr         = NULL;
    char* end_of_line = NULL;
    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        ptr         = struct_element->line[type_of_output[i]];
        end_of_line = struct_element->line[type_of_output[i] + 1];
        while (ptr < end_of_line)
        {
            printf("%c", *ptr);
            ptr++;
        }
    }
    printf("\n");
    return 0;
}

int sort_and_output(data* struct_element)
{
    for (size_t i = 0; i < struct_element->number_of_lines; i++)
    {
        (struct_element->original)[i] = i;
        (struct_element->sorted)[i]   = i;
        (struct_element->rsorted)[i]  = struct_element->number_of_lines - i;
    }
    sort(struct_element);
    output(struct_element, struct_element->sorted);
    return 0;
}

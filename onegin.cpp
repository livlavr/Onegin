#include <stdio.h>
#include <stdlib.h>

const size_t MAXLEN = 100;
const size_t NUMBER_OF_LINES = 40;

bool isletter(int a);
int  swap(int mas[], size_t);
int  output(int mas[], const char data[][MAXLEN]);
int  sort(int mas[], const char data[][MAXLEN]);
int  sort_and_output(const char data[][MAXLEN]);
int  my_strcmp(const char line1[], const char line2[]);
int  text_init(const char* filename, char data[][MAXLEN]);
/*
сделать массив через указатели (рваный)
доделать /n
пустые строки
алгоритм qsort
сортировка с конца
unicode
Бредогенератор :)
Обмен через буквенную переменную и указатель ull
Можно дополнять массивы до %8 = 0
mmap?
asserts
magic numbers
*/
int main()
{
    const char* filename = "onegin.txt";
    char data[NUMBER_OF_LINES][MAXLEN] = {};
    int mas[NUMBER_OF_LINES] = {};
    for (size_t i = 0; i < NUMBER_OF_LINES; i++)
    {
        mas[i] = i;
    }
    text_init(filename, data);
    sort_and_output(data);
    output(mas, data);
}

bool isletter(int a)
{
    return (a == 32 || (a >= 65 && a <= 90) || (a >= 97 && a <= 122));
}

int text_init(const char * filename, char data[][MAXLEN])
{
    FILE* file = fopen(filename, "r");
    for (size_t i = 0; i < NUMBER_OF_LINES; i++)
    {
        fgets(data[i], MAXLEN, file);
    }
    fclose(file);
    return 0;
}

int my_strcmp(const char line1[], const char line2[])
{
    int difference = 0;
    int i = 0, j = 0, a = 0, b = 0;
    while (difference == 0)
    {
        a = line1[i];
        b = line2[j];
        if (a == '\n' || a == EOF)
            return (b == '\n' || b == EOF) ? 0 : -1;
        if (b == '\n' || b == EOF)
            return 1;
        if (isletter(a))
        {
            if (isletter(b))
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

int swap(int mas[], size_t i)
{
    int transit = 0;
    transit = mas[i];
    mas[i] = mas[i + 1];
    mas[i + 1] = transit;
    return 0;
}

int sort(int mas[], const char data[][MAXLEN])
{
    int count_changes = 1;
    while (count_changes != 0)
    {
        count_changes = 0;
        for (size_t i = 0; i < NUMBER_OF_LINES - 1; i++)
        {
            if (my_strcmp(data[mas[i]], data[mas[i+1]]) > 0)
            {
                swap(mas, i);
                count_changes += 1;
            }
        }
    }
    return 0;
}

int output(int mas[], const char data[][MAXLEN])
{
    for (size_t i = 0; i < NUMBER_OF_LINES; i++)
    {
        printf("%s", data[mas[i]]);
    }
    printf("\n");
    return 0;
}

int sort_and_output(const char data[][MAXLEN])
{
    int mas[NUMBER_OF_LINES] = {};
    for (size_t i = 0; i < NUMBER_OF_LINES; i++)
    {
        mas[i] = i;
    }
    sort(mas, data);
    output(mas, data);
    return 0;
}

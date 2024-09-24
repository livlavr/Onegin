#ifndef STRUCT_TEXT_H_
#define STRUCT_TEXT_H_

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

#endif

#ifndef PROCESS_TEXT_H_
#define PROCESS_TEXT_H_

#include "struct_text.h"

int  init_text              (const char* filename, text_parameters* text_and_parameters);
int  scan_text              (const char* filename, text_parameters* text_and_parameters);
int  count_lines            (text_parameters* text_and_parameters);
int  init_strings           (text_parameters* text_and_parameters);

#endif

#ifndef OUTPUT_H_
#define OUTPUT_H_

int  output_to_console      (text_parameters* text_and_parameters);
int  output_text_to_file    (const char* filename, const text_parameters* text_and_parameters, const char* mode); //TODO \0

#endif

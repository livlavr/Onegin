#ifndef SORT_AND_STRCMP_H_
#define SORT_AND_STRCMP_H_

typedef int my_strcmp_t(const void* line_ptr1, const void* line_ptr2);

int  my_sort                          (void* ptr, size_t count, size_t size, my_strcmp_t my_strcmp);
int  return_text_to_original          (const void* line_ptr1, const void* line_ptr2);
int  my_strcmp                        (const void* line_ptr1, const void* line_ptr2);
int  my_reverse_strcmp                (const void* line_ptr1, const void* line_ptr2);
inline int swap                       (void* ptr1, void* ptr2, size_t size);
inline int skip_spaces_and_punctuation(char** pointer, int direction);


#endif

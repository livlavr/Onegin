#ifndef CHECK_EXPRESSION_H_
#define CHECK_EXPRESSION_H_

enum TYPE_OF_ERROR
{
    POINTER_IS_NULL = 1,
    DOUBLE_IS_NAN = 2
};

#ifndef NDEBUG

    #define check_expression(condition, message) do                              \
    {                                                                            \
        if (!(condition))                                                        \
        {                                                                        \
            printf("ERROR (%s), file: %s, function: %s, line: %d\n", #condition, \
            __FILE__, __FUNCTION__, __LINE__);                                   \
            return message;                                                      \
        }                                                                        \
    } while(0)

#else

    #define check_expression(condition, message)

#endif
#endif

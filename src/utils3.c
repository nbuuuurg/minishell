#include "../include/minishell.h"

int count_subshell(char *s)
{
    int open;
    int i;

    open = 0;
    i = 0;
    while (s[i])
    {
        if (s[i] == '(')
            open++;
        i++;
    }
    return (open);
}
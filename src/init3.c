#include "../include/minishell.h"

int     init_subshell(t_line *line, t_token *subinput)
{
    t_line  *subline;

    subline = dup_line(line, subinput);
    if (!subline)
        return (EX_GEN);
    free_line(subline);
    return (0);
}
#include "../include/minishell.h"

int     init_subshell(t_line *line, t_token *subinput)
{
    t_line  *subline;

    subline = dup_line(line, subinput);
    if (!subline)
        return (EX_GEN);
    subline->last_exit = lexer_input(subline);
    if (subline->last_exit != 0)
        return (free_line(subline), EX_GEN);
    // print_expr(subline);
    if (subline->exprs)// && subline->last_exit != -1)
        exec_minishell(subline);
    // if (subline->last_exit == -1)
    //     printf("non non non\n");
    free_line(subline);
    free(subline);
    return (0);
}
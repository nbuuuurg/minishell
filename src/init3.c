#include "../include/minishell.h"

int     init_subshell(t_line *line, t_token *subinput)
{
    t_line  *subline;

    subline = dup_line(line, subinput);
    if (!subline)
        return (EX_GEN);
    subline->last_exit = lexer_input(subline);
    if (!subline)
        return (EX_GEN);
    // print_expr(subline);
    if (subline->exprs)
        exec_minishell(subline, line->envp) ;
    free_line(subline);
    return (0);
}
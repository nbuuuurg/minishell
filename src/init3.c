#include "../include/minishell.h"

int     init_subshell(t_line *line, t_token *subinput)
{
    t_line  *subline;

    subline = dup_line(line, subinput);
    if (!subline)
        return (EX_GEN);
    subline->last_exit = init_clean_input(subline);
    if (!subline)
        return (EX_GEN);
    subline->last_exit = lexer_input(subline);
    if (!subline)
        return (EX_GEN);
    if (subline->exprs)
        exec_minishell(subline, subline->envp);
    // print_expr(subline);
    free_line(subline);
    return (0);
}
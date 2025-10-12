#include "../include/minishell.h"

char    *new_subinput(t_line *line, t_token *token)
{
    size_t  len;
    char    *subinput;
    size_t i;
    size_t j;
    int     subshell;
    int     subshell2;

    (void)line;
    len = ft_strlen(token->s);
    subinput = ft_calloc(len - 1, 1);
    if (!subinput)
        return (NULL);
    subshell = 0;
    subshell2 = 0;
    i = 0;
    j = 0;
    while (token->s[i] && i < len - 1)
    {
        if ((subshell == 0 && token->s[i] == '(') || (subshell2 == 0 && token->s[len - i - 1] == ')'))
        {
            if (subshell == 0 && token->s[i] == '(')
                subshell++;
            if (subshell2 == 0 && token->s[len - i - 1] == ')')
                subshell2++;
            i++;
        }
        if (token->s[i] && i < len - 1)
            subinput[j++] = token->s[i++];
    }
    // printf("subinput : %s\n", subinput);
    return(subinput);

}

t_line  *dup_line(t_line *line, t_token *subinput)
{
    t_line  *subline;

    subline = ft_calloc(1, sizeof(t_line));
    if (!subline)
        return(line->last_exit = EX_GEN, NULL);
    subline->input = new_subinput(line, subinput);
    if (!subline->clean)
        return (line->last_exit = EX_GEN, NULL);
    subline->envp = line->envp;
    subline->tokens = NULL;
    subline->exprs = NULL;
    subline->last_exit = 0;
    subline->len = line->len - 2;
    subline->lexer_err = 0;
    subline->num_expr = 0;
    subline->path = get_path(line->envp);
    return(subline);
}
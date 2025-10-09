#include "../include/minishell.h"

void	print_token(t_line *line)
{
    while (line->tokens)
    {
        printf("token : %s", line->tokens->s);
        if (line->tokens->type == 0)
            printf("$ - type : WORD$");
        else if (line->tokens->type == 1)
            printf("$ - type : PIPE$");
        else if (line->tokens->type == 2)
            printf("$ - type : OR$");
        else if (line->tokens->type == 3)
            printf("$ - type : AND$");
        else if (line->tokens->type == 4)
            printf("$ - type : REDIR_IN$");
        else if (line->tokens->type == 5)
            printf("$ - type : REDIR_OUT$");
        else if (line->tokens->type == 6)
            printf("$ - type : REDIR_APPEND$");
        else if (line->tokens->type == 7)
            printf("$ - type : HEREDOC$");   
        else if (line->tokens->type == 8)
            printf("$ - type : ASSIGNMENT");
        if (line->tokens->in_subshell != 0)
            printf(" - (%d subshell)", line->tokens->in_subshell);
        if (line->tokens->has_expand != 0)
            printf(" - has %d env var", line->tokens->has_expand);
        if (line->tokens->quoted != NO_QUOTE)
        {
            if (line->tokens->quoted == SINGLE)
                printf(" - SINGLE");
            else if (line->tokens->quoted == DOUBLE)
                printf(" - DOUBLE");
            else
                printf(" - MULTIPLE");
        }
        printf("\n");
        line->tokens = line->tokens->next;
    }
}

void    print_expr(t_line *line)
{
    int     i;
    int     j;
    int     k;


    k = 0;
    while(line->exprs)
    {
        i = 0;
        printf("\nLine %d\n ______________\n|\n", k + 1);
        while(i <= line->exprs->pipe_count)
        {
            printf("|  Pipeline %d (%u)\n|______________\n", i + 1, line->exprs->pipeline[i].position);
            j = 0;
            printf("|\n|    ARGV : [");
            if (line->exprs->pipeline[i].word_count > 0)
            {
                while (j < line->exprs->pipeline[i].word_count)
                {
                    printf("%s", line->exprs->pipeline[i].args[j]);
                    if (j < line->exprs->pipeline[i].word_count - 1)
                        printf(", ");
                    else
                        printf("]");
                    j++;
                }
            }
            j = 0;
            if (line->exprs->pipeline[i].redir_count > 0)
            {
                while (j < line->exprs->pipeline[i].redir_count)
                {
                    printf("\n|    REDIRECTION %d :\n", line->exprs->pipeline[i].redirect[j].order + 1);
                    printf("|     ->From fd : %d\n", line->exprs->pipeline[i].redirect[j].from_fd);
                    printf("|     ->Type of redir : %s\n", line->exprs->pipeline[i].redirect[j].redir);
                    printf("|     ->File attached : %s", line->exprs->pipeline[i].redirect[j].file);
                    j++;
                }
            }
            j = 0;
            if (line->exprs->pipeline[i].assign_count > 0)
            {
                while (j < line->exprs->pipeline[i].assign_count)
                {
                    printf("\n|    ASSIGNEMENT %d : name = %s, value = %s", j + 1, line->exprs->pipeline[i].assign[j].name, line->exprs->pipeline[i].assign[j].value);
                    j++;
                }
            }
            i++;
            if (i <= line->exprs->pipe_count )
                printf("\n|______________\n|\n");
        }
        k++;
        printf("\n|______________\n");
        line->exprs = line->exprs->next;
    }
    line->exprs = NULL;
}

void    print_error(char *s, t_exit code)
{
    (void)s;
    //ft_fprintf
    if (code == EX_GEN)
        ft_printf("minishell: fatal error: malloc failed\n");
    else if (code == EX_USAGE)
        ft_printf("minishell: syntax error: unexpected token\n");
    else if (code == EX_NOEXEC)
        ft_printf("minishell: %s: permission denied\n", s);
    else if (code == EX_NOTFOUND)
        ft_printf("minishell: %s: command not found\n", s);
}
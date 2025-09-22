#include "../include/minishell.h"

void	print_token(t_line *line)
{
    while (line->tokens)
    {
        printf("token : %s", line->tokens->s);
        if (line->tokens->type == 0)
            printf("$ - type : WORD$\n");
        if (line->tokens->type == 1)
            printf("$ - type : PIPE$\n");
        if (line->tokens->type == 2)
            printf("$ - type : OR$\n");
        if (line->tokens->type == 3)
            printf("$ - type : AND$\n");
        if (line->tokens->type == 4)
            printf("$ - type : REDIR_IN$\n");
        if (line->tokens->type == 5)
            printf("$ - type : REDIR_OUT$\n");
        if (line->tokens->type == 6)
            printf("$ - type : REDIR_APPEND$\n");
        if (line->tokens->type == 7)
            printf("$ - type : HEREDOC$\n");   
        if (line->tokens->type == 8)
            printf("$ - type : ASSIGNMENT\n");
        if (line->tokens->in_subshell == 1)
            printf("(in_subshell)\n");
        if (line->tokens->has_env_var == 1)
            printf("has env var\n");
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

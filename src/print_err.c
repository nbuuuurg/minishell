#include "minishell.h"

void    print_err_1(t_token *begin)
{
    if (begin->in_subshell == -3)
        ft_putstr_fd("mini: syntax error near unexpected token `('\n", STDERR_FILENO);
    else if (begin->in_subshell == -1)
        ft_putstr_fd("mini: syntax error near unexpected token `)'\n", STDERR_FILENO);
    else
        ft_putstr_fd("mini: syntax error near unexpected token `)'\n", STDERR_FILENO);
}

void    print_err_2(t_token *begin)
{
    int fd;

    fd = 0;
    if (!begin->next)
    {
        ft_putstr_fd("mini: syntax error near unexpected token `newline'\n", STDERR_FILENO);
    }
    else
    {
        if (begin->next->type != WORD)
        {
            ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
            ft_putstr_fd(begin->next->s, STDERR_FILENO);
            ft_putstr_fd("\'\n", STDERR_FILENO);
        }
        else
        {
            if (begin->type == HEREDOC)
            {
                (void)fd;
            }
        }
    }
}

void    print_err_3(t_token *begin)
{
    if (!begin->next)
    {
        ft_putstr_fd("mini: syntax error near unexpected token `|'\n", STDERR_FILENO);
    }
    else
    {
        if (begin->next->type == AND || begin->next->type == PIPE || begin->next->type == OR)
        {
            ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
            ft_putstr_fd(begin->next->s, STDERR_FILENO);
            ft_putstr_fd("\'\n", STDERR_FILENO);
        }
    }
}

void    print_err_4(t_token *begin)
{
    if (!begin->next)
    {
        ft_putstr_fd("mini: syntax error near unexpected token `||'\n", STDERR_FILENO);
    }
    else
    {
        if (begin->next->type == AND || begin->next->type == PIPE || begin->next->type == OR)
        {
            ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
            ft_putstr_fd(begin->next->s, STDERR_FILENO);
            ft_putstr_fd("\'\n", STDERR_FILENO);
        }
    }
}

void    print_err_5(t_token *begin)
{
    if (!begin->next)
    {
        ft_putstr_fd("mini: syntax error near unexpected token `||'\n", STDERR_FILENO);
    }
    else
    {
        if (begin->next->type == AND || begin->next->type == PIPE || begin->next->type == OR)
        {
            ft_putstr_fd("mini: syntax error near unexpected token `", STDERR_FILENO);
            ft_putstr_fd(begin->next->s, STDERR_FILENO);
            ft_putstr_fd("\'\n", STDERR_FILENO);
        }
    }
}
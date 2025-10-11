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

t_quoted	def_quote(int	multiple_quote, int quote)
{
    
	if (multiple_quote == 2)
	{
		if (quote == 39)
			return (SINGLE);
		else
			return (DOUBLE);
	}
	else	
			return (MULTIPLE);
}	

int     is_something(char c)
{
    if (is_special(c) || is_whitespace(c) || is_quote(c) || is_subshell(c))
        return (1);
    return (0);
}
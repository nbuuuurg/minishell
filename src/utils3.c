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

int		need_expand(char *s)
{
	int		i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (is_dollar(s[i]))
			return (1);
		i++;
	}
	return (0);
}

int		has_wildcards(char *s)
{
	int		i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

char	**ft_strdup2(char **env)
{
	int		i;
	char	**new;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
		i++;
	new = malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new[i] = ft_strdup(env[i]);
		if (!new[i])
			return (free_split(new), NULL);
		i++;
	}
	new[i] = NULL;
	return (new);
}

char	*find_env_var(t_line *line, char *var)
{
	int     i;
	int     len_var;
	char    *the_env;

	if (!line->envp || !var)
		return (NULL);
	len_var = ft_strlen(var);
	i = 0;
	while (line->envp[i])
	{
		if (ft_strncmp(line->envp[i], var, len_var) == 0
			&& line->envp[i][len_var] == '=')
		{
			the_env = line->envp[i] + len_var + 1;
			return (the_env);
		}
		i++;
	}
	return (NULL);
}

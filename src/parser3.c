#include "../include/minishell.h"

char	*new_expanded_content(t_line *line, size_t j, char *s, char *ex_var, size_t old_len)
{
	(void)line;
	char	*new_s;
	size_t		len_ex_var;
	size_t		new_len;
	size_t		len_s;
	size_t			i;
	size_t			l;
	size_t			k;

	if (!s || !ex_var)
		return (NULL);
	len_s = ft_strlen(s);
	if (j > len_s || old_len > len_s - j)
		return (NULL);
	if (*ex_var == 0)
		len_ex_var = 0;
	else
		len_ex_var = ft_strlen(ex_var);
	new_len = len_s + len_ex_var - old_len - 1;
	if (new_len == 1)
    {
        if (len_ex_var == 2 && ft_strncmp(ex_var, "$?", 2) == 0)
            return (ft_strdup(ex_var));
        else
            return (ft_strdup("\n"));
	}
	printf("new_len : %ld\n", new_len);
	new_s = ft_calloc(new_len + 1, 1);
	if (!new_s)
		return (NULL);
	i = 0;
    while (i < j)
    {
        new_s[i] = s[i];
        i++;
    }
    k = 0;
    while (k < len_ex_var)
        new_s[i++] = ex_var[k++];
    l = j + old_len;
    while (l < len_s)
        new_s[i++] = s[l++];
    new_s[new_len - 1] = '\0';
	printf("new_s = %s et s : %s\n", new_s, s);
	return (new_s);
}

char	*expanded_content(char *s, t_line *line)
{
	char	*var;
	char	*ex_var;
	char	*new_s;
	int		i;
	int		j;
	int		len;
	char	*start;

	var = NULL;
	ex_var = NULL;
	new_s = s;
	start = NULL;
	i = 0;
	len = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '$')
		{
			j = i;
			i++;
			if ((is_dollar(s[i]) && len == 0) || (s[i] == '?' && len == 0))
			{
				start = &s[i - 1];
				len++;
			}
			else
				start = &s[i];
			while (s[i] && (ft_isalnum(s[i]) || (is_dollar(s[i]) && len == 1) || (s[i] == '?' && len == 1)))
			{
				i++;
				len++;
			}
			var = ft_calloc(len + 1, 1);
			if (!var)
				return (NULL);
			ft_memcpy(var, start, len);
			ex_var = expanded_var(line, var);
			if (!ex_var)
				return (free(var), NULL);
			new_s = new_expanded_content(line, (size_t)j, s, ex_var, (size_t)len);
			if (!new_s)
				return (free(var), free(ex_var), NULL);
		}
		i++;
	}
	return (new_s);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:54 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/26 04:38:58 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_subshell(int c)
{
	if (c == '(')
		return (c);
	if (c == ')')
		return (c);
	return (0);
}

int	is_assignment(char *s)
{
	int	i;

	if (!(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{		
		if (s[i] == '=')
			return (1);
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (0);
}

char	*dup_assign_name(char *s)
{
	char	*name;
	int		len;
	int		i;

	len = 0;
	while (s[len] && s[len] != '=')
		len++;
	if (len > 0)
		name = ft_calloc(len + 1, 1);
	else
		return (NULL);
	if (!name)
		return (NULL);
	i = -1;
	while (++i < len)
		name[i] = s[i];
	name[i] = 0;
	return (name);
}

char	*dup_assign_value(char *s)
{
	char	*value;
	int		len;
	int		i;
	int		j;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	len = i;
	while (s[len])
		len++;
	if (len > 0)
		value = ft_calloc(len + 1, 1);
	else
		return (NULL);
	if (!value)
		return (NULL);
	j = 0;
	while (j < len)
		value[j++] = s[++i];
	value[j] = 0;
	return (value);
}

int     len_whitespace(t_line *line)
{
    size_t i;
    size_t j;

    i = 0;
    j = 0;
    while (line->input[i] && is_whitespace(line->input[i]))
    {
        i++;
        j++;
    }
    while (line->input[i])
        i++;
    i--;
    while(i > 0 && is_whitespace(line->input[i]))
    {
        i--;
        j++;
    }
    return (j);
}
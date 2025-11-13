/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:56:49 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:56:51 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/**/
/* char	*n_exp_c(t_line *line, size_t j, char *s, char *ex_var, size_t old_len) */
/* { */
/* 	char		*new_s; */
/* 	size_t		len_ex_var; */
/* 	size_t		new_len; */
/* 	size_t		len_s; */
/* 	size_t		i; */
/* 	size_t		l; */
/* 	size_t		k; */
/**/
/* 	(void)line; */
/* 	if (!s || !ex_var) */
/* 		return (NULL); */
/* 	len_s = ft_strlen(s); */
/* 	if (j > len_s || old_len > len_s - j) */
/* 		return (NULL); */
/* 	if (*ex_var == 0) */
/* 		len_ex_var = 0; */
/* 	else */
/* 		len_ex_var = ft_strlen(ex_var); */
/* 	new_len = len_s + len_ex_var - old_len; */
/* 	if (new_len == 1) */
/* 	{ */
/* 		if (len_ex_var == 2 && ft_strncmp(ex_var, "$?", 2) == 0) */
/* 			return (ft_strdup(ex_var)); */
/* 		else */
/* 			return (ft_strdup("\n")); */
/* 	} */
/* 	new_s = ft_calloc(new_len + 1, 1); */
/* 	if (!new_s) */
/* 		return (NULL); */
/* 	i = 0; */
/* 	while (i < j) */
/* 	{ */
/* 		new_s[i] = s[i]; */
/* 		i++; */
/* 	} */
/* 	k = 0; */
/* 	while (k < len_ex_var) */
/* 		new_s[i++] = ex_var[k++]; */
/* 	l = j + old_len; */
/* 	while (l < len_s) */
/* 		new_s[i++] = s[l++]; */
/* 	new_s[new_len - 1] = '\n'; */
/* 	new_s[new_len] = '\0'; */
/* 	return (new_s); */
/* } */
/**/
/* char	*expanded_content(char *s, t_line *line) */
/* { */
/* 	char	*var; */
/* 	char	*ex_var; */
/* 	char	*new_s; */
/* 	int		i; */
/* 	int		j; */
/* 	int		len; */
/* 	char	*start; */
/**/
/* 	var = NULL; */
/* 	ex_var = NULL; */
/* 	new_s = s; */
/* 	start = NULL; */
/* 	i = 0; */
/* 	len = 0; */
/* 	if (!s) */
/* 		return (NULL); */
/* 	while (s[i]) */
/* 	{ */
/* 		if (s[i] == '$') */
/* 		{ */
/* 			j = i; */
/* 			i++; */
/* 			if ((is_dollar(s[i]) && len == 0) || (s[i] == '?' && len == 0)) */
/* 			{ */
/* 				start = &s[i - 1]; */
/* 				len++; */
/* 			} */
/* 			else */
/* 				start = &s[i]; */
/* 			while (s[i] && (ft_isalnum(s[i]) */
/* 					|| (is_dollar(s[i]) && len == 1) */
/* 					|| (s[i] == '?' && len == 1))) */
/* 			{ */
/* 				i++; */
/* 				len++; */
/* 			} */
/* 			var = ft_calloc(len + 1, 1); */
/* 			if (!var) */
/* 				return (NULL); */
/* 			ft_memcpy(var, start, len); */
/* 			ex_var = expanded_var(line, var); */
/* 			if (!ex_var) */
/* 				return (NULL); */
/* 			new_s = n_exp_c(line, (size_t)j, s, ex_var, (size_t)len + 1); */
/* 			if (!new_s) */
/* 				return (free(ex_var), NULL); */
/* 			free(ex_var); */
/* 			return (new_s); */
/* 		} */
/* 		i++; */
/* 	} */
/* 	return (new_s); */
/* } */

char	*replace_var(char *s, int *len, t_line *line)
{
	char	*var;
	int		i;
	int		j;

	if (s[1] == '\0')
	{
		*len = 1;
		var = ft_strdup("$");
		if (!var)
			return (perror("malloc"), NULL);
		return (var);
	}
	if (s[1] == '?')
	{
		*len = 2;
		var = ft_strdup("$?");
		if (!var)
			return (perror("malloc"), NULL);
		var = expanded_var(line, var);
		if (!var)
			return (perror("malloc"), NULL);
		return (var);
	}
	if (s[1] == '$')
	{
		*len = 2;
		var = ft_strdup("$$");
		if (!var)
			return (perror("malloc"), NULL);
		var = expanded_var(line, var);
		if (!var)
			return (perror("malloc"), NULL);
		return (var);
	}
	i = 1;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	if (i == 1)
	{
		*len = 1;
		var = ft_strdup("$");
		if (!var)
			return (perror("malloc"), NULL);
		return (var);
	}
	var = malloc(sizeof(char) * i);
	if (!var)
		return (perror("malloc"), NULL);
	/* printf("s: %s, i: %d\n", s, i); */
	j = 0;
	while (j < i - 1)
	{
		var[j] = s[j + 1];
		j++;
	}
	var[j] = '\0';
	/* printf("var before expand: %s\n", var); */
	var = expanded_var(line, var);
	if (!var)
		return (perror("malloc"), NULL);
	/* printf("var after expand: %s\n", var); */
	*len = i;
	return (var);
}

char	*ft_strjoin_char(char *s, char c)
{
	char	*new_s;
	int		i;

	if (!s)
		return (NULL);
	new_s = malloc(sizeof(char) * (ft_strlen(s) + 2));
	if (!new_s)
		return (perror("malloc"), NULL);
	i = 0;
	while (s[i])
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i++] = c;
	new_s[i] = '\0';
	return (new_s);
}

char	*expanded_content(char *s, t_line *line)
{
	char	*new_s;
	int		i;
	int		len;
	char	*temp;
	char	*tmp;

	if (!s)
		return (NULL);
	new_s = ft_strdup("");
	if (!new_s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		len = 0;
		if (s[i] == '$')
		{
			temp = replace_var(&s[i], &len, line);
			if (!temp)
				return (free(new_s), NULL);
			i += len;
			tmp = ft_strjoin(new_s, temp);
			free(temp);
			if (!tmp)
				return (free(new_s), NULL);
		}
		else
		{
			tmp = ft_strjoin_char(new_s, s[i]);
			if (!tmp)
				return (free(new_s), NULL);
			i++;
		}
		free(new_s);
		new_s = tmp;
	}

	return (new_s);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:11 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/25 18:16:23 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(t_line *line, char *s, int len)
{
	t_token	*token;

	(void)line;
	(void)s;
	if (len <= 0)
		return (NULL); // c'est ok
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL); //clean exit - malloc fail
	token->s = ft_calloc(len + 1, 1);
	if (!token->s)
		return (NULL); //clean exit - malloc fail
	ft_memcpy(token->s, s, len);
	token->s[len] = 0;
	token->next = NULL;
	token->quoted = NO_QUOTE;
	if (token->s[len - 1] == ')')
		token->in_subshell = 1;
	else
		token->in_subshell = 0;
	token->in_heredoc = 0;
	token = token_type(token);
	token = has_env_var(token);
	return (token);
}

t_token	*create_quoted_token(t_line *line, char *s, int len, int quote)
{
	t_token	*token;
	int	i;
	int	j;

	(void)line;
	(void)s;
	if (len <= 0)
		return (NULL); // c'est ok
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL); //clean exit - malloc fail
	token->s = ft_calloc(len + 1, 1); // peut etre faire len - 1 vu quon enleve les deux quotes
	if (!token->s)
		return (NULL); //clean exit - malloc fail
	i = 0;
	j = 0;
	while (i < len)
	{
		if (s[i] == quote)
			i++;
		token->s[j++] = s[i++];
	}
	token->s[j - 1] = 0;
	if (quote == 39)
		token->quoted = SINGLE;
	else
		token->quoted = DOUBLE;
	token->previous = NULL;
	token->next = NULL;
	token = token_type(token);
	return (token);
}

t_token	*last_elem(t_line *line)
{
	t_token	*temp;

	temp = NULL;
	if (line->tokens)
	{
		temp  = line->tokens;
		while (temp->next)
			temp = temp->next;
	}
	return (temp);
}

void	add_back(t_line *line, t_token *new)
{
	t_token	*temp;

	if (new)
	{
		if (!line->tokens)
		{
			line->tokens = new;
			new->next = NULL;
			new->previous = NULL;
		}
		else
		{
			temp = last_elem(line);
			temp->next = new;
			new->previous = temp;
			new->next = NULL;
		}
	}
	return ;
}
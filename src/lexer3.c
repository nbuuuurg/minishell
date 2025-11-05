/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:06:51 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*lexer_last_char(t_line *line, char *s, char *start, char *end)
{
	if (*(s - 1))
	{
		if (!is_whitespace(*(s - 1))
			&& !is_quote(*(s - 1)) && !is_subshell(*(s - 1)))
		{
			end = s;
			add_back(line, create_token(line, start, (end - start) + 1));
			if (line->last_exit != 0)
				return (NULL);
		}
	}
	return (s);
}

t_token	*token_type(t_token *token)
{
	if (!ft_strncmp(token->s, "||", 2) && ft_strlen(token->s) != 1)
		return (token->type = OR, token);
	else if (!ft_strncmp(token->s, "|", 1) && ft_strlen(token->s) != 2)
		return (token->type = PIPE, token);
	else if (!ft_strncmp(token->s, "&&", 2))
		return (token->type = AND, token);
	else if (ft_strnstr(token->s, "<", ft_strlen(token->s))
		&& !ft_strnstr(token->s, "<<", ft_strlen(token->s))
		&& token->quoted == NO_QUOTE)
		return (token->type = REDIR_IN, token);
	else if (ft_strnstr(token->s, ">", ft_strlen(token->s))
		&& !ft_strnstr(token->s, ">>", ft_strlen(token->s))
		&& token->quoted == NO_QUOTE)
		return (token->type = REDIR_OUT, token);
	else if (ft_strnstr(token->s, ">>", ft_strlen(token->s))
		&& token->quoted == NO_QUOTE)
		return (token->type = REDIR_APPEND, token);
	else if (ft_strnstr(token->s, "<<", ft_strlen(token->s))
		&& token->quoted == NO_QUOTE)
		return (token->type = HEREDOC, token);
	return (token->type = WORD, token);
}

t_token	*has_expand(t_token *token)
{
	int	i;

	i = 0;
	token->has_expand = 0;
	if (token->type != WORD)
		return (token);
	while (token->s[i])
	{
		if (token->s[i] == '$')
			token->has_expand++;
		i++;
	}
	return (token);
}

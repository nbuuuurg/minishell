/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:02:38 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 09:02:39 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*new_subinput(t_line *line, t_token *token)
{
	size_t	len;
	char	*subinput;
	size_t	i;
	size_t	j;
	int		subshell;
	int		subshell2;
	char	*temp;
	t_token	*ttemp;

	(void)line;
	len = ft_strlen(token->s);
	subinput = ft_calloc(len - 1, 1);
	if (!subinput)
		return (NULL);
	subshell = 0;
	subshell2 = 0;
	i = 0;
	j = 0;
	temp = NULL;
	while (token->s[i] && i < len - 1)
	{
		if ((subshell == 0 && token->s[i] == '(')
			|| (subshell2 == 0 && token->s[len - i - 1] == ')'))
		{
			if (subshell == 0 && token->s[i] == '(')
				subshell++;
			if (subshell2 == 0 && token->s[len - i - 1] == ')')
				subshell2++;
			i++;
		}
		if (token->s[i] && i < len - 1)
			subinput[j++] = token->s[i++];
	}
	ttemp = token;
	if (ttemp->previous)
	{
		if (ttemp->previous->type == PIPE)
		{
			while (ttemp->previous && ttemp->previous->type != AND
				&& ttemp->previous->type != OR)
			{
				temp = ft_strjoin(ttemp->previous->s, " ");
				ttemp->previous->s = temp;
				temp = ft_strjoin(ttemp->previous->s, subinput);
				free(subinput);
				subinput = temp;
				ttemp = ttemp->previous;
			}
		}
	}
	ttemp = token;
	if (ttemp->next)
	{
		if (ttemp->next->type == PIPE)
		{
			while (ttemp->next && ttemp->next->type != AND
				&& ttemp->next->type != OR)
			{
				temp = ft_strjoin(subinput, " ");
				free(subinput);
				subinput = temp;
				temp = ft_strjoin(subinput, ttemp->next->s);
				free(subinput);
				subinput = temp;
				ttemp = ttemp->next;
			}
		}
	}
	return (subinput);
}

t_line	*dup_line(t_line *line, t_token *subinput)
{
	t_line	*subline;

	subline = ft_calloc(1, sizeof(t_line));
	if (!subline)
		return (line->last_exit = EX_GEN, NULL);
	subline->input = new_subinput(line, subinput);
	if (!subline->input)
		return (line->last_exit = EX_GEN, NULL);
	subline->clean = subline->input;
	subline->envp = line->envp;
	subline->tokens = NULL;
	subline->exprs = NULL;
	subline->cmd = NULL;
	subline->prev_exit = line->prev_exit;
	subline->subline = line;
	subline->last_exit = 0;
	subline->len = line->len - 2;
	subline->lexer_err = 0;
	subline->num_expr = 0;
	subline->heredoc_flag = line->heredoc_flag;
	subline->path = get_path(line->envp);
	return (subline);
}

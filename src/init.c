/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:24 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:00:00 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_minishell(t_line *line, char **envp, int start_flag, t_save *save)
{
	init_line(line, envp, start_flag, save);
	if (line->last_exit != 0)
		return (1);
	if (line->len == 0)
		return (1);
	line->last_exit = lexer_input(line);
	if (line->last_exit != 0)
		return (0);
	if (line->heredoc_flag == 1)
	{
		if (line->exprs)
		{
			free_e(line->exprs);
			line->exprs = NULL;
		}
	}
	return (0);
}

int	init_clean_input(t_line *line)
{
	size_t	i;
	size_t	j;

	if (line->len == 0 || line->len == 1)
		return (0);
	j = len_whitespace(line);
	line->clean = ft_calloc(line->len - j + 1, 1);
	if (!line->clean)
		return (EX_GEN);
	i = 0;
	while (line->input[i] && is_whitespace(line->input[i]))
		i++;
	ft_memcpy(line->clean, &line->input[i], line->len - j);
	free(line->input);
	line->input = line->clean;
	return (0);
}

void	first_init_line(t_line *line, char **envp)
{
	line->tokens = NULL;
	line->exprs = NULL;
	line->cmd = NULL;
	line->subline = NULL;
	line->num_expr = 0;
	line->lexer_err = 0;
	line->heredoc_flag = 0;
	line->prev_exit = 0;
	line->len = ft_strlen(line->input);
	if (envp)
		line->envp = envp;
	else
		line->envp = NULL;
	if (envp)
		line->path = get_path(envp);
	else
		line->path = NULL;
	line->last_exit = init_clean_input(line);
	if (line->last_exit != 0)
		return ;
}

void	other_init_line(t_line *line, char **envp, t_save *save)
{
	line->tokens = NULL;
	line->exprs = NULL;
	line->cmd = NULL;
	line->subline = NULL;
	line->num_expr = 0;
	line->lexer_err = 0;
	line->heredoc_flag = 0;
	line->len = ft_strlen(line->input);
	if (line->envp)
		free_split(line->envp);
	if (envp)
		line->envp = ft_strdup2(save->envp);
	else
		line->envp = NULL;
	if (save->envp)
		line->path = get_path(save->envp);
	else
		line->path = NULL;
	line->last_exit = init_clean_input(line);
	if (line->last_exit != 0)
		return ;
	if (save->envp)
		free_split(save->envp);
	line->prev_exit = save->exit;
}

void	init_line(t_line *line, char **envp, int start_flag, t_save *save)
{
	if (start_flag == 0)
		first_init_line(line, envp);
	else
		other_init_line(line, envp, save);
}

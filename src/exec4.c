/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:42:38 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/10 00:42:38 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	stop_hd_eof(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	if (limiter)
		ft_putstr_fd(limiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

int	fill_res(char **res, char **content, t_line *line)
{
	char	*tmp;
	char	*temp;

	if (*content && need_expand(*content) != 0)
	{
		temp = expanded_content(*content, line);
		if (!temp)
			return (free(*content), perror("malloc"), -1);
		free(*content);
		*content = temp;
	}
	tmp = ft_strjoin(*content, "\n");
	if (!tmp)
		return (free(*content), perror("malloc"), -1);
	free(*content);
	*content = tmp;
	tmp = *res;
	*res = ft_strjoin(tmp, *content);
	if (!*res)
		return (free(*content), free(tmp), perror("malloc"), -1);
	free(*content);
	free(tmp);
	return (0);
}

int	fill_hd_content(char **res, char *li, t_line *line, int *flag)
{
	char	*c;

	while (1)
	{
		c = readline("heredoc> ");
		if (g_sig == 1)
		{
			(1 && (g_sig = 0), (*flag = 1));
			free(c);
			break ;
		}
		if (!c)
		{
			stop_hd_eof(li);
			break ;
		}
		if (ft_strncmp(c, li, ft_strlen(li)) == 0 && c[ft_strlen(li)] == '\0')
		{
			free(c);
			break ;
		}
		if (fill_res(res, &c, line) != 0)
			return (-1);
	}
	return (0);
}

int	hd_c(char *limiter, t_line *line)
{
	int			here_tube[2];
	char		*res;
	int			flag;
	int			save_stdin;
	t_signals	sig;

	(1 && (flag = 0), (save_stdin = dup(STDIN_FILENO)));
	handle_signals_hd(&sig);
	res = ft_strdup("");
	if (!res)
		return (perror("malloc"), restore_signals_hd(&sig), -1);
	if (pipe(here_tube) == -1)
		return (perror("pipe"), restore_signals_hd(&sig), free(res), -1);
	if (fill_hd_content(&res, limiter, line, &flag) != 0)
		return (free(res), restore_signals_hd(&sig), -1);
	dup2(save_stdin, STDIN_FILENO);
	close(save_stdin);
	if (flag != 1)
		write(here_tube[1], res, ft_strlen(res));
	free(res);
	close(here_tube[1]);
	restore_signals_hd(&sig);
	if (flag == 1)
		return (line->heredoc_flag = 1, line->prev_exit = 130, here_tube[0]);
	return (here_tube[0]);
}

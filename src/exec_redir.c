/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 09:26:26 by adeflers          #+#    #+#             */
/*   Updated: 2025/11/28 09:26:26 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redir_out_trunc(t_redir *redirect, char *cmd, int *i, int *fd)
{
	*fd = open(redirect[*i].file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (*fd == -1)
		return (perror(redirect[*i].file), 1);
	if (cmd != NULL)
		dup2(*fd, STDOUT_FILENO);
	close(*fd);
	return (0);
}

int	redir_out_append(t_redir *redirect, char *cmd, int *i, int *fd)
{
	*fd = open(redirect[*i].file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (*fd == -1)
		return (perror(redirect[*i].file), 1);
	if (cmd != NULL)
		dup2(*fd, STDOUT_FILENO);
	close(*fd);
	return (0);
}

int	redir_in_heredoc(t_redir *redirect, int *i, int *fd)
{
	*fd = redirect[*i].hd_fd;
	if (*fd == -1)
		return (perror("hd_fd"), 1);
	dup2(*fd, STDIN_FILENO);
	close(*fd);
	redirect[*i].hd_fd = -1;
	return (0);
}

int	redir_in_file(t_redir *redirect, int *i, int *fd)
{
	*fd = open(redirect[*i].file, O_RDONLY, 0644);
	if (*fd == -1)
		return (perror(redirect[*i].file), 1);
	dup2(*fd, STDIN_FILENO);
	close(*fd);
	return (0);
}

int	ft_redir(t_redir *redirect, char *cmd)
{
	int	i;
	int	fd;

	i = 0;
	if (redirect[i].file == NULL)
		return (1);
	while (redirect[i].file)
	{
		if (ft_strncmp(redirect[i].redir, ">",
				ft_strlen(redirect[i].redir)) == 0)
			redir_out_trunc(redirect, cmd, &i, &fd);
		else if (ft_strncmp(redirect[i].redir, ">>",
				ft_strlen(redirect[i].redir)) == 0)
			redir_out_append(redirect, cmd, &i, &fd);
		else if (ft_strncmp(redirect[i].redir, "<",
				ft_strlen(redirect[i].redir)) == 0)
			redir_in_file(redirect, &i, &fd);
		else if (ft_strncmp(redirect[i].redir, "<<",
				ft_strlen(redirect[i].redir)) == 0)
			redir_in_heredoc(redirect, &i, &fd);
		i++;
	}
	return (0);
}

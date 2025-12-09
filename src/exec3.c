/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:14:23 by adeflers          #+#    #+#             */
/*   Updated: 2025/12/10 00:14:23 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_fd(int *fd_in, int *fd_out, t_redir *redirect, char *cmd)
{
	if (fd_in)
	{
		dup2(fd_in[0], STDIN_FILENO);
		close(fd_in[0]);
		close(fd_in[1]);
	}
	if (fd_out && cmd != NULL)
	{
		dup2(fd_out[1], STDOUT_FILENO);
		close(fd_out[0]);
		close(fd_out[1]);
	}
	else if (fd_out && cmd == NULL)
	{
		close(fd_out[0]);
		close(fd_out[1]);
	}
	if (redirect)
		return (ft_redir(redirect, cmd));
	return (0);
}

int	ft_redir(t_redir *redirect, char *cmd)
{
	int	i;

	i = 0;
	if (redirect[i].file == NULL)
		return (1);
	while (redirect[i].file)
	{
		if (redir_out(redirect[i], cmd) != 0)
			return (1);
		if (redir_in(redirect[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	redir_out(t_redir redirect, char *cmd)
{
	int	fd;

	if (ft_strncmp(redirect.redir, ">", ft_strlen(redirect.redir)) == 0)
	{
		fd = open(redirect.file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd == -1)
			return (perror(redirect.file), 1);
		if (cmd != NULL)
			dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (ft_strncmp(redirect.redir, ">>", ft_strlen(redirect.redir)) == 0)
	{
		fd = open(redirect.file, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
			return (perror(redirect.file), 1);
		if (cmd != NULL)
			dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int	redir_in(t_redir redirect)
{
	int	fd;

	if (ft_strncmp(redirect.redir, "<", ft_strlen(redirect.redir)) == 0)
	{
		fd = open(redirect.file, O_RDONLY, 0644);
		if (fd == -1)
			return (perror(redirect.file), 1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (ft_strncmp(redirect.redir, "<<", ft_strlen(redirect.redir)) == 0)
	{
		fd = redirect.hd_fd;
		if (fd == -1)
			return (1);
		dup2(fd, STDIN_FILENO);
		close(fd);
		redirect.hd_fd = -1;
	}
	return (0);
}

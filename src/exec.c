/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeflers <adeflers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:59:20 by adeflers          #+#    #+#             */
/*   Updated: 2025/09/24 15:59:20 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/wait.h>


void	exec_minishell(t_line line, char **env)
{
	t_cmd	*cmd;


	// on isole chaque bloc de la ligne si il y a des || && ()
	// pour chaque bloc :
	// on regarde si il y a des redirection
	// si oui on associe les bons fd - get_infile et get_outfile
	// on regarde les pipes dans chaque bloc pour l exec multi pipes 
	// on regarde s'il y a des buildins
	// tab de t_cmd pour les sorties de execeve avec un child id pour chaque processus enfant
	// waitpid de tous les childs
	//
	// et attention aux exit codes

	cmd = get_cmd(line.input , line.path); // a modifier pour recuper les data ordonnes du parsing
	cmd[0].id = ft_exec(cmd, env);
	waitpid(cmd[0].id, &cmd[0].status, 0);
	}

t_cmd	*get_cmd(char *input, char** path)
{
	int		i;
	t_cmd	*cmd;
	char	*path_cmd;
	char	**split_input;

	split_input = ft_split(input, ' ');
	i = 0;
	while (split_input[i])
		i++;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = malloc(sizeof(char *) * (i + 1));
	if (!cmd->cmd)
		return (NULL);
	i = 0;
	while (split_input[i])
{
		cmd->cmd[i] = split_input[i];
		i++;
	}
	cmd->cmd[i] = NULL;
	i = 0;
	while (path[i])
	{
		path_cmd = ft_strjoin(path[i], cmd->cmd[0]);
		if (!path_cmd)
			return (free(cmd->cmd), NULL);
		if (access(path_cmd, F_OK) != -1)
		{
			if (access(path_cmd, X_OK) != -1)
			{
				cmd->full_path = path_cmd;
				break ;
			}
		}
		free(path_cmd);
		i++;
	}
	return (cmd);
}

pid_t	ft_exec(t_cmd *cmd, char **env)
{
	pid_t	id;

	id = fork();
	if (id == -1)
		return (1); // error fork
	if (id == 0)
	{
		execve(cmd[0].full_path, cmd[0].cmd, env);
	}
	// apres le fork tjr un waitpid etc mais dans le parent
	

	return (id);
}

/* int	main(int ac, char **av, char **env) */
/* { */
/* 	t_data	data; */
/* 	int		i; */
/**/
/* 	if (ac < 0) */
/* 		return (1); */
/* 	data.env = env; */
/* 	data.path = find_path(data.env); */
/* 	if (!data.path) */
/* 		return (1); */
/* 	data.cmd = get_cmd(av, data.path); */
/* 	if (!data.cmd) */
/* 		return (1); */
/* 	i = 0; */
/* 	while (data.cmd->cmd[i]) */
/* 	{ */
/* 		printf("%s\n", data.cmd->cmd[i]); */
/* 		i++; */
/* 	} */
/* 	printf("%s\n", data.cmd->cmd[i]); */
/* 	ft_exec(data); */
/* 	free_split(data.path); */
/* 	free(data.cmd->full_path); */
/* 	free(data.cmd->cmd); */
/* 	free(data.cmd); */
/* 	return (0); */
/* } */

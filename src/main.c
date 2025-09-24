/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:07:17 by nburgevi          #+#    #+#             */
/*   Updated: 2025/09/23 23:49:23 by adeflers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/wait.h>

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	char	**env;
	t_cmd	*cmd;
	t_child	child;

	env = envp;
	(void)av;
	if (ac != 1)
		exit ((ft_putstr_fd("invalid arguments\n", 2), EX_USAGE));
	ft_bzero(&line, sizeof(t_line));
	while(1)
	{
		line.input = readline("minishell>>>");
		if (!line.input)
			exit(EX_OK); // EOF
		if (ft_strncmp(line.input, "exit", 4) == 0)
			return (EX_OK); // exit
		if (line.input)
			add_history(line.input);
		init_minishell(&line, env);


					/* ----- EXEC ---- */

		cmd = get_cmd(line.input, line.path);

		child.id = ft_exec(cmd, env);
		waitpid(child.id, &child.status, 0);





	/**/
	/* 	printf("%s\n", line.input); */
	/* 	printf("%s\n", line.clean); */
	/* 	printf("%d\n", line.last_exit); */
	/* 	printf("%d\n", line.num_expr); */
	/**/
	/* 	int	i = 0; */
	/* 	while (line.envp[i])  */
	/* 	{ */
	/* 		printf("%s\n", line.envp[i]); */
	/* 		i++; */
	/* 	} */
	/* 	 */
	/* 	i = 0; */
	/* 	while (line.path[i])  */
	/* 	{ */
	/* 		printf("%s\n", line.path[i]); */
	/* 		i++; */
	/* 	} */
	/*  */
		
		// on regarde bien ce qui ressort du parsing et on attaque l exec ici
		// et voila quoi
		// fork
		// execve tt ca 
		// waitpid et autres joyeuseries
		// pipe tt ca 




		free(line.input);
		free_split(line.path);
	}
}

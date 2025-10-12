/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:07:17 by nburgevi          #+#    #+#             */
/*   Updated: 2025/10/08 22:06:21 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// sortir propre avec la fonction exit
// subshell
// wildcard
// norme

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	char	**env;

	(void)env;
	(void)av;
	env = envp;
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
		init_minishell(&line, envp);
		// if (line.exprs)
		//  	exec_minishell(&line, env);
		free_line(&line);
		// free_readline(...);
	}
}

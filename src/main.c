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

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	char	**env;

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
		/* printf("%s\n", line.input); */
		if (ft_strncmp(line.input, "exit", 4) == 0)
			return(EX_OK); // exit
		if (line.input)
			add_history(line.input);
		init_minishell(&line, env);
					/* ----- EXEC ---- */
		if (line.exprs) // enlever print_expr(line) de lexer.c pour rentrer dans l exec
			exec_minishell(&line, env);
		free(line.input);
		free_split(line.path);
	}
	return (0);
}


// isatty pour pas que le ./minishell | ./minishell plante

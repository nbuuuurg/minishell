/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:07:17 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:10:25 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_sig = 0;

// s | (echo a) -> leak
// ls | cat << eof -> leak fd non ferme
// ls | exit -> ne doit pas exit
// (exit) -> doit exit le subshell
// export A="          " && $A -> erreur sortie

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	t_save	save;
	char	**env;
	int		start_flag;

	// if (!isatty(STDIN_FILENO))
	// 	return (1);
	setup_signals();
	start_flag = 0;
	env = ft_strdup2(envp);
	if (!env)
		return (1);
	(void)av;
	if (ac != 1)
		exit ((ft_putstr_fd("invalid arguments\n", 2), EX_USAGE));
	ft_bzero(&line, sizeof(t_line));
	ft_bzero(&save, sizeof(t_save));
	while (1)
	{
		line.input = readline("minishell> ");
		if (g_sig == 1)
		{
			g_sig = 0;
			save.exit = 130;
		}
		if (!line.input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			clear_history();
			if (save.envp)
				free_split(save.envp);
			if (line.envp)
				free_split(line.envp);
			else if (env)
				free_split(env);
			return (EX_OK);
		}
		if (line.input && line.input[0] != '\0')
			add_history(line.input);
		init_minishell(&line, env, start_flag, &save);
		// if (line.exprs)
			// exec_minishell(&line);
		recup_save(&line, &save);
		free_line(&line);
		start_flag = 1;
	}
	free_split(env);
	return (0);
}

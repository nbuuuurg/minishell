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

// expand : si $[not found in env] faire comme si ca n existait pas au lieu de remplacer par ""
//			(pas dramatique dans le heredoc mais relou en dehors)
//
//			a deplacer au niveau de exec_exprs pour que marche export A=123 && echo $A 
//			(parser l'expand de exprs 2 apres avoir executer exprs 1)
//
// heredoc : << e cat << r va cat e au lieu de r --> pourquoi ? idk man
//
// Signaux : ^C ne met pas tjr exit code a 130
//			 ^D leaks si il y a eu des commandes avant 

// s | (echo a) -> leak

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
		if (line.input)
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

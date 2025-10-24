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

// signaux
// free cmd dans le fork
// builtin
// assignation
// exit avec les bon codes erreurs
// invalid synthaxe si redirect.file = NULL
// (fun check)
// (SHLVL et si export PATH=:)
// operateur OU
// probleme valgrind enfant quand on a plusieurs exprs

void	restore_terminal(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
	{
		write(STDERR_FILENO, "sois plus sympa avec tes tests et suis la fiche de correction stp\n", 66);
		if (tcgetattr(STDIN_FILENO, &term) == 0)
		{
			term.c_lflag |= (ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
		}
		exit (1);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	char	**env;
	int		start_flag;

	restore_terminal();
	start_flag = 0;
	env = ft_strdup2(envp);
	if (!env)
		exit (1);
	(void)av;
	if (ac != 1)
		exit ((ft_putstr_fd("invalid arguments\n", 2), EX_USAGE));
	ft_bzero(&line, sizeof(t_line));
	while (1)
	{
		restore_terminal();
		line.input = readline("minishell>>>");
		if (!line.input)
		{
			clear_history();
			free_line(&line);
			free_split(env);
			return (EX_OK);
		}
		if (ft_strncmp(line.input, "exit", 4) == 0)
		{
			clear_history();
			free_split(env);
			return (EX_OK);
		}
		if (line.input)
			add_history(line.input);
		init_minishell(&line, env, start_flag);
		// print_token(&line);
		// print_expr(&line);
		if (line.exprs)
			exec_minishell(&line);
		free_line(&line);
		start_flag = 1;
	}
	free_split(env);
	return (0);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_line	line;
// 	char	**env;

// 	(void)env;
// 	(void)av;
// 	env = envp;
// 	if (ac != 1)
// 		exit ((ft_putstr_fd("invalid arguments\n", 2), EX_USAGE));
// 	ft_bzero(&line, sizeof(t_line));
// 	while(1)
// 	{
// 		line.input = readline("minishell>>>");
// 		if (!line.input)
// 			exit(EX_OK); // EOF
// 		if (ft_strncmp(line.input, "exit", 4) == 0)
// 			return (EX_OK); // exit
// 		if (line.input)
// 			add_history(line.input);
// 		init_minishell(&line, envp);
// 		// if (line.exprs)
// 		//  	exec_minishell(&line, env);
// 		t_token *temp;
// 		temp = line.tokens;
// 		while (line.tokens->next)
// 		{
// 			if (line.tokens->has_wildcards == 1)
// 			{
// 				line.tokens = parse_wildcards(&line, line.tokens);
// 				line.tokens = line.tokens->next;
// 			}
// 		}
// 		temp = line.tokens;
// 		print_expr(&line);
// 		// print_token(&line);
// 		free_line(&line);
// 		// free_readline(...);
// 	}
// }

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

// builtin
// exit code cmd not found
// (SHLVL et si export PATH=:)
// (heredoc avec guillemets sans expand de variable voir e"'e'")
// signaux heredoc / signaux dans cat > e
// > e ou >> e

volatile sig_atomic_t g_sig = 0;

void	restore_terminal(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
	{
		/* write(STDERR_FILENO, "sois plus sympa avec tes tests et suis la fiche de correction stp\n", 66); */
		if (tcgetattr(STDIN_FILENO, &term) == 0)
		{
			term.c_lflag |= (ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
		}
		return ;
		// attention aux leaks si tu sors d'ici
	}
}

void	recup_save(t_line *line, t_save *save)
{
	if (line->envp)
		save->envp = ft_strdup2(line->envp);
	else
		save->envp = NULL;
	save->exit = line->prev_exit;
}

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	t_save	save;
	char	**env;
	int		start_flag;

	setup_signals(); 	
	start_flag = 0;
	// pour faire un env avec que 3 lignes comme env -i bash --posix (si on a le temps)
	/* if (!envp || !envp[0]) */
	// env = malloc(3 * sizeof(char *));
	// avec pwd, shlvl=X, _=/usr/bin/env

	env = ft_strdup2(envp);
	if (!env)
		return (1);
	(void)av;
	if (ac != 1)
		exit ((ft_putstr_fd("invalid arguments\n", 2), EX_USAGE));
	ft_bzero(&line, sizeof(t_line));
	ft_bzero(&save, sizeof(t_save));
	/* restore_terminal(); */
	while (1)
	{
		if (g_sig == 1)
		{
            line.last_exit = 130;
            g_sig = 0;
        }
		line.input = readline("minishell> ");
		if (!line.input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			clear_history();
			free_split(env);
			return (EX_OK);
		}
		if (ft_strncmp(line.input, "exit", 4) == 0)
		{
			// free propre fonction exit
			clear_history();
			recup_save(&line, &save);
			free_split(env);
			free_split(save.envp);
			free(line.input);
			return (EX_OK);
		}
		if (line.input)
			add_history(line.input);
		init_minishell(&line, env, start_flag, &save); // +1 var
		if (line.exprs)
			exec_minishell(&line);
		// print_expr(&line);
		// print_token(&line);
		recup_save(&line, &save);
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

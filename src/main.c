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

void	restore_terminal(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= (ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_line	line;
	char	**env;
	int		start_flag;

	if (!isatty(STDIN_FILENO))
	{
		restore_terminal();
		write(STDERR_FILENO, "sois plus sympa avec tes tests et suis la fiche de correction stp\n", 66);
		return (1);
	}
	start_flag = 0;
	env = ft_strdup2(envp);
	if (!env)
		exit (1);
	(void)av;
	if (ac != 1)
		exit ((ft_putstr_fd("invalid arguments\n", 2), EX_USAGE));
	ft_bzero(&line, sizeof(t_line));
	while(1)
	{
		restore_terminal();
		line.input = readline("minishell>>>");
		if (!line.input)
			exit(EX_OK); // EOF
		/* printf("%s\n", line.input); */
		if (ft_strncmp(line.input, "exit", 4) == 0)
			return(EX_OK); // exit
		if (line.input)
			add_history(line.input);
		init_minishell(&line, env, start_flag);
					/* ----- EXEC ---- */
		if (line.exprs) // enlever print_expr(line) de lexer.c pour rentrer dans l exec
			exec_minishell(&line);
		free(line.input);
		free_split(line.path);
		start_flag = 1;
	}
	return (0);
}

// probleme avec le restore terminal qui ne restaure pas bien le terminal
// a creuser



//
// en fait je sais : ca vient d un moment ou readline a crasher et il a pas pu reset le terminal ce qui fait qu on quitte le mode canonique ou un truc du genre et du coup tout le terminal est bloque la dedans, ca vient pas de mon code - mais justement tcgetattr peut aider a regler ca et sera utile pour gerer les signaux
// bref un bordel

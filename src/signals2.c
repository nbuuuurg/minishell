/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:41 by nburgevi          #+#    #+#             */
/*   Updated: 2025/11/05 08:59:59 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sigint_handler_hd(int sig)
{
	(void)sig;
	g_sig = 1;
	rl_done = 1;
	write(STDERR_FILENO, "^C", 2);
	close(STDIN_FILENO);
}
void	sigquit_handler_hd(int sig)
{
	(void)sig;
	g_sig = 1;
}

void	sigint_handler_child(int sig)
{
	(void)sig;
	g_sig = 1;
	write(STDERR_FILENO, "^C\n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	sigquit_handler_child(int sig)
{
	(void)sig;
	g_sig = 1;
	write(STDERR_FILENO, "^\\Quit (core dumped)\n", 21);
	rl_replace_line("", 0);
	rl_on_new_line();
}

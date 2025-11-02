/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburgevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:38:41 by nburgevi          #+#    #+#             */
/*   Updated: 2025/08/26 04:38:47 by nburgevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void sigint_handler(int sig)
{
    (void)sig;
    g_sig = 1;
    write(STDERR_FILENO, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void sigint_handler_hd(int sig)
{
    (void)sig;
    g_sig = 1;
    write(STDERR_FILENO, "^C\n", 3);
}

void sigquit_handler_hd(int sig)
{
    (void)sig;
    g_sig = 1;
}

void sigint_handler_child(int sig)
{
    (void)sig;
    g_sig = 1;
    write(STDERR_FILENO, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    // _exit(130);
}

void sigquit_handler_child(int sig)
{
    (void)sig;
    g_sig = 1;
    write(STDERR_FILENO, "Quit (core dumped)\n", 19);
    rl_replace_line("", 0);
    rl_on_new_line();
}

void setup_signals(void)
{
    extern int rl_catch_signals; // connue par la lib readline
    rl_catch_signals = 0;
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

void setup_signals_child(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
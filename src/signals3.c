#include "../include/minishell.h"

void	handle_signals_child(t_signals *sig)
{
	sigemptyset(&sig->new.sa_mask);
	sig->new.sa_handler = SIG_IGN;
	sig->new.sa_flags = 0;
	sigaction(SIGINT, &sig->new, &sig->old_int);
	sigaction(SIGQUIT, &sig->new, &sig->old_quit);
	signal(SIGINT, sigint_handler_child);
	signal(SIGQUIT, sigquit_handler_child);
	tcgetattr(STDIN_FILENO, &sig->old);
	sig->t = sig->old;
	sig->t.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &sig->t);
}

void	restore_signals_child(t_signals *sig)
{
	sigaction(SIGINT, &sig->old_int, NULL);
	sigaction(SIGQUIT, &sig->old_quit, NULL);
	tcsetattr(STDIN_FILENO, TCSANOW, &sig->old);
}

void	handle_signals_hd(t_signals *sig)
{
	sigemptyset(&sig->sa_int.sa_mask);
	sigemptyset(&sig->sa_quit.sa_mask);
	sig->sa_int.sa_flags = 0;
	sig->sa_quit.sa_flags = 0;
	sig->sa_int.sa_handler = sigint_handler_hd;
	sig->sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sig->sa_int, &sig->old_int);
	sigaction(SIGQUIT, &sig->sa_quit, &sig->old_quit);
}

void	restore_signals_hd(t_signals *sig)
{
	sigaction(SIGINT, &sig->old_int, NULL);
	sigaction(SIGQUIT, &sig->old_quit, NULL);
}
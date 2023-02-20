/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 12:42:22 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/17 21:42:40 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	Handles the signal ctrl-c, exit code is changed to 130 if user does ctrl-d.
	Outputs a newline to simulate the behavior as in bash. Tells the update 
	functions that we have moved onto a new (empty) line.
	Changes what’s displayed on the screen to reflect the current content.
*/
static void	sig_handler(int sig, siginfo_t *info, void *ucontext)
{
	if (sig == SIGINT)
	{
		(void)info;
		(void)ucontext;
		(void)sig;
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_code = 130;
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;
	struct sigaction	quit_sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &sig_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, 0);
	sigemptyset(&quit_sa.sa_mask);
	quit_sa.sa_handler = SIG_IGN;
	quit_sa.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &quit_sa, 0);
}

/*
	Handle the SIGINT signal (ctrl-C) during heredoc
	//signal(SIGINT, SIG_DFL);
*/
static void	signint_heredoc(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	(void)sig;
	write(STDOUT_FILENO, "\n\b", 2);
	close(STDIN_FILENO);
	g_exit_code = 130;
}

void	setup_signal_heredoc(void)
{
	struct sigaction	sa;
	struct sigaction	quit_sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &signint_heredoc;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, 0);
	sigemptyset(&quit_sa.sa_mask);
	quit_sa.sa_handler = SIG_IGN;
	quit_sa.sa_flags = 0;
	sigaction(SIGQUIT, &quit_sa, NULL);
}

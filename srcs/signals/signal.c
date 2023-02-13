/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 12:42:22 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/02 19:58:59 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	Handles the signal ctrl-c, exit code is changed to 130 if user does ctrl-d.
	Outputs a newline to simulate the behavior as in bash. Tells the update 
	functions that we have moved onto a new (empty) line.
	Changes what’s displayed on the screen to reflect the current content.
*/
void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_code = 130;
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:19:29 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/16 17:51:55 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_file_fd(t_prompt *p)
{
	if (p->infile != -2)
	{
		close(p->infile);
		p->infile = -2;
	}
	if (p->outfile != -2)
	{
		close(p->outfile);
		p->outfile = -2;
	}
}

void	open_infile_outfile(t_prompt *p, t_list_tokens *tmp)
{
	if (tmp->type == HEREDOC)
	{
		if (p->infile != -2)
			close(p->infile);
		p->infile = open(tmp->str, O_RDONLY);
	}
	if (tmp->type == INPUT)
	{
		if (p->infile != -2)
			close(p->infile);
		p->infile = open(tmp->next->str, O_RDONLY);
	}
	if (tmp->type == R_DREDIR)
	{
		if (p->outfile != -2)
			close(p->outfile);
		p->outfile = open(tmp->next->str, O_CREAT | O_WRONLY
				| O_APPEND, 0777);
	}
	if (tmp->type == R_REDIR)
	{
		if (p->outfile != -2)
			close(p->outfile);
		p->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0777);
	}
}

void	wait_signal(pid_t wpid, t_prompt *p, int save_status, int is_builtin)
{
	wpid = waitpid(-1, &g_exit_code, 0);
	save_status = g_exit_code;
	while (wpid != -1 || errno != ECHILD)
	{
		wpid = waitpid(-1, &g_exit_code, 0);
		if (wpid == p->pipex->pid && p->nbr_pipe > 0)
			save_status = g_exit_code;
	}
	if (WIFSIGNALED(save_status) && is_builtin == 0)
		g_exit_code = 128 + WTERMSIG(save_status);
	else if (WIFEXITED(save_status) && is_builtin == 0)
		g_exit_code = WEXITSTATUS(save_status);
	else if (is_builtin == 0)
		g_exit_code = save_status;
}

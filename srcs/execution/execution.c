/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:12:22 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/16 18:00:17 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	one_command(t_prompt *p, t_list_tokens *e_tokens, int *is_builtin)
{
	int	ret;

	ret = CMD_NOT_FOUND;
	while (e_tokens->next && e_tokens->type != STRING)
		e_tokens = e_tokens->next;
	if (e_tokens->type == END)
		return (close_pipe(p), 0);
	else if (is_built(e_tokens->str) == 1)
	{
		*is_builtin = 1;
		ret = execute_built(p, e_tokens);
	}
	else
		ret = execute_one_sys(p, e_tokens);
	return (ret);
}

static void	dup_fd(t_prompt *p, int **fd, int index)
{
	if (p->infile != -2)
		dup2(p->infile, STDIN_FILENO);
	else if (index != 0)
		dup2(fd[index - 1][0], STDIN_FILENO);
	if (p->outfile != -2)
		dup2(p->outfile, STDOUT_FILENO);
	else if (index != p->nbr_pipe)
		dup2(fd[index][1], STDOUT_FILENO);
	close_pipe(p);
}

int	child_process(t_prompt *p, t_list_tokens *e_tokens, int *is_builtin)
{
	int				**fd;
	int				index;
	t_list_tokens	*tmp;

	fd = p->pipex->fd;
	index = e_tokens->index;
	tmp = e_tokens;
	while (tmp->type != STRING && tmp->type != PIPE && tmp->type != END)
		tmp = tmp->next;
	if (tmp->type == END || tmp->type == PIPE)
	{
		close_free_pipe(p);
		return (exit_shell(p, g_exit_code), 0);
	}
	if (is_built(e_tokens->str) == 1)
	{
		*is_builtin = 1;
		g_exit_code = execute_built(p, e_tokens);
	}
	else
	{
		dup_fd(p, fd, index);
		g_exit_code = execute_sys(p, tmp);
	}
	return (g_exit_code);
}

static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	execute_cmd(t_prompt *p, t_list_tokens *e_tokens, int *is_builtin)
{
	t_pipex	*pipex;

	pipex = p->pipex;
	if (p->nbr_pipe == 0)
		g_exit_code = one_command(p, e_tokens, is_builtin);
	else
	{
		signal(SIGINT, &sig_handler);
		pipex->pid = fork();
		if (pipex->pid == -1)
			perror("fork error\n");
		if (pipex->pid == 0)
			g_exit_code = child_process(p, e_tokens, is_builtin);
	}
	return (g_exit_code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:19:29 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/28 13:50:29 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_file_fd(t_prompt *p)
{
	if (p->infile != -2)
	{
		if (p->infile != -1)
			close(p->infile);
		p->infile = -2;
	}
	if (p->outfile != -2)
	{
		if (p->infile != -1)
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
				| O_APPEND, 0644);
	}
	if (tmp->type == R_REDIR)
	{
		if (p->outfile != -2)
			close(p->outfile);
		p->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0644);
	}
}

void	wait_signal(pid_t wpid, t_prompt *p, int save_status, int is_builtin)
{
	wpid = waitpid(-1, &g_exit_code, 0);
	save_status = g_exit_code;
	while (wpid != -1 || errno != ECHILD)
	{
		(void)p;
		wpid = waitpid(-1, &g_exit_code, 0);
		if (wpid == p->pipex->pid && p->nbr_pipe > 0)
			save_status = g_exit_code;
	}
	if (p->infile == -1 || p->outfile == -1)
		g_exit_code = 1;
	else if (WIFSIGNALED(save_status) && is_builtin == 0)
		g_exit_code = 128 + WTERMSIG(save_status);
	else if (WIFEXITED(save_status) && is_builtin == 0)
		g_exit_code = WEXITSTATUS(save_status);
	else if (is_builtin == 0)
		g_exit_code = save_status;
}

static int	continue_result(t_list_tokens *e_tokens, int result)
{
	if (access(e_tokens->str, F_OK) != -1 \
	&& ((ft_strncmp(e_tokens->str, "./", 2) == 0) \
	&& ft_strlen(e_tokens->str) > 2))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(e_tokens->str, STDERR_FILENO);
		ft_putendl_fd(": permission denied", STDERR_FILENO);
		g_exit_code = 126;
		return (126);
	}
	else if ((ft_strncmp(e_tokens->str, "./", 2) == 0)
		&& ft_strlen(e_tokens->str) > 2)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(e_tokens->str, STDERR_FILENO);
		ft_putendl_fd(": no such file or directory", STDERR_FILENO);
		g_exit_code = 127;
		return (127);
	}
	else
		result = -1;
	return (result);
}

int	get_result(t_prompt *p, t_list_tokens *e_tokens, int result)
{
	int	fd;

	fd = open(e_tokens->str, O_RDWR);
	if (fd == -1 && errno == EISDIR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(e_tokens->str, STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
		g_exit_code = 126;
		return (126);
	}
	if (fd != -1)
		close (fd);
	if (p->pipex->cmd != NULL)
		result = execve(p->pipex->cmd, p->pipex->cmd_arg, p->env);
	else if (access(e_tokens->str, X_OK) != -1
		&& ((ft_strncmp(e_tokens->str, "./", 2) == 0)
			&& ft_strlen(e_tokens->str) > 2))
	{
		g_exit_code = 0;
		return (0);
	}
	else
		result = continue_result(e_tokens, result);
	return (result);
}

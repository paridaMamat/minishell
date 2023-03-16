/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/16 12:27:23 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_file(t_prompt *p, t_list_tokens *e_tokens)
{
	t_list_tokens   *tmp;

	tmp = e_tokens;
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
	while (tmp->type != PIPE && p->infile != -1 && p->outfile != -1 && tmp->type != END)
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
			p->outfile = open(tmp->next->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
		}
		if (tmp->type == R_REDIR)
		{
			if (p->outfile != -2)
				close(p->outfile);
			p->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0777);
		}
		if (p->infile == -1)
			perror("infile");
		if (p->outfile == -1)
			perror("outfile");
		tmp = tmp->next;
	}
}

int	count_pipe(t_prompt *p)
{
	int		i;
	t_list_tokens   *tmp;

	i = 0;
	tmp = p->tokens;
	while (tmp)
	{
		if (tmp->type == PIPE)
			i++;
		tmp->index = i;
		tmp = tmp->next;
	}
	return (i);
}

int	open_pipe(t_prompt *p)
{
	int	i;

	i = 0;
	p->pipex->fd = 0;
	if (p->nbr_pipe != 0)
	{
		p->pipex->fd =(int **)malloc(sizeof(int *) * (p->nbr_pipe + 2));
		if (p->pipex->fd == NULL)
			return (free(p->pipex), perror("malloc"), 1);
		p->pipex->fd[p->nbr_pipe + 1] = 0;
		while (i <= p->nbr_pipe)
		{
			p->pipex->fd[i] = (int *)malloc(sizeof(int) * 2);
			if (!p->pipex->fd[i])
				return(free(p->pipex->fd), free(p->pipex), perror("pipe error\n"), 2);
			if (pipe(p->pipex->fd[i]) < 0)
				return (free(p->pipex->fd), free(p->pipex), perror("pipe error\n"), 2);
			i++;
		}
	}
	return (0);
}

int	init_data(t_prompt *p)
{
	int	ret;

	ret = 0;
	p->infile = -2;
	p->outfile = -2;
	p->tokens->index = -1;
	p->nbr_pipe = count_pipe(p);
	ret = open_pipe(p);
	return (ret);
}

int start_execute(t_prompt *p)
{
    t_list_tokens   *curr;
    pid_t    wpid;
    int      save_status;
	int		is_builtin;
        
    curr = p->tokens;
    save_status = 0;
    wpid = 0;
	is_builtin = 0;
    while (curr)
    {
        open_file(p, curr);
        if (p->infile != -1 && p->outfile != -1)
            save_status = execute_cmd(p, curr, &is_builtin);
        while (curr->type != PIPE && curr->type != END)
            curr = curr->next;
        curr = curr->next;
    }
    if (p->nbr_pipe != 0)
		close_free_pipe(p);
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
    return (g_exit_code);
}


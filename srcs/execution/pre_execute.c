/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/17 15:45:45 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(t_prompt *p, t_list_tokens *e_tokens)
{
	t_list_tokens	*tmp;

	tmp = e_tokens;
	init_file_fd(p);
	while (tmp->type != PIPE && p->infile != -1
		&& p->outfile != -1 && tmp->type != END)
	{
		open_infile_outfile(p, tmp);
		if (p->infile == -1 || p->outfile == -1)
			perror("minishell");
		tmp = tmp->next;
	}
	return (0);
}

int	count_pipe(t_prompt *p)
{
	int				i;
	t_list_tokens	*tmp;

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
		p->pipex->fd = (int **)malloc(sizeof(int *) * (p->nbr_pipe + 2));
		if (p->pipex->fd == NULL)
			return (free(p->pipex), perror("malloc"), 1);
		p->pipex->fd[p->nbr_pipe + 1] = 0;
		while (i <= p->nbr_pipe)
		{
			p->pipex->fd[i] = (int *)malloc(sizeof(int) * 2);
			if (!p->pipex->fd[i])
				return (free(p->pipex->fd), free(p->pipex),
					perror("pipe error\n"), 2);
			if (pipe(p->pipex->fd[i]) < 0)
				return (free(p->pipex->fd), free(p->pipex),
					perror("pipe error\n"), 2);
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

int	start_execute(t_prompt *p)
{
	t_list_tokens	*curr;
	pid_t			wpid;
	int				save_status;
	int				is_builtin;

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
	wait_signal(wpid, p, save_status, is_builtin);
	return (g_exit_code);
}

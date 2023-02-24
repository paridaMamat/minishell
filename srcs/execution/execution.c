/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:12:22 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/24 10:27:17 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	one_command(t_prompt *p, t_list_tokens *e_tokens)
{
	int ret;
    
    ret = CMD_NOT_FOUND;
    while (e_tokens->next && e_tokens->type != STRING)
		e_tokens = e_tokens->next;
	if (e_tokens->type == END)	
		return (close_pipe(p), 0);
	else if (is_built(e_tokens->str) == 1)
		ret = execute_built(p, e_tokens);
	else
		ret = execute_one_sys(p, e_tokens);
	return (ret);
}


int	child_process(t_prompt *p, t_list_tokens *e_tokens)
{
	int		**fd;
	int		index;
	int		ret;
    
    fd = p->pipex->fd;
	index = e_tokens->index;
	t_list_tokens	*tmp;
    
    ret = CMD_NOT_FOUND;
	tmp = e_tokens;
 	while (tmp->type != STRING && tmp->type != PIPE && tmp->type != END)
		tmp = tmp->next;
	if (tmp->type == END || tmp->type == PIPE)
	{
		close_free_pipe(p);
		return (exit_shell(p, g_exit_code), 0);
	}
	if (is_built(e_tokens->str) == 1)
		ret = execute_built(p, e_tokens);
    else 
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
		ret = execute_sys(p, tmp);
	}
	// dprintf(2, "return of child process = %d\n", ret);
	return (ret);
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

int	execute_cmd(t_prompt *p, t_list_tokens *e_tokens)
{
    t_pipex *pipex;
	int		ret;
    
	ret = 0;
    pipex = p->pipex;
	if (p->nbr_pipe == 0)
        ret = one_command(p, e_tokens);
	else
	{
		signal(SIGINT, &sig_handler);
		pipex->pid = fork();
		if (pipex->pid == -1)
			perror("fork error\n");
		if (pipex->pid == 0)
			ret = child_process(p, e_tokens);
	}
	// dprintf(2, "return value of execute_cmd = %d\n", ret);
	return (ret);
}
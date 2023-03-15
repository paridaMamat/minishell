/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 12:23:08 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/13 14:28:16 by parida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int	execute_built(t_prompt *p, t_list_tokens *e_tokens)
{
	int	ret;
	int	fd;

	ret = 0;
	if (p->infile != -2)
		close(p->infile);
	if (p->outfile != -2)
		fd = p->outfile;
	else if (e_tokens->index < p->nbr_pipe)
		fd = p->pipex->fd[e_tokens->index][1];
	else
		fd = STDOUT_FILENO;
	if (ft_strncmp(e_tokens->str, "echo", 5) == 0)
		ret = minishell_echo(p, e_tokens->next, fd);
    //else if (ft_strncmp(e_tokens->str, "cd", 3) == 0)
	    //ret = execute_cd(p, e_tokens);
	else if (ft_strncmp(e_tokens->str, "env", 4) == 0)
	    ret = minishell_env(p, e_tokens->next, fd);
    else if (ft_strncmp(e_tokens->str, "export", 7) == 0)
	    ret = minishell_export(p, e_tokens->next, fd);
    else if (ft_strncmp(e_tokens->str, "pwd", 4) == 0)
	    ret = minishell_pwd(p, e_tokens, fd);
    else if (ft_strncmp(e_tokens->str, "unset", 6) == 0)
	    ret = minishell_unset(p, e_tokens->next);
    else if (ft_strncmp(e_tokens->str, "exit", 5) == 0)
	    ret = minishell_exit(p, e_tokens, fd);
    if (p->outfile != -2)
	    close(p->outfile);
    if (e_tokens->index < p->nbr_pipe)
	    close(p->pipex->fd[e_tokens->index][1]);
    if (p->nbr_pipe != 0)
	{
		close_free_pipe(p);
		exit_shell(p, g_exit_code);	
	}			
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

int	execute_one_sys(t_prompt *p, t_list_tokens *e_tokens)
{
	int	ret;

	ret = 0;
	signal(SIGINT, &sig_handler);
	p->pipex->pid = fork();
	if (p->pipex->pid == -1)
		perror("fork error\n");
	if (p->pipex->pid == 0)
	{
		if (p->infile != -2)
		{
			dup2(p->infile, STDIN_FILENO);
			close(p->infile);
		}
		if (p->outfile != -2)
		{
			dup2(p->outfile, STDOUT_FILENO);
			close(p->outfile);
		}
		ret = execute_sys(p, e_tokens);
	}
	if (p->infile != -2)
		close(p->infile);
	if (p->outfile != -2)
		close(p->outfile);
	printf("ret one system = %d\n", ret);
    return (ret);
}

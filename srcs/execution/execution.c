/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:12:22 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/14 23:53:51 by parida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	one_command(t_prompt *p, t_list_tokens *e_tokens)
// {
// 	int ret;
    
//     ret = CMD_NOT_FOUND;
//     while (e_tokens->type != STRING)
//         e_tokens = e_tokens->next;
//     if (ft_strncmp(e_tokens->str, "echo", 5) == 0)
//         ret = execute_echo(p, e_tokens);
//     if (ft_strncmp(e_tokens->str, "cd", 3) == 0)
// 		ret = execute_cd(p, e_tokens);
// 	else if (ft_strncmp(e_tokens->str, "env", 4) == 0)
// 		ret = execute_env(p, e_tokens);
// 	else if (ft_strncmp(e_tokens->str, "export", 7) == 0)
// 		ret = execute_export(p, e_tokens);
// 	else if (ft_strncmp(e_tokens->str, "pwd", 4) == 0)
// 		ret = execute_pwd(p, e_tokens);
// 	else if (ft_strncmp(e_tokens->str, "unset", 6) == 0)
// 		ret = execute_unset(p, e_tokens);
// 	else if (ft_strncmp(e_tokens->str, "exit", 5) == 0)
// 		ret = execute_exit(p, e_tokens);
// 	return (ret);
// }

int execute(t_prompt *p, t_list_tokens *e_tokens)
{
    int ret;
    
    ret = CMD_NOT_FOUND;
    while (e_tokens->type != STRING)
        e_tokens = e_tokens->next;
    // if (ft_strncmp(e_tokens->str, "echo", 5) == 0 && e_tokens->nbr_pipe != 0)
    //     ret = execute_echo(p, e_tokens);
    // if (ft_strncmp(e_tokens->str, "cd", 3) == 0 && e_tokens->nbr_pipe != 0)
	// 	ret = execute_cd(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "env", 4) == 0 && e_tokens->nbr_pipe != 0)
	// 	ret = execute_env(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "export", 7) == 0 && e_tokens->nbr_pipe != 0)
	// 	ret = execute_export(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "pwd", 4) == 0 && e_tokens->nbr_pipe != 0)
	// 	ret = execute_pwd(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "unset", 6) == 0 && e_tokens->nbr_pipe != 0)
	// 	ret = execute_unset(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "exit", 5) == 0 && e_tokens->nbr_pipe != 0)
	// 	ret = execute_exit(p, e_tokens);
    // else
    ret = execute_sys(p, e_tokens);
	return (ret);
}

int	child_process(t_prompt *p, t_list_tokens *e_tokens)
{
	int		(*fd)[2];
    t_fds   *fds;
	int		index;
	int		ret = 0;
    
    fd = p->pipex->fd;
    fds = p->fds;
	index = e_tokens->index;
	if (fds->infile != -1 && fds->infile != -2)
	{
		dup2(fds->infile, STDIN_FILENO);
		close(fds->infile);
	}
	else if (index != 0)
		dup2(fd[index][0], STDIN_FILENO);
	if (fds->outfile != -1 && fds->outfile != -2)
	{
		dup2(fds->outfile, STDOUT_FILENO);
		close(fds->outfile);
	}
	else if (index != e_tokens->nbr_pipe)
		dup2(fd[index + 1][1], STDOUT_FILENO);
	close_pipe(p);
	// ret = execute(p, e_tokens);
	execlp("ls", "ls", "-al", NULL);
	return (ret);
}

int	parent_process(t_prompt *p)
{
	t_pipex *pipex;
    t_fds   *fds;
	int	i;
    
	i = 1;
    pipex = p->pipex;
    fds = p->fds;
	if (fds->infile != -1 && fds->infile != -2)
		close(fds->infile);
	if (fds->outfile != -1 && fds->outfile != -2)
		close(fds->outfile);
	close_pipe(p);
	while (i != -1 || errno != ECHILD)
        i = waitpid(-1, NULL, 0);
	free(fds);
	free(pipex);
	if (p->tokens->nbr_pipe != 0)
		ft_free_fd(pipex->fd);
	return (0);
}

int	multiple_pipe(t_prompt *p, t_list_tokens *e_tokens)
{
    t_pipex *pipex;
    
    pipex = p->pipex;
	
	pipex->pid2[1] = fork();
	if (pipex->pid2[1] == -1)
		perror("fork error\n");
	if (pipex->pid2[1] == 0)
		child_process(p, e_tokens);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:12:22 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/08 15:21:59 by pmaimait         ###   ########.fr       */
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

// int	fds_pipex(t_prompt *p, t_list_tokens *e_tokens)
// {
// 	t_fds	*fds;
	
// 	fds = p->fds;
// 	if (fds->infile != -1 && fds->infile != -2)
// 		dup2(fds->infile, STDIN_FILENO);
// 	if (fds->outfile != -1 && fds->outfile != -2)
// 		dup2(fds->outfile, STDOUT_FILENO);
// }

int	multiple_pipe(t_prompt *p, t_list_tokens *e_tokens)
{
    t_pipex *pipex;
    t_fds   *fds;
    
    pipex = p->pipex;
    fds = p->fds;
	if (pipe(pipex->end) == -1)
		perror("pipe error\n");
    // fds_pipe(p, e_tokens);
	pipex->pid2[1] = fork();
	if (pipex->pid2[1] == -1)
		perror("fork error\n");
	if (pipex->pid2[1] == 0)
	{
		if (fds->outfile != -1 && fds->outfile != -2)
		{
			dup2(fds->outfile, STDOUT_FILENO);
			close(fds->outfile);
		}
		else
			dup2(pipex->end[1], STDOUT_FILENO);
		close(pipex->end[0]);
		close(pipex->end[1]);
		return (execute(p, e_tokens));
	}
	else
	{
		if (fds->infile != -1 && fds->infile != -2)
		{
			dup2(fds->infile, STDIN_FILENO);
			close(fds->infile);
		}
		else 
			dup2(pipex->end[0], STDIN_FILENO);
		close(pipex->end[0]);
		close(pipex->end[1]);
	}
	return (0);
}
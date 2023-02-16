/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:12:22 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/16 18:39:42 by pmaimait         ###   ########.fr       */
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
	t_list_tokens	*tmp;
    
    ret = CMD_NOT_FOUND;
	tmp = e_tokens;
 	while (tmp->type != STRING)
		tmp = tmp->next;
    /*if (ft_strncmp(tmp->str, "echo", 5) == 0 && p->nbr_pipe != 0)
        ret = execute_echo(p, tmp);
    if (ft_strncmp(tmp->str, "cd", 3) == 0 && p->nbr_pipe != 0)
		ret = execute_cd(p, tmp);
	else if (ft_strncmp(tmp->str, "env", 4) == 0 && p->nbr_pipe != 0)
		ret = execute_env(p, tmp);
	else if (ft_strncmp(tmp->str, "export", 7) == 0 && p->nbr_pipe != 0)
		ret = execute_export(p, tmp);
	else if (ft_strncmp(tmp->str, "pwd", 4) == 0 && p->nbr_pipe != 0)
		ret = execute_pwd(p, tmp);
	else if (ft_strncmp(tmp->str, "unset", 6) == 0 && p->nbr_pipe != 0)
		ret = execute_unset(p, tmp);
	else if (ft_strncmp(tmp->str, "exit", 5) == 0 && p->nbr_pipe != 0)
		ret = execute_exit(p, tmp);
    else */
	ret = execute_sys(p, tmp);
	return (ret);
}

int	child_process(t_prompt *p, t_list_tokens *e_tokens)
{
	int		**fd;
	int		index;
	int		ret;
    
    fd = p->pipex->fd;
	index = e_tokens->index;
	if (p->infile != -1 && p->infile != -2)
		dup2(p->infile, STDIN_FILENO);
	else if (index != 0)
		dup2(fd[index][0], STDIN_FILENO);
	if (p->outfile != -1 && p->outfile != -2)
		dup2(p->outfile, STDOUT_FILENO);
	else if (index != p->nbr_pipe)
		dup2(fd[index + 1][1], STDOUT_FILENO);
	close_pipe(p);
	ret = execute(p, e_tokens);
	return (ret);
}

int	close_free_pipe(t_prompt *p)
{
	close_pipe(p);
	if (p->nbr_pipe != 0)
		ft_free_fd(p);
	return (0);
}

int	multiple_pipe(t_prompt *p, t_list_tokens *e_tokens)
{
    t_pipex *pipex;
	int		ret;
    
    pipex = p->pipex;
	ret = 5;
	pipex->pid = fork();
	if (pipex->pid == -1)
		perror("fork error\n");
	if (pipex->pid == 0)
		ret = child_process(p, e_tokens);
	return (ret);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 12:23:08 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/22 15:44:06 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int execute_one_built(t_prompt *p, t_list_tokens *e_tokens)
{
    int ret;
    
    ret = 0;
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
    if (ft_strncmp(e_tokens->str, "echo", 5) == 0)
        ret = minishell_echo(p, e_tokens->next);
    // if (ft_strncmp(e_tokens->str, "cd", 3) == 0)
	// 	ret = execute_cd(p, e_tokens);
	else if (ft_strncmp(e_tokens->str, "env", 4) == 0)
		ret = minishell_env(p, e_tokens->next);
	// else if (ft_strncmp(e_tokens->str, "export", 7) == 0)
	// 	ret = execute_export(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "pwd", 4) == 0)
	// 	ret = execute_pwd(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "unset", 6) == 0)
	// 	ret = execute_unset(p, e_tokens);
	// else if (ft_strncmp(e_tokens->str, "exit", 5) == 0)
	// 	ret = execute_exit(p, e_tokens);
	close(STDIN_FILENO);
    return (ret);  
}

int execute_one_sys(t_prompt *p, t_list_tokens *e_tokens)
{
    int ret;
    
    ret = 0;
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
    return (ret);
}
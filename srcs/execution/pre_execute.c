/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/13 14:52:01 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_file(t_prompt *p, t_list_tokens *e_tokens)
{
    t_list_tokens   *tmp;
    t_fds       *fds;

    tmp = e_tokens;
    fds = p->fds;
    //check infile & create outfile
    while (tmp->type != PIPE && fds->infile != -1 && fds->outfile != -1 && tmp->type != END)
    {
        if (tmp->type == INPUT)
            fds->infile = open(tmp->next->str, O_RDONLY);
        if (tmp->type == R_DREDIR)
            fds->outfile = open(tmp->next->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        if (tmp->type == R_REDIR)
            fds->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0777);
        tmp = tmp->next;
    }
    if (fds->infile == -1)
        printf("%s: No such file or directory", tmp->next->str);
    if (fds->outfile == -1)
        printf("%s: Permission denied", tmp->next->str);
}


int     count_pipe(t_list_tokens *e_tokens)
{
    int i;

    i = 0;
    while (e_tokens)
    {
        if (e_tokens->type == PIPE)
            i++;
        e_tokens->index = i;
        e_tokens = e_tokens->next;
    }
    printf("number of pipe is : %d\n", i);
    return (i);
}

int     init_data(t_prompt *p)
{
    
    p->fds = malloc(sizeof(t_fds) * 1);
    p->pipex = malloc(sizeof(t_pipex) * 1);
    p->tokens->index = -1;
    p->fds->infile = -2;
    p->fds->outfile = -2;
    p->tokens->nbr_pipe = count_pipe(p->tokens);
    p->pipex->fd = malloc(sizeof(*fd[2]) * p->tokens->nbr_pipe);
	if (!p->pipex->fd)
		perror("malloc");
	while (p->tokens->nbr_pipe)
	{
		if (pipe(p->pipex->fd[p->tokens->nbr_pipe--]) == -1)
			perror("pipe error\n");
	}
    // p->fds->fd_in = STDIN_FILENO;
    // p->fds->fd_out = STDOUT_FILENO;
    return (0);
}

int start_execute(t_prompt *p)
{
    t_list_tokens   *curr;
    int ret;
    int i;

    curr = p->tokens;
    ret = 0;
    i = 0;
    while (curr)
    {
        open_file(p, curr);
        // if (curr->nbr_pipe == 0)
        //     one_command(p, curr);
        ret = multiple_pipe(p, curr);
        while (curr->type != PIPE && curr->type != END)
            curr = curr->next;
        curr = curr->next;  
    }
    while (i != -1 || errno != ECHILD)
        i = waitpid(-1, NULL, 0);
    return (ret);
}

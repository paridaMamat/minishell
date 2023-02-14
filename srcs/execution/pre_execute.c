/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/14 14:52:38 by pmaimait         ###   ########.fr       */
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
        {
            close(fds->infile);
            fds->infile = open(tmp->next->str, O_RDONLY);
        }
        if (tmp->type == R_DREDIR)
        {
            close(fds->outfile);   
            fds->outfile = open(tmp->next->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        }
        if (tmp->type == R_REDIR)
        {
            close(fds->outfile);
            fds->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0777);
        }
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
    int i;

    i = 1;
    p->fds = malloc(sizeof(t_fds) * 1);
    p->pipex = malloc(sizeof(t_pipex) * 1);
    p->tokens->index = -1;
    p->fds->infile = -2;
    p->fds->outfile = -2;
    p->tokens->nbr_pipe = count_pipe(p->tokens);
    if (p->tokens->nbr_pipe != 0)
    {
        p->pipex->fd = malloc(sizeof(*(p->pipex->fd)) * p->tokens->nbr_pipe * 2);
        if (p->pipex->fd == NULL)
            perror("malloc");    
        while (i <= p->tokens->nbr_pipe)
        {
            if (pipe(p->pipex->fd[i]) == -1)
                perror("pipe error\n");
            i++;
            // printf("nbr_pipe = %d\n", p->tokens->nbr_pipe);
            // printf("p->pipex->fd[p->tokens->nbr_pipe][0] = %d\n", p->pipex->fd[p->tokens->nbr_pipe][0]);
            // printf("p->pipex->fd[p->tokens->nbr_pipe][1] = %d\n", p->pipex->fd[p->tokens->nbr_pipe][1]);
        }
    }
    return (0);
}

int start_execute(t_prompt *p)
{
    t_list_tokens   *curr;
    int ret;

    curr = p->tokens;
    ret = 0;
    while (curr)
    {
        open_file(p, curr);
        dprintf(2, "index = %d\n", curr->index);
        // if (curr->nbr_pipe == 0)
        //     one_command(p, curr);
        ret = multiple_pipe(p, curr);
        while (curr->type != PIPE && curr->type != END)
            curr = curr->next;
        curr = curr->next;  
    }
    return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/15 15:25:40 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_file(t_prompt *p, t_list_tokens *e_tokens)
{
    t_list_tokens   *tmp;

    tmp = e_tokens;
    //check infile & create outfile
    while (tmp->type != PIPE && p->infile != -1 && p->outfile != -1 && tmp->type != END)
    {
        if (tmp->type == INPUT)
        {
            close(p->infile);
            p->infile = open(tmp->next->str, O_RDONLY);
        }
        if (tmp->type == R_DREDIR)
        {
            close(p->outfile);   
            p->outfile = open(tmp->next->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        }
        if (tmp->type == R_REDIR)
        {
            close(p->outfile);
            p->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0777);
        }
        tmp = tmp->next;
    }
    if (p->infile == -1)
        printf("%s: No such file or directory", tmp->next->str);
    if (p->outfile == -1)
        printf("%s: Permission denied", tmp->next->str);
}


int     count_pipe(t_prompt *p)
{
    int i;

    i = 0;
    while (p->tokens)
    {
        if (p->tokens->type == PIPE)
            i++;
        p->tokens->index = i;
        p->tokens = p->tokens->next;
    }
    printf("number of pipe is : %d\n", i);
    return (i);
}

int open_pipe(t_prompt *p)
{
    int i;

    i = 1;
    if (p->nbr_pipe != 0)
    {
        p->pipex->fd =(int **)malloc(sizeof(int *) * (p->nbr_pipe + 1));
        if (p->pipex->fd == NULL)
            return (free(p->pipex), perror("malloc"), 1);    
        dprintf(2, "nbr_pipe = %d\n", p->nbr_pipe);
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

int     init_data(t_prompt *p)
{
    int ret;

    ret = 0;
    p->pipex = malloc(sizeof(t_pipex) * 1);
    p->tokens->index = -1;
    p->infile = -2;
    p->outfile = -2;
    p->nbr_pipe = count_pipe(p);
    ret = open_pipe(p);
    return (ret);
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
        dprintf(1, "index = %d\nnbr_pipe = %d\n", curr->index, p->nbr_pipe);
        // if (curr->nbr_pipe == 0)
        //     one_command(p, curr);
        ret = multiple_pipe(p, curr);
        while (curr->type != PIPE && curr->type != END)
            curr = curr->next;
        curr = curr->next;  
    }
    return (ret);
}

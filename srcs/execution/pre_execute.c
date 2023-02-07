/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/07 15:58:39 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_file(t_prompt *p, t_list_tokens *e_tokens)
{
    t_list_tokens   *tmp;
    t_fds       *fds;
    int         i;

    i = 0;
    tmp = e_tokens;
    fds = p->fds;
    //check infile
    while (tmp->type != PIPE && tmp->type != END) 
    {
        if (tmp->type == INPUT)
            fds->infile = open(tmp->next->str, O_RDONLY);
        tmp = tmp->next;
    }  
    //create outfile
    tmp = e_tokens;
    while (tmp->type != PIPE && fds->infile != -1 && tmp->type != END)
    {
        if (tmp->type == R_DREDIR)
            fds->outfile = open(tmp->next->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        if (tmp->type == R_REDIR)
            fds->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0644);
        tmp = tmp->next;
    }
    if (fds->infile == -1)
        perror("infile");
    if (fds->outfile == -1)
        perror("outfile");
}


int     count_pipe(t_prompt *p, t_list_tokens *e_tokens)
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
    p->pipes = malloc(sizeof(*(p->pipes)) * i);
    if (p->pipes == NULL)
        perror("malloc");
    printf("number of pipe is : %d\n", i);
    return (i);
}




int     init_data(t_prompt *p)
{
    p->fds = malloc(sizeof(t_fds) * 1);
    p->pipes = malloc(sizeof(t_pipex) * 1);
    p->tokens->index = -1;
    p->fds->infile = -2;
    p->fds->outfile = -2;
    p->fds->fd_in = STDIN_FILENO;
    p->fds->fd_out = STDOUT_FILENO;
    return (0);
}

int start_execute(t_prompt *p)
{
    t_list_tokens   *curr;
    int     nbr_pipe;
    //int ret;

    curr = p->tokens;
    nbr_pipe = count_pipe(p, curr);
    while (curr)
    {
        open_file(p, curr);
        execute_sys(p, curr);
        multiple_pipe(p, curr);
        while (curr->type != PIPE && curr->type != END)
            curr = curr->next;
        curr = curr->next;  
    }
    return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/24 10:31:08 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_file(t_prompt *p, t_list_tokens *e_tokens)
{
    t_list_tokens   *tmp;

    tmp = e_tokens;
    if (p->infile != -2)
    {
        close(p->infile);
        p->infile = -2;
    }
    if (p->outfile != -2)
    {
        close(p->outfile);
        p->outfile = -2;
    }
    //check infile & create outfile
    while (tmp->type != PIPE && p->infile != -1 && p->outfile != -1 && tmp->type != END)
    {
        if (tmp->type == HEREDOC)
        {
            if (p->infile != -2)
                close(p->infile);
            p->infile = open(tmp->str, O_RDONLY);
        }
        if (tmp->type == INPUT)
        {
            if (p->infile != -2)
                close(p->infile);
            p->infile = open(tmp->next->str, O_RDONLY);
        }
        if (tmp->type == R_DREDIR)
        {
            if (p->outfile != -2)
                close(p->outfile);  
            p->outfile = open(tmp->next->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        }
        if (tmp->type == R_REDIR)
        {
            if (p->outfile != -2)
                close(p->outfile); 
            p->outfile = open(tmp->next->str, O_CREAT | O_RDWR | O_TRUNC, 0777);
        }
        if (p->infile == -1)
            printf("%s: No such file or directory\n", tmp->next->str);
        if (p->outfile == -1)
            printf("%s: Permission denied\n", tmp->next->str);
        tmp = tmp->next;
    }
    printf("index = %d  infile = %d  outfile = %d\n", e_tokens->index, p->infile, p->outfile);
}

int     count_pipe(t_prompt *p)
{
    int i;
    t_list_tokens   *tmp;

    i = 0;
    tmp = p->tokens;
    while (tmp)
    {
        if (tmp->type == PIPE)
            i++;
        tmp->index = i;
        tmp = tmp->next;
    }
    printf("number of pipe is : %d\n", i);
    return (i);
}

int open_pipe(t_prompt *p)
{
    int i;

    i = 0;
    p->pipex->fd = 0;
    if (p->nbr_pipe != 0)
    {
        p->pipex->fd =(int **)malloc(sizeof(int *) * (p->nbr_pipe + 2));
        if (p->pipex->fd == NULL)
            return (free(p->pipex), perror("malloc"), 1);
        p->pipex->fd[p->nbr_pipe + 1] = 0;
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
/*     p->pipex = malloc(sizeof(t_pipex) * 1);
    if (!p->pipex)
        return (perror("malloc"), 2); */
    p->infile = -2;
    p->outfile = -2;
    p->tokens->index = -1;
    p->nbr_pipe = count_pipe(p);
    ret = open_pipe(p);
    return (ret);
}

int start_execute(t_prompt *p)
{
    t_list_tokens   *curr;
    int ret;
    int	i;
        
    curr = p->tokens;
    i = 1;
    ret = 0;
    while (curr)
    {
        open_file(p, curr);
        if (p->infile != -1 && p->outfile != -1)
            ret = execute_cmd(p, curr);
        // dprintf(2, "execute_cmd (1) = %d\n", ret);
        while (curr->type != PIPE && curr->type != END)
            curr = curr->next;
        curr = curr->next;  
    }
    if (p->nbr_pipe != 0)
        close_free_pipe(p);
    while (i != -1 || errno != ECHILD)
        i = waitpid(-1, NULL, 0);
    //setup_signal_handlers();
    // dprintf(2, "execute_cmd (2) = %d\n", ret);
    return (ret);
}

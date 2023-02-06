/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/06 15:35:36 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_redirection(t_prompt *p, t_list_tokens *e_tokens)
{
    t_list_tokens   *tmp;
    t_fds       *fds;
    int         i;

    i = 0;
    tmp = e_tokens;
    fds = p->fds;
    fds->fd_in = STDIN_FILENO;
    fds->fd_out = STDOUT_FILENO;
    //check infile
    while (e_tokens->next->type !=  PIPE)
    {
        if (e_tokens->type == INPUT)
            fds->infile = open(e_tokens->next->str, O_RDONLY);
        e_tokens = e_tokens->next;
    }    




    
    if (e_tokens)
    {
        if (e_tokens->type == INPUT)
        {
            e_tokens->pipex->infile = open(e_tokens->str, O_RDONLY);
            if (e_tokens->pipex->infile != 0)
                // not excute that node of command, even will not create outfile, how to do that?
                printf("%s : No such file or directory", e_tokens->str);
        }
        if (e_tokens->type == R_DREDIR)
            e_tokens->pipex->outfile = open(e_tokens->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        if (e_tokens->type == R_REDIR)
            e_tokens->pipex->outfile = open(e_tokens->str, O_CREAT | O_RDWR | O_TRUNC, 0644);
        e_tokens = e_tokens->next;
    }
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

// void	multiple_pipe(t_pipex *pipex, char *av, char *envp[])
// {
// 	if (pipe(pipex->end) == -1)
// 		exit_perror("pipe error\n", 1);
// 	pipex->pid2[1] = fork();
// 	if (pipex->pid2[1] == -1)
// 		exit_perror("fork error\n", 1);
// 	if (pipex->pid2[1] == 0)
// 	{
// 		if (dup2(pipex->end[1], 1) == -1)
// 			exit_perror("dup2 fail\n", 1);
// 		if (pipex->outfile != -1)
// 			close(pipex->outfile);
// 		close(pipex->end[0]);
// 		close(pipex->end[1]);
// 		execute(pipex, av, envp);
// 	}
// 	else
// 	{
// 		close(pipex->end[1]);
// 		if (dup2(pipex->end[0], 0) == -1)
// 			exit_perror("dup2 fail\n", 1);
// 		close(pipex->end[0]);
// 	}
// }




int start_execute(t_prompt *p)
{
    t_list_tokens   *curr;
    int     nbr_pipe;
    //int ret;

    curr = p->tokens;
    nbr_pipe = count_pipe(p, curr);
    open_redirection(p, curr);
    



    //create all here_doc

    //prepare the command list for excution, create pipes , return false for error 
    // ret = make_pipe_and_fds(curr->fds);


    //open all the file in the list of command no matter we use it or not
    

    //seperate command list , that have to be start with single command who is accesseble 
    // while (curr)
    // {
    //     open_file(curr);
    //     if (curr->type ==  )
    //     curr = curr->next;
    // }
    return (0);
}

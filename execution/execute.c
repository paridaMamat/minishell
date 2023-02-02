/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:36:06 by pmaimait          #+#    #+#             */
/*   Updated: 2023/01/31 11:36:23 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    open_file(t_list_tokens *t)
{
    t_list_tokens   *curr;

    curr = t;
    if (curr)
    {
        if (curr->type == INPUT)
        {
            curr->pipe->infile = open(curr->str, O_RDONLY);
            if (curr->pipe->infile != 0)
                // not excute that node of command, even will not create outfile, how to do that?
                printf("%s : No such file or directory", curr->str);
        }
        if (curr->type == R_DREDIR)
            curr->pipe->outfile = open(curr->str, O_CREAT | O_WRONLY | O_APPEND, 0777);
        if (curr->type == R_REDIR)
            curr->pipe->outfile = open(curr->str, O_CREAT | O_RDWR | O_TRUNC, 0644);
        curr = curr->next;
    }
}


int start_excute(t_prompt *p)
{
    t_list_tokens   *curr;
    int ret;

    //create all here_doc

    //prepare the command list for excution, create pipes , return false for error 
    ret = make_pipe_and_fds(curr->fds);


    //open all the file in the list of command no matter we use it or not
    

    //seperate command list , that have to be start with single command who is accesseble 
    while (curr)
    {
        open_file(curr);
        if (curr->type ==  )
        curr = curr->next;
    }
}

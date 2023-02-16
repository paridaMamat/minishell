/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 23:17:00 by parida            #+#    #+#             */
/*   Updated: 2023/02/16 15:12:31 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    close_pipe(t_prompt *p)
{
    	int	i;
    
	i = 1;
	while (i <= p->nbr_pipe)
	{
		close(p->pipex->fd[i][0]);
		close(p->pipex->fd[i][1]);
		i++;
	}
	if (p->infile != -1 && p->infile != -2)
		close(p->infile);
	if (p->outfile != -1 && p->outfile != -2)
		close(p->outfile);
}
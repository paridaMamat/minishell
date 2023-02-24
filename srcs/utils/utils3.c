/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 23:17:00 by parida            #+#    #+#             */
/*   Updated: 2023/02/24 11:14:13 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    close_pipe(t_prompt *p)
{
    	int	i;
    
	i = 0;
	while (p->nbr_pipe && i <= p->nbr_pipe)
	{
		close(p->pipex->fd[i][0]);
		close(p->pipex->fd[i][1]);
		i++;
	}
	if (p->infile != -2)
		close(p->infile);
	if (p->outfile != -2)
		close(p->outfile);
}

int	close_free_pipe(t_prompt *p)
{
	close_pipe(p);
	if (p->nbr_pipe != 0)
		ft_free_fd(p);
	return (0);
}

int	is_built(char *str)
{
	if ((ft_strncmp(str, "echo", 5) == 0) || (ft_strncmp(str, "cd", 3) == 0)
	|| (ft_strncmp(str, "env", 4) == 0) || (ft_strncmp(str, "export", 7) == 0)
	|| (ft_strncmp(str, "pwd", 4) == 0) || (ft_strncmp(str, "unset", 6) == 0)
	|| (ft_strncmp(str, "exit", 5) == 0))
		return (1);
	return (0);
}
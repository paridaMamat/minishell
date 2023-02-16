/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:32:24 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/16 11:22:10 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ptr(void *thing)
{
	if (thing)
	{
		free(thing);
		thing = NULL;
	}
}

void	free_line(t_prompt *p)
{
	if (p->line)
		free_ptr(p->line);
	if (p->tokens)
	{
		lstclear_token(&p->tokens);
		free_ptr(p->tokens);
	}
	if (p->pipex)
		free_ptr(p->pipex);
}

void	exit_shell(t_prompt *data, int nb)
{
	if (data)
		free_all(data);
	exit(nb);
}

void	free_all(t_prompt *p)
{
	if (p)
	{
		if (p->pwd)
			free_ptr(p->pwd);
		if (p->p)
			free_ptr(p->p);
		if (p->env[0])
			ft_free_matrix(p->env);
		if (p->tokens)
			lstclear_token(&p->tokens);
	}
}

/*char	*find_str_i(char **env, char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (env[i] && env[i][0])
	{
		if (ft_strncmp(str, env[i], len) == 0)
			return (env[i] + (len + 1));
		i++;
	}
	return (NULL);
} */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:32:24 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/27 12:52:31 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_line(t_prompt *p)
{
	if (p->line)
	{
		free(p->line);
		p->line = NULL;
	}
	if (p->tokens)
	{
		lstclear_token(&p->tokens);
		free(p->tokens);
		p->tokens = NULL;
	}
}

void	exit_shell(t_prompt *data, int nb)
{
	if (data)
		free_all(data);
	exit(nb);
}

void free_all(t_prompt *p)
{
	if (p)
	{
		if (p->pwd)
			free(p->pwd);
		if (p->p)
			free(p->p);
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
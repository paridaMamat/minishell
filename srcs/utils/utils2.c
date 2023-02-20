/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:32:24 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/20 10:43:53 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static void	unlink_files(t_list_tokens **t)
{
	t_list_tokens	*tmp;

	tmp = *t;
	while (tmp->next != NULL)
	{
		if (tmp->type == HEREDOC)
			unlink(tmp->str);
		tmp = tmp->next;
	}
}

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
		unlink_files(&p->tokens);
		lstclear_token(&p->tokens);
		free_ptr(p->tokens);
	}
	if (p->pipex)
		free_ptr(p->pipex);
}

void	free_all(t_prompt *p)
{
	if (p)
	{
		if (p->line)
			free_ptr(p->line);
		if (p->pwd)
			free_ptr(p->pwd);
		if (p->p)
			free_ptr(p->p);
		if (p->env[0])
			ft_free_matrix(p->env);
		if (p->tokens)
			lstclear_token(&p->tokens);
		if (p->pipex)
			free_ptr(p->pipex);
	}
}

void	exit_shell(t_prompt *data, int nb)
{
	rl_clear_history();
	if (data)
		free_all(data);
	exit(nb);
}

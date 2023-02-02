/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:24:01 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/27 18:33:53 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_tokens	*add_new_token(char *str, int type)
{
	t_list_tokens	*new_node;

	new_node = malloc(sizeof(t_list_tokens) * 1);
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	lstadd_back_token(t_list_tokens **lst, t_list_tokens *new_node)
{
	t_list_tokens	*start;

	start = *lst;
	if (!start)
	{
		*lst = new_node;
		return ;
	}
	if (lst && *lst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

void	lstclear_token(t_list_tokens **lst)
{
	t_list_tokens	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free((*lst)->str);
		(*lst)->str = NULL;
		if ((*lst)->prev)
			(*lst)->prev->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->prev = (*lst)->prev;
		free((*lst));
		*lst = tmp;
	}
}

/* int	tokenize(t_prompt *p)
{
	t_list_tokens	*curr;
	int		i;

	i = 0;
	curr = p->tokens;
	while (curr->next != NULL)
	{
		if (curr->type == STRING)
		{
			handle_quotes
		}
		curr = curr->next;
	}
	return (1);
} */
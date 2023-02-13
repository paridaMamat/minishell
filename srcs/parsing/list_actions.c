/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_actions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 11:55:22 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/03 20:16:41 by mflores-         ###   ########.fr       */
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

int	lstadd_back_token(t_list_tokens **lst, t_list_tokens *new_node)
{
	t_list_tokens	*start;

	start = *lst;
	if (!new_node)
	{
		lstclear_token(lst);
		return (0);
	}
	else if (!start)
	{
		*lst = new_node;
		return (1);
	}
	else if (lst && *lst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
	return (1);
}

void	lstclear_token(t_list_tokens **lst)
{
	t_list_tokens	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free((*lst)->str);
		if ((*lst)->prev)
			(*lst)->prev->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->prev = (*lst)->prev;
		free_ptr((*lst));
		*lst = tmp;
	}
}

static int	get_len_strs(t_list_tokens *ptr)
{
	int	buffer;

	buffer = 0;
	while (ptr)
	{
		buffer = buffer + ft_strlen(ptr->str);
		ptr = ptr->next;
	}
	return (buffer);
}

char	*join_nodes(t_list_tokens *lst)
{
	t_list_tokens	*curr;
	char			*str;
	int				i;
	int				j;

	str = "";
	i = 0;
	j = 0;
	if (!lst)
		return (NULL);
	curr = lst;
	str = ft_calloc(get_len_strs(curr) + 1, sizeof(str));
	if (!str)
		return (NULL);
	while (curr)
	{
		i = -1;
		while (curr->str[++i])
			str[j++] = curr->str[i];
		curr = curr->next;
	}
	str[j] = '\0';
	lstclear_token(&lst);
	return (str);
}

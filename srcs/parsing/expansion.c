/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 15:16:28 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/08 16:26:24 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var(char *str)
{
	char	*res;

	res = NULL;
	if (str[0] == '$' && str[1] == '?')
	{
		free(str);
		return (ft_itoa(g_exit_code));
	}
	else if (str[0] == '$' && (str[1] == '$' || str[1] == '\0'))
	{
		free(str);
		return (ft_strdup("$"));
	}
	else
	{
		res = ft_strdup(getenv(str + 1));
		free(str);
		if (!res)
			return (ft_strdup(""));
		return (res);
	}
}

static int	expand_nodes_with_variables(t_list_tokens **ptr)
{
	t_list_tokens	*tmp;

	tmp = *ptr;
	while (tmp)
	{
		if (tmp->str[0] == '$' && tmp->str[1] == '\0' && tmp->next)
		{
			free(tmp->str);
			tmp->str = ft_strdup("");
			if (!tmp->str)
				return (0);
		}
		else if (ft_strchr(tmp->str, '$'))
		{
			tmp->str = get_var(tmp->str);
			if (!tmp->str)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

char	*get_dollar(char *str, int type)
{
	char			*new_str;
	t_list_tokens	*ptr_str;

	ptr_str = NULL;
	new_str = NULL;
	if (isolate_var(&ptr_str, str, type) == -1)
		return (NULL);
	if (!expand_nodes_with_variables(&ptr_str))
	{
		lstclear_token(&ptr_str);
		return (NULL);
	}
	new_str = join_nodes(ptr_str);
	free(str);
	if (!new_str)
		return (NULL);
	return (new_str);
}

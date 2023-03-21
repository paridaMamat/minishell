/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 15:16:28 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/20 14:20:49 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var(char **env, char *str)
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
		if (!res)
		{
			res = ft_strdup(get_env_var(env, str + 1));
			if (!res)
				return (free(str), ft_strdup(""));
		}
		return (free(str), res);
	}
}

static int	expand_nodes_with_variables(char **env, t_list_tokens **ptr, \
int heredoc)
{
	t_list_tokens	*tmp;

	tmp = *ptr;
	while (tmp)
	{
		if (tmp->str[0] == '$' && tmp->str[1] == '\0' && tmp->next
			&& heredoc == 0 && (tmp->type != D_QUOTE && tmp->type != S_QUOTE \
			&& tmp->type != STRING))
		{
			free(tmp->str);
			tmp->str = ft_strdup("");
			if (!tmp->str)
				return (0);
		}
		else if (ft_strchr(tmp->str, '$'))
		{
			tmp->str = get_var(env, tmp->str);
			if (!tmp->str)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

char	*get_dollar(char **env, char *str, int type, int heredoc)
{
	char			*new_str;
	t_list_tokens	*ptr_str;

	ptr_str = NULL;
	new_str = NULL;
	if (isolate_var(&ptr_str, str, type) == -1)
		return (NULL);
	if (!expand_nodes_with_variables(env, &ptr_str, heredoc))
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

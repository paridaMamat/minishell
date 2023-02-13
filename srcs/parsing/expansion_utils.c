/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 16:27:32 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/07 12:52:11 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_delimiter(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '$' && (str[i + 1] == '\0'))
		return (1);
	else if (str[i] == '$' && (str[i + 1] == '$' || str[i + 1] == '?'))
		return (2);
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

static int	create_node_for_var(t_list_tokens **ptr, int *i, \
char *str, int type)
{
	char	*tmp;
	int		delimiter;

	tmp = NULL;
	delimiter = find_delimiter(str + *i);
	tmp = ft_substr(str, (*i), delimiter);
	if (!tmp)
		return (-1);
	if (!lstadd_back_token(ptr, add_new_token(tmp, type)))
		return (-1);
	*i = *i + delimiter;
	return (1);
}

static int	create_node_for_rest(t_list_tokens **ptr, int *i, \
char *str, int type)
{
	char	*tmp;
	int		delimiter;

	tmp = NULL;
	delimiter = *i;
	while (str[(*i)] && str[(*i)] != '$')
		(*i)++;
	tmp = ft_substr(str, delimiter, (*i) - delimiter);
	if (!tmp)
		return (-1);
	if (!lstadd_back_token(ptr, add_new_token(tmp, type)))
		return (-1);
	return (1);
}

int	isolate_var(t_list_tokens **ptr, char *str, int type)
{
	int		i;
	int		start;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{	
			if (create_node_for_var(ptr, &i, str, type) == -1)
				return (-1);
		}
		else if (str[i] != '$')
		{
			if (create_node_for_rest(ptr, &i, str, type) == -1)
				return (-1);
		}
	}
	return (1);
}

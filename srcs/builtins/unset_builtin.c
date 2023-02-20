/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:48:39 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/16 09:55:54 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_unset(t_prompt *data, char **args)
{
	int	i;
	int	index;
	int	res;

	res = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_var_key(args[i]) || ft_strchr(args[i], '=') != NULL)
		{
            ft_putstr_fd("minishell: unset: not a valid identifier", STDOUT_FILENO);
			res = 1;
		}
		else
		{
			index = get_env_var_index(data->env, args[i]);
			if (index != -1)
				remove_env_var(data, index);
		}
		i++;
	}
	return (res);
}
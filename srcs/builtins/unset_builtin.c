/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:48:39 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/24 14:28:22 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//function check str in env , if there is double return -2, if str is not in 
// env it return -1, if str is the env return index of his position
int	check_in_env(t_prompt *p, char *str)
{
	int	i;
	int	len;
	int	d_flag;
	int	index;

	i = 0;
	len = ft_strlen(str);
	d_flag = -1;
	index = -1;
	while (p->env[i])
	{
		if (ft_strncmp(p->env[i], str, len) == 0 && p->env[i][len] == '=')
		{
			d_flag++;
			index = i;
		}	
		i++;
	}
	if (d_flag != 0)
		return (-2);
	else
		return (index);
}

int	minishell_unset(t_prompt *p, t_list_tokens *e_tokens)
{
	int	i;
	int	index;
	int	res;

	res = 0;
	i = 1;
	while (p->env[i])
	{
		if (!is_valid_env_var_key(p->env[i]) || ft_strchr(p->env[i], '=') != NULL)
		{
            ft_putstr_fd("minishell: unset: not a valid identifier", STDOUT_FILENO);
			res = 1;
		}
		else
		{
			index = get_env_var_index(p->env, p->env[i]);
			if (index != -1)
				remove_env_var(p, index);
		}
		i++;
	}
	return (res);
}
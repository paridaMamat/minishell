/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:48:39 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/16 14:54:32 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//function check str in env  if str is not in 
// env it return -1, if str is the env return index of his position
int	check_in_env(t_prompt *p, char *str)
{
	int	i;
	int	len;
	int	index;

	i = 0;
	len = ft_strlen(str);
	index = -1;
	while (p->env[i])
	{
		if (ft_strncmp(p->env[i], str, len) == 0 && p->env[i][len] == '=')
			index = i;
		i++;
	}
	return (index);
}

static int	take_off(t_prompt *p, int i, int index)
{
	char	**tmp;

	tmp = (char **)malloc(sizeof(char *) * ft_matrixlen(p->env));
	if (!tmp)
		return (write(2, "malloc error\n", 14), 1);
	while (p->env[i] && index > -1)
	{
		if (i < index)
			tmp[i] = ft_strdup(p->env[i]);
		else if (i > index)
			tmp[i - 1] = ft_strdup(p->env[i]);
		i++;
	}
	tmp[i - 1] = 0;
	ft_free_matrix(p->env);
	p->env = ft_dup_matrix(tmp);
	ft_free_matrix(tmp);
	return (0);
}

int	minishell_unset(t_prompt *p, t_list_tokens *e_tokens)
{
	int		i;
	int		index;

	while (e_tokens->type != STRING && e_tokens->type != END
		&& e_tokens->type != PIPE)
		e_tokens = e_tokens->next;
	while (e_tokens->type != END && e_tokens->type != PIPE)
	{
		i = 0;
		if (e_tokens->type == STRING)
		{
			index = check_in_env(p, e_tokens->str);
			if (index > -1)
				g_exit_code = take_off(p, i, index);
		}
		e_tokens = e_tokens->next;
	}
	return (g_exit_code);
}

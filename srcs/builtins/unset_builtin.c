/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:48:39 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/08 19:00:14 by parida           ###   ########.fr       */
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
	printf("index = %d\n", index);
	return (index);
}

int	minishell_unset(t_prompt *p, t_list_tokens *e_tokens)
{
	int	i;
	int	index;
	char	**tmp;

	while (e_tokens->type != STRING && e_tokens->type != END && e_tokens->type != PIPE)
		e_tokens = e_tokens->next;
	while (e_tokens->type != END && e_tokens->type != PIPE)
	{
		i = 0;
		if (e_tokens->type == STRING)
		{
			index = check_in_env(p, e_tokens->str);
			tmp = (char **)malloc(sizeof(char *) * ft_matrixlen(p->env));
			if (!tmp)
				return (write(2, "malloc error\n", 14), 1);
			while (p->env[i] && index > -1)
			{
				if (i < index)
					tmp[i] = p->env[i];
				else if (i > index)
					tmp[i - 1] = p->env[i];
				i++;
			}
			tmp[i - 1] = NULL;
			ft_free_matrix(p->env);
			p->env = tmp;
			ft_free_matrix(tmp);
		}
		e_tokens = e_tokens->next;
	}
	return (0);
}

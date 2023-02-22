/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:41:53 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/22 15:24:06 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int minishell_env(t_prompt *p, t_list_tokens *e_tokens)
{
	int	i;

	if (e_tokens->type != END && e_tokens->type != PIPE) 
		return (ft_putendl_fd("minishell: env: too many arguments", STDERR_FILENO), 1);
	i = 0;
	if (!p->env)
		return (1);
	while (p->env[i])
		ft_putendl_fd(p->env[i++], STDOUT_FILENO);
	return (0);
}
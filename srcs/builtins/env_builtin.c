/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:41:53 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/15 19:45:05 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int minishell_env(t_prompt *data, char **args)
{
	int	i;

	if (args && args[1])
		return (ft_putendl_fd("minishell: env: too many arguments", STDERR_FILENO), 1);
	i = 0;
	if (!data->env)
		return (1);
	while (data->env[i])
		ft_putendl_fd(data->env[i++], STDOUT_FILENO);
	return (0);
}
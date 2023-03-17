/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:41:53 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/16 16:09:56 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_env(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
	int	i;

	if (e_tokens->type != END && e_tokens->type != PIPE)
		return (ft_putendl_fd("minishell: env: too many arguments",
				STDERR_FILENO), 1);
	i = 0;
	if (!p->env)
		return (1);
	while (p->env[i])
		ft_putendl_fd(p->env[i++], fd);
	return (0);
}

int	print_perror_export(char *arg)
{
	ft_putstr_fd("minishell: export :'", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("' not a valide identifier", 2);
	g_exit_code = 1;
	return (g_exit_code);
}

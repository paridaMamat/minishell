/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:24:36 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/15 19:40:19 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_pwd(t_prompt *data)
{
	char	buffer[BUFSIZ];
	char	*pwd;

	if (data->pwd)
	{
		ft_putendl_fd(data->pwd, STDOUT_FILENO);
		return (0);
	}
	pwd = getcwd(buffer, BUFSIZ);
	if (pwd)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		return (0);
	}
	return (1);
}
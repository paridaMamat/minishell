/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:24:36 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/07 15:06:34 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_pwd(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
	char	buffer[BUFSIZ];
	char	*pwd;

	(void)e_tokens;
	if (p->pwd)
	{
		ft_putendl_fd(p->pwd, fd);
		return (0);
	}
	pwd = getcwd(buffer, BUFSIZ);
	if (pwd)
	{
		ft_putendl_fd(pwd, fd);
		return (0);
	}
	return (1);
}

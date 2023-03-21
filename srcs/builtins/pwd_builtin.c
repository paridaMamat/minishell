/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:24:36 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/16 15:15:45 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var(char **env, char *var)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	free_ptr(tmp);
	return (NULL);
}

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
	else if (!pwd)
	{
		pwd = get_var(p->env, "PWD");
		if (pwd)
		{
			ft_putendl_fd(pwd, fd);
			return (0);
		}
	}
	return (1);
}

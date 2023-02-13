/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 12:37:43 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/02 19:58:18 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

static char	*pimp_pwd(char **path, char *username)
{
	char	*tmp;
	char	*tmp2;
	char	*color;

	if (*path)
		free(*path);
	*path = getcwd(NULL, 0);
	if (!*path)
		return (NULL);
	tmp2 = ft_strnstr(*path, username, ft_strlen(*path));
	color = ft_strjoin(GREEN, "\001~\002");
	if (!color)
		return (NULL);
	tmp = ft_strjoin(color, tmp2 + ft_strlen(username));
	free(color);
	if (!tmp)
		return (NULL);
	color = ft_strjoin(tmp, "$\001\e[0m\002 ");
	free(tmp);
	return (color);
}

char	*get_prompt(t_prompt *p)
{
	char	*pwd;
	char	*tmp;
	char	*tmp2;

	p->user = getenv("USER");
	if (!p->user)
		return (NULL);
	tmp2 = MAGENTA;
	tmp = ft_strjoin(tmp2, p->user);
	if (!tmp)
		return (NULL);
	tmp2 = ft_strjoin(tmp, PROMPT_HOSTNAME);
	free(tmp);
	if (!tmp2)
		return (NULL);
	pwd = pimp_pwd(&p->pwd, p->user);
	if (!pwd)
	{
		free(tmp2);
		return (NULL);
	}
	tmp = ft_strjoin(tmp2, pwd);
	free(tmp2);
	free(pwd);
	return (tmp);
}

int	init_prompt(t_prompt *p, char **env)
{
	g_exit_code = 0;
	p->env = ft_dup_matrix(env);
	if (!p->env)
		return (0);
	p->tokens = NULL;
	p->user = NULL;
	p->pwd = NULL;
	p->p = NULL;
	return (1);
}

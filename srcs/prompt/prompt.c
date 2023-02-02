/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 12:37:43 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/27 11:14:30 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

static char	*pimp_pwd(char *path, char *username)
{
	char	*tmp;
	char	*tmp2;
	char	*color;

	tmp2 = ft_strnstr(path, username, ft_strlen(path));
	color = ft_strjoin(GREEN, "\001~\002");
	tmp = ft_strjoin(color, tmp2 + ft_strlen(username));
	free(color);
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
	if (ft_strncmp(p->user, "root", 4) == 0)
		tmp2 = RED;
	else
		tmp2 = MAGENTA;
	tmp = ft_strjoin(tmp2, p->user);
	tmp2 = ft_strjoin(tmp, PROMPT_HOSTNAME);
	free(tmp);
	if (p->pwd)
		free(p->pwd);
	p->pwd = getcwd(NULL, 0);
	if (!p->pwd)
	{
		free(tmp2);
		return (NULL);
	}
	else
		pwd = pimp_pwd(p->pwd, p->user);
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
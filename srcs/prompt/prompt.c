/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 12:37:43 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/20 12:51:47 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

static char	*pimp_me(char *tmp2, char *username, char **tmp, int flag)
{
	char	*color;

	if (flag)
		color = ft_strjoin(GREEN, "\001\002");
	else
		color = ft_strjoin(GREEN, "\001~\002");
	if (!color)
		return (NULL);
	if (flag)
		*tmp = ft_strjoin(color, tmp2);
	else
		*tmp = ft_strjoin(color, tmp2 + ft_strlen(username));
	free(color);
	if (!*tmp)
		return (NULL);
	color = ft_strjoin(*tmp, "$\001\e[0m\002 ");
	return (color);
}

static char	*pimp_pwd(char **path, char *username, char **env)
{
	char	*tmp;
	char	*tmp2;
	char	*color;
	int		flag;

	flag = 0;
	if (*path)
		free(*path);
	*path = getcwd(NULL, 0);
	if (!*path)
	{
		*path = ft_strdup(get_env_var(env, "PWD"));
		if (!*path)
			return (NULL);
	}
	tmp2 = ft_strnstr(*path, username, ft_strlen(*path));
	if (!tmp2)
	{
		tmp2 = ft_strnstr(*path, "/", ft_strlen(*path));
		if (!tmp2)
			return (NULL);
		flag = 1;
	}
	color = pimp_me(tmp2, username, &tmp, flag);
	return (free(tmp), color);
}

char	*get_env_var(char **env, char *var)
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
	pwd = pimp_pwd(&p->pwd, p->user, p->env);
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
	p->pipex = malloc(sizeof(t_pipex) * 1);
	if (!p->pipex)
	{
		ft_free_matrix(p->env);
		return (perror("malloc"), 0);
	}
	return (1);
}

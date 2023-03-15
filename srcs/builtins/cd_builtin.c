/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 20:02:02 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/14 20:45:14 by mflores-         ###   ########.fr       */
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

static void	set_var(char **env, char *var, char *val)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return ;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free(env[i]);
			env[i] = ft_strjoin(tmp, val);
			free_ptr(tmp);
			return ;
		}
		i++;
	}
	free_ptr(tmp);
	return ;
}

static	int	chdir_err_msg(char *path)
{
	if (errno == ESTALE)
		errno = ENOENT;
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (0);
}

static int	change_dir(t_prompt *p, char *path)
{
	char	*ret;
	char	cwd[BUFSIZ];

	ret = NULL;
	if (chdir(path) != 0)
		return (chdir_err_msg(path));
	ret = getcwd(cwd, BUFSIZ);
	if (!ret)
	{
		ft_putstr_fd("cd: error retrieving current directory ", \
		STDERR_FILENO);
		ft_putstr_fd("getcwd: cannot access parent directories ", \
		STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		ret = get_var(p->env, "PWD");
	}
	else
		ret = ft_strdup(cwd);
	set_var(p->env, "OLDPWD", get_var(p->env, "PWD"));
	set_var(p->env, "PWD", ret);
	free_ptr(ret);
	return (1);
}

int	minishell_cd(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
	char	*path;

	if (e_tokens->type == STRING && e_tokens->str[0] == '\0')
		return (0);
	if ((e_tokens->type == STRING && ft_strncmp(e_tokens->str, "--", 3) == 0)
		|| e_tokens->type != STRING)
	{
		path = get_var(p->env, "HOME");
		if (!path || *path == '\0' || ft_isspace(*path))
			return (ft_putendl_fd(ERR_CD_MSG1, STDERR_FILENO), 1);
		return (!change_dir(p, path));
	}
	if (e_tokens->next->type == STRING)
		return (ft_putendl_fd(ERR_CD, STDERR_FILENO), 1);
	if (e_tokens->type == STRING && ft_strncmp(e_tokens->str, "-", 2) == 0)
	{
		path = get_var(p->env, "OLDPWD");
		if (!path)
			return (ft_putendl_fd(ERR_CD_MSG2, STDERR_FILENO), 1);
		ft_putendl_fd(path, fd);
		return (!change_dir(p, path));
	}
	return (!change_dir(p, e_tokens->str));
}

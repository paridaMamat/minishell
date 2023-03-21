/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_sys.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 14:41:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/21 13:43:37 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cmd(char **path, char *cmd)
{
	int		i;
	int		fd;
	char	*path_cmd;
	char	*path_cm;

	if (cmd)
	{
		fd = access(cmd, X_OK);
		if (fd != -1)
			return (cmd);
	}
	i = 0;
	while (path[i] && cmd)
	{
		path_cm = ft_strjoin(path[i], "/");
		path_cmd = ft_strjoin(path_cm, cmd);
		free(path_cm);
		fd = access(path_cmd, X_OK);
		if (fd != -1)
			return (path_cmd);
		free(path_cmd);
		i++;
	}
	return (NULL);
}

static char	**get_path(t_pipex *pipex, char **envp, char *cmd)
{
	char	*path;

	if (*envp == NULL || envp == NULL || envp[0][0] == 0)
	{
		free(pipex->cmd_arg);
		ft_putendl_fd("minishell: env: is empty", STDERR_FILENO);
		return (0);
	}
	while (*envp && ft_strncmp("PATH=", *envp, 5))
		envp++;
	if (*envp == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (NULL);
	}
	path = *envp + 5;
	return (ft_split(path, ':'));
}

static char	**create_cmd_arg(t_list_tokens *e_tokens)
{
	char			**array;
	int				i;
	int				count;
	t_list_tokens	*tmp;

	tmp = e_tokens;
	i = 0;
	count = 0;
	while (tmp->type != PIPE && tmp->type == STRING)
	{
		tmp = tmp->next;
		count++;
	}
	array = (char **)malloc(sizeof(char *) * (count + 2));
	if (array == NULL)
		return (ft_putendl_fd(ERR_MALLOC, STDERR_FILENO), NULL);
	tmp = e_tokens;
	while (tmp->type != PIPE && tmp->type != END)
	{
		if (tmp->type == STRING)
			array[i++] = ft_strdup(tmp->str);
		tmp = tmp->next;
	}
	array[i] = 0;
	return (array);
}

static int	path_and_execve(t_prompt *p, t_list_tokens *e_tokens)
{
	int		result;

	result = 0;
	p->pipex->cmd_arg = create_cmd_arg(e_tokens);
	if (access(e_tokens->str, X_OK) == 0)
	{
		result = execve(e_tokens->str, p->pipex->cmd_arg, p->env);
		result = get_result(p, e_tokens, result);
	}
	else
	{
		p->pipex->path = get_path(p->pipex, p->env, e_tokens->str);
		if (p->pipex->path == NULL)
		{
			ft_free_matrix(p->pipex->cmd_arg);
			if (p->outfile != 0)
				close_free_pipe(p);
			exit_shell(p, g_exit_code);
		}
		p->pipex->cmd = get_cmd(p->pipex->path, e_tokens->str);
		ft_free_matrix(p->pipex->path);
		result = get_result(p, e_tokens, result);
		free(p->pipex->cmd);
	}
	return (ft_free_matrix(p->pipex->cmd_arg), result);
}

int	execute_sys(t_prompt *p, t_list_tokens *e_tokens)
{
	int			result;

	result = path_and_execve(p, e_tokens);
	if (result == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(e_tokens->str, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		g_exit_code = 127;
	}
	if (p->nbr_pipe != 0)
		ft_free_fd(p);
	exit_shell(p, g_exit_code);
	return (result);
}

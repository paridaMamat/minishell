/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 14:41:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/03 14:47:36 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd(char **path, char *cmd)
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

char	**get_path(t_pipex *pipex, char **envp)
{
	char	*path;

	if (*envp == NULL || envp == NULL || envp[0][0] == 0)
	{
		free(pipex->cmd_arg);
		printf("PATH is empty\n");
        return (0);
	}
	while (ft_strncmp("PATH=", *envp, 5))
		envp++;
	path = *envp + 5;
	return (ft_split(path, ':'));
}

void	execute(t_pipex *pipex, char *cmd, char **envp)
{
	int	result;

	pipex->cmd_arg = ft_split(cmd, ' ');
	if (access(pipex->cmd_arg[0], X_OK) != -1)
	{
		result = execve(pipex->cmd_arg[0], pipex->cmd_arg, NULL);
		free_array(pipex->cmd_arg);
	}
	else
	{
		pipex->path = get_path(pipex, envp);
		pipex->cmd = get_cmd(pipex->path, pipex->cmd_arg[0]);
		ft_free_matrix(pipex->path);
		if (pipex->cmd != NULL)
			result = execve(pipex->cmd, pipex->cmd_arg, envp);
		else
			result = -1;
		free(pipex->cmd);
		free_array(pipex->cmd_arg);
	}
	if (result == -1)
		printf("command not found2\n");
}
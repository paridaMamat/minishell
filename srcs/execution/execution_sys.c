/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_sys.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 14:41:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/16 17:24:46 by pmaimait         ###   ########.fr       */
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

char	**create_cmd_arg(t_list_tokens *e_tokens)
{
	char	**array;
	int		i;
	int		count;
	t_list_tokens	*tmp;
	
	tmp = e_tokens;
	i = 0;
	count = 0;
	while (tmp->type != PIPE && tmp->type == STRING)
	{
		tmp = tmp->next;
		count++;
	}
	array = (char **)malloc(sizeof(char *) * (count + 1));
	if (array == NULL) 
		return (perror("malloc"), NULL);
	tmp = e_tokens;
	while (tmp->type != PIPE && tmp->type != END)
	{
		if (tmp->type == STRING)
		{
			array[i] = ft_strdup(tmp->str);
			i++;
		}
		tmp = tmp->next;
	}
	array[i] = 0;
	return (array);
}

int	execute_sys(t_prompt *p, t_list_tokens *e_tokens)
{
	int	result;
	t_pipex		*pipex;
	int	i;

	i = 0;
	pipex = p->pipex;
	while (e_tokens->type != PIPE && e_tokens->type != END)
	{
		if (e_tokens->type == STRING)
		{
			pipex->cmd_arg = create_cmd_arg(e_tokens);
			if (access(e_tokens->str, X_OK) != -1)
				result = execve(e_tokens->str, pipex->cmd_arg, p->env);
			else
			{
				pipex->path = get_path(pipex, p->env);
				pipex->cmd = get_cmd(pipex->path, e_tokens->str);
				ft_free_matrix(pipex->path);
				if (pipex->cmd != NULL)
					result = execve(pipex->cmd, pipex->cmd_arg, p->env);
				else
					result = -1;
				free(pipex->cmd);
			}
			ft_free_matrix(pipex->cmd_arg);	
			if (result == -1)
			{
				perror("Command not found\n");
				g_exit_code = CMD_NOT_FOUND;
				return(CMD_NOT_FOUND);
			}
		}
		e_tokens = e_tokens->next;
	}
	return (result);
}

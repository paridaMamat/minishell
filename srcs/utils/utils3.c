/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 23:17:00 by parida            #+#    #+#             */
/*   Updated: 2023/03/17 15:27:37 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(t_prompt *p)
{
	int	i;

	i = 0;
	while (p->nbr_pipe && i <= p->nbr_pipe)
	{
		close(p->pipex->fd[i][0]);
		close(p->pipex->fd[i][1]);
		i++;
	}
	if (p->infile != -2 && p->infile != -1)
		close(p->infile);
	if (p->outfile != -2 && p->outfile != -1)
		close(p->outfile);
}

int	close_free_pipe(t_prompt *p)
{
	close_pipe(p);
	if (p->nbr_pipe != 0)
		ft_free_fd(p);
	return (0);
}

int	is_built(char *str)
{
	if ((ft_strncmp(str, "echo", 5) == 0) || (ft_strncmp(str, "cd", 3) == 0)
		|| (ft_strncmp(str, "env", 4) == 0)
		|| (ft_strncmp(str, "export", 7) == 0)
		|| (ft_strncmp(str, "pwd", 4) == 0)
		|| (ft_strncmp(str, "unset", 6) == 0)
		|| (ft_strncmp(str, "exit", 5) == 0))
		return (1);
	return (0);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new_str;
	size_t	len_s1_s2;
	size_t	i;
	size_t	x;

	if (!s1 || !s2)
		return (NULL);
	len_s1_s2 = ft_strlen(s1) + ft_strlen(s2) + 1;
	new_str = malloc(sizeof(char) * len_s1_s2);
	if (!new_str)
		return (NULL);
	i = 0;
	x = 0;
	while (s1[i])
		new_str[x++] = s1[i++];
	i = 0;
	while (s2[i])
		new_str[x++] = s2[i++];
	new_str[x] = 0;
	free(s1);
	free(s2);
	return (new_str);
}

int	print_export(t_prompt *p, int fd)
{
	int		i;
	int		j;
	char	**env;

	i = 0;
	env = p->env;
	while (env[i])
	{
		j = 0;
		ft_putstr_fd("declare -x ", fd);
		while (env[i][j])
		{
			ft_putchar_fd(env[i][j], fd);
			if (env[i][j] == '=')
				ft_putchar_fd('"', fd);
			j++;
		}
		ft_putstr_fd("\"\n", fd);
		i++;
	}
	return (0);
}

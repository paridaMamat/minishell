/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 15:07:26 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/15 15:44:44 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_matrixlen(char **m)
{
	int	i;

	i = 0;
	while (m && m[i])
		i++;
	return (i);
}

void	ft_free_matrix(char **m)
{
	int	i;

	i = 0;
	while (m && m[i])
	{
		free(m[i]);
		i++;
	}
	if (m)
	{
		free(m);
		m = NULL;
	}
}

char	**ft_extend_matrix(char **in, char *newstr)
{
	char	**out;
	int		len;
	int		i;

	i = -1;
	out = NULL;
	if (!newstr)
		return (in);
	len = ft_matrixlen(in);
	out = malloc(sizeof(char *) * (len + 2));
	out[len + 1] = NULL;
	if (!out)
		return (in);
	while (++i < len)
	{
		out[i] = ft_strdup(in[i]);
		if (!out[i])
		{
			ft_free_matrix(in);
			ft_free_matrix(out);
		}
	}
	out[i] = ft_strdup(newstr);
	ft_free_matrix(in);
	return (out);
}

char	**ft_dup_matrix(char **m)
{
	char	**res;
	int		n_rows;
	int		i;

	if (!m)
		return (NULL);
	i = 0;
	n_rows = ft_matrixlen(m);
	res = malloc(sizeof(char *) * (n_rows + 1));
	if (!res)
		return (NULL);
	while (m[i])
	{
		res[i] = ft_strdup(m[i]);
		if (!res[i])
		{
			ft_free_matrix(res);
			return (NULL);
		}
		i++;
	}
	res[i] = NULL;
	return (res);
}

void	ft_free_fd(int	**fd)
{
	int	i;

	i = 1;
	while (*fd && fd[i])
	{
		free(fd[i]);
		i++;
	}
	if (fd)
		free(fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 15:28:44 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/16 10:39:04 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*char	*find_str_i(char **env, char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (env[i] && env[i][0])
	{
		if (ft_strncmp(str, env[i], len) == 0)
			return (env[i] + (len + 1));
		i++;
	}
	return (NULL);
} */
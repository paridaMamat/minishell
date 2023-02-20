/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 20:02:02 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/16 09:54:21 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_cd(t_prompt *data)
{
	char	**str[2];
	char	*aux;

	g_exit_code = 0;
	//str[0] = ((t_mini *)data->cmds->content)->full_cmd;
	aux = minishell_getenv("HOME", data->env, 4);
	if (!aux)
		aux = ft_strdup("");
	str[1] = ft_extend_matrix(NULL, aux);
	free(aux);
	aux = getcwd(NULL, 0);
	str[1] = ft_extend_matrix(str[1], aux);
	free(aux);
	cd_error(str);
	if (!g_exit_code)
		data->env = mini_setenv("OLDPWD", str[1][1], data->env, 6);
	aux = getcwd(NULL, 0);
	if (!aux)
		aux = ft_strdup("");
	str[1] = ft_extend_matrix(str[1], aux);
	free(aux);
	data->env = mini_setenv("PWD", str[1][2], data->env, 3);
	ft_free_matrix(&str[1]);
	return (g_exit_code);
}

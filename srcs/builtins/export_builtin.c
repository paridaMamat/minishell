/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 09:21:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/16 16:58:14 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int	minishell_export(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
	while (e_tokens->type != STRING && e_tokens->type != END
		&& e_tokens->type != PIPE)
		e_tokens = e_tokens->next;
	if (e_tokens->type == END || e_tokens->type == PIPE)
		print_export(p, fd);
	else
		export_arg(p, e_tokens);
	return (g_exit_code);
}

static char	*make_arg(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = (char *)malloc(sizeof(char) * ft_strlen(str));
	if (!tmp)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			i++;
		tmp[j++] = str[i++];
	}
	tmp[j] = '\0';
	return (tmp);
}

int	add_to_env(t_prompt *p, char *arg, int i)
{
	char	*str;

	str = ft_substr(arg, 0, i);
	if (check_in_env(p, str) == -1)
	{
		if (arg[i] == '+')
		{
			arg = make_arg(arg);
			p->env = ft_extend_matrix(p->env, arg);
			free(arg);
		}
		else
			p->env = ft_extend_matrix(p->env, arg);
	}
	else if (check_in_env(p, str) > -1)
		add_or_replace_env(p, arg, str);
	free(str);
	return (0);
}

int	export_arg(t_prompt *p, t_list_tokens *e_tokens)
{
	char	*arg;
	int		i;

	while (e_tokens->type != END && e_tokens->type != PIPE)
	{
		if (e_tokens->type == STRING)
		{
			i = 0;
			g_exit_code = 0;
			arg = e_tokens->str;
			while (arg[i] && (ft_isalpha(arg[i]) || ft_isalnum(arg[i])
					|| arg[i] == '_'))
				i++;
			if (i != 0 && (arg[i] == '=' || (arg[i] == '+'
						&& arg[i + 1] == '=')))
				add_to_env(p, arg, i);
			else if (arg[i])
				print_perror_export(arg);
		}
		e_tokens = e_tokens->next;
	}
	return (g_exit_code);
}

int	add_or_replace_env(t_prompt *p, char *line, char *str)
{
	int	i;
	int	index;

	i = 0;
	index = check_in_env(p, str);
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	if (line[i] == '=')
	{
		free(p->env[index]);
		p->env[index] = ft_strdup(line);
	}
	if (line[i] == '+' && line[i + 1] == '=')
	{
		str = ft_substr(line, (i + 2), ft_strlen(line));
		p->env[index] = ft_strjoin_free(p->env[index], str);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:02:44 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/20 14:33:40 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_flag(char *arg)
{
	int		i;
	int		n_flag;

	n_flag = 0;
	i = 0;
	if (arg[i] != '-')
		return (n_flag);
	i++;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		n_flag = 1;
	return (n_flag);
}

static int	is_there_more(t_list_tokens *t)
{
	t_list_tokens	*tmp;

	tmp = t;
	while (tmp->next->type != END)
	{
		if (tmp->type == STRING)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static void	print_args(t_list_tokens *t, int fd)
{
	if (!t->str[0])
		ft_putchar_fd('\0', fd);
	else
	{
		ft_putstr_fd(t->str, fd);
		if (is_there_more(t))
			ft_putchar_fd(' ', fd);
	}
}

int	minishell_echo(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
	int							i;
	int							n_flag;
	t_list_tokens				*tmp;

	(void)p;
	n_flag = 0;
	tmp = e_tokens;
	i = 0;
	while (tmp->type == STRING && tmp->str[i] && is_n_flag(tmp->str))
	{
		n_flag = 1;
		i++;
	}
	if (n_flag)
		tmp = tmp->next;
	while (tmp && (tmp->type != END && tmp->type != PIPE))
	{
		if (tmp->type == STRING)
			print_args(tmp, fd);
		tmp = tmp->next;
	}
	if (!n_flag)
		ft_putchar_fd('\n', fd);
	return (0);
}

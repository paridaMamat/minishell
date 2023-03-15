/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:32:41 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/08 20:09:27 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_ints(int neg, unsigned long long num, int *flag)
{
	if ((neg == 1 && num > LONG_MAX)
		|| (neg == -1 && num > -(unsigned long)LONG_MIN))
		*flag = 1;
	return (*flag);
}

static int	ft_atoi_long(const char *str, int *flag)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		neg *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (check_ints(neg, num, flag))
			break ;
		i++;
	}
	return (num * neg);
}

static int	get_exit_code(char *arg, int *flag)
{
	unsigned long long	i;

	i = 0;
	while (ft_isspace(arg[i]))
		i++;
	if (arg[i] == '\0')
		*flag = 1;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!ft_isdigit(arg[i]))
		*flag = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && !ft_isspace(arg[i]))
			*flag = 1;
		i++;
	}
	i = ft_atoi_long(arg, flag);
	return (i % 256);
}

int	minishell_exit(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
	int		exit_code;
	int		flag;

	(void)fd;
	flag = 0;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (e_tokens->type == END || e_tokens->type == PIPE)
		exit_code = g_exit_code;
	else if (e_tokens->type == STRING)
	{
		exit_code = get_exit_code(e_tokens->str, &flag);
		if (flag)
		{
			exit_code = 2;
			ft_putstr_fd(ERR_EXIT, STDERR_FILENO);
			ft_putstr_fd(e_tokens->str, STDERR_FILENO);
			ft_putendl_fd(ERR_EXIT_MSG1, STDERR_FILENO);
		}
		else if (e_tokens->next->type == STRING)
			return (ft_putendl_fd(ERR_EXIT ERR_EXIT_MSG2, STDERR_FILENO), 1);
	}
	exit_shell(p, exit_code);
	return (2);
}

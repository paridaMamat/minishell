/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:32:41 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/16 12:42:23 by mflores-         ###   ########.fr       */
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
		if (!isdigit(arg[i]) && !ft_isspace(arg[i]))
			*flag = 1;
		i++;
	}
	i = ft_atoi_long(arg, flag);
	return (i % 256);
}

int minishell_exit(t_prompt *data, char **args)
{
	int		exit_code;
	int		flag;
    
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args || !args[1])
		exit_code = g_exit_code;
	else
	{
		exit_code = get_exit_code(args[1], &flag);
		if (flag)
			exit_code = 2;
			ft_putendl_fd("minishell: exit: numeric argument required", \
			STDERR_FILENO);
		if (args[2])
			return (ft_putendl_fd("minishell: exit: too many arguments", \
			STDERR_FILENO), 2);
	}
	exit_shell(&data, exit_code);
	return (2);
}

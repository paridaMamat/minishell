/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 01:56:18 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/27 14:13:48 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_args(int ac)
{
	if (ac == 1)
		return (1);
	ft_putendl_fd(ERR_USAGE, 2);
	return (0);
}

/**
	Checks line.
	On failure or exiting, returns NULL.
	If user presses enter and line is empty, continues reading a new line.
	If not empty, recovers the line and is added to history.
*/
static void	*check_line(t_prompt *p)
{
	if (!p->line)
	{
		printf(MSG_EXIT);
		return (NULL);
	}
	if (ft_strncmp(p->line, "exit", 4) == 0)
	{
		printf(MSG_EXIT);
		free(p->line);
		p->line = NULL;
		return (NULL);
	}
	if (p->line[0] != '\0')
	{
		if (parse_line(p))
		{
			print_structs_debug(&p, 0);
			start_execute(p);
			//g_exit_code = execute cmd table;
		}
			
		else
			g_exit_code = 1;
	}
	free_line(p);
	return (p);
}

static void	start_minishell(t_prompt *p)
{
	while (1)
	{
		setup_signal_handlers();
		p->p = get_prompt(p);
		if (!p->p)
			p->line = readline(DEFAULT_PROMPT);
		else
		{
			p->line = readline(p->p);
			free(p->p);
			p->p = NULL;
		}
		if (!check_line(p))
			break ;
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
	t_prompt		prompt;

	if (!check_args(ac) || !init_prompt(&prompt, env))
		exit_shell(NULL, EXIT_FAILURE);
	(void)av;
	start_minishell(&prompt);
	exit_shell(&prompt, g_exit_code);
}

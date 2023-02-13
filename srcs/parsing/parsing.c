/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:13:25 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/07 18:44:58 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_syntax_error(int status, char *line)
{
	if (status == D_QUOTE)
		ft_putendl_fd(ERR_SYNTAX"\"", 2);
	else if (status == S_QUOTE)
		ft_putendl_fd(ERR_SYNTAX"\'", 2);
	add_history(line);
	return (0);
}

static int	pre_tokenize(t_prompt *p, int line_len)
{
	int	i;
	int	start;
	int	status;

	i = -1;
	start = 0;
	status = STRING;
	while (++i <= line_len)
	{
		status = set_status(status, p->line, i);
		if (status == STRING)
		{
			start = save_word_or_sep(&i, start, p);
			if (start == -1)
				return (0);
		}
	}
	if (status != STRING)
		return (set_syntax_error(status, p->line));
	return (1);
}

static int	has_special_char(char *s)
{
	while (s && *s)
	{
		if (*s == '$' || *s == '\'' || *s == '\"')
			return (1);
		s++;
	}
	return (0);
}

static int	tokenize(t_prompt *p)
{
	t_list_tokens	*curr;

	curr = p->tokens;
	while (curr->next != NULL)
	{
		if (curr->type == STRING && has_special_char(curr->str))
			if (!handle_nodes(curr))
				return (0);
		curr = curr->next;
	}
	return (1);
}

/*
	Function to parse line recovered from readline
*/
int	parse_line(t_prompt *p)
{
	if (!pre_tokenize(p, ft_strlen(p->line)))
		return (2);
	print_structs_debug(&p, 0);
	if (!tokenize(p))
		return (2);
	if (!check_tokens(p))
		return (2);
	add_history(p->line);
	return (0);
}

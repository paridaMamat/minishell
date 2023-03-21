/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:13:25 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/20 20:25:09 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (set_syntax_error(status, NULL));
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
		if (curr->prev && curr->prev->type == HEREDOC && curr->type == STRING)
		{
			curr->type = H_DELIMITER;
			if (ft_strchr(curr->str, '\'') || ft_strchr(curr->str, '\"'))
				curr->type = H_DELIMITER_QUOTES;
			if (!handle_heredoc_delimiter(curr))
				return (0);
		}
		else if (curr->type == STRING && has_special_char(curr->str))
		{
			if (!handle_nodes(p, curr))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}

/*
	Prints the correct error syntax error
*/
int	set_syntax_error(int status, char *err_msg)
{
	if (status == D_QUOTE)
		ft_putendl_fd(ERR_SYNTAX"\"", STDERR_FILENO);
	else if (status == S_QUOTE)
		ft_putendl_fd(ERR_SYNTAX"\'", STDERR_FILENO);
	else
	{
		ft_putstr_fd(ERR_SYNTAX, STDERR_FILENO);
		ft_putstr_fd(err_msg, STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);
	}
	return (0);
}

/*
	Function to parse line recovered from readline
	//print_structs_debug(&p, 0);
*/
int	parse_line(t_prompt *p)
{
	int		err;
	int		ret;
	char	*line;

	err = 0;
	ret = 0;
	line = ft_strdup(p->line);
	p->line = get_dollar(p->env, p->line, STRING, 0);
	if (!p->line)
		return (free(line), 2);
	if (!pre_tokenize(p, ft_strlen(p->line)))
		return (free(line), 2);
	if (!tokenize(p))
		return (free(line), 2);
	add_history(line);
	free(line);
	if (!check_tokens(p))
	{
		err = 1;
		continue_checks(p, err, &ret);
		return (2);
	}
	if (!continue_checks(p, err, &ret))
		return (2);
	return (0);
}

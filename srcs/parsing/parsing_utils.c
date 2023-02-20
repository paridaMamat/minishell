/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 13:27:39 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/17 21:41:20 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_case(int type, char *str, t_list_tokens *next)
{
	if ((type == D_QUOTE || type == S_QUOTE) && (str[0] == '$'
			&& str[1] == '\0') && next)
		return (1);
	else if (type == STRING && (str[0] == '$' && str[1] == '\0') && next
		&& (next->type == D_QUOTE || next->type == S_QUOTE))
		return (2);
	else if (type == STRING && ft_strchr(str, '$') && next
		&& (next->type == D_QUOTE || next->type == S_QUOTE)
		&& str[ft_strlen(str) - 1] == '$')
		return (3);
	else
		return (0);
}

static int	do_this(char **str, char *s, int or_this)
{
	char	*tmp;

	tmp = NULL;
	if (or_this)
	{
		tmp = ft_substr(*str, 0, ft_strlen(*str) - 1);
		if (!tmp)
			return (0);
		free(*str);
		*str = tmp;
	}
	else
	{
		free(*str);
		*str = ft_strdup(s);
		if (!*str)
			return (0);
	}
	return (1);
}

static char	*check_for_dollars_w_qoutes(t_list_tokens *t)
{
	t_list_tokens	*c;

	c = t;
	while (c)
	{
		if (check_case(c->type, c->str, c->next) == 1)
		{
			if (do_this(&c->str, "$", 0) == 0)
				return (NULL);
		}
		else if (check_case(c->type, c->str, c->next) == 2)
		{
			if (do_this(&c->str, "", 0) == 0)
				return (NULL);
		}
		else if (check_case(c->type, c->str, c->next) == 3)
		{
			if (do_this(&c->str, NULL, 1) == 0)
				return (NULL);
		}
		c = c->next;
	}
	return (join_nodes(t));
}

int	handle_heredoc_delimiter(t_list_tokens *t)
{
	t_list_tokens	*tmp;

	tmp = NULL;
	if (!check_string(&tmp, t->str))
		return (0);
	free(t->str);
	t->str = check_for_dollars_w_qoutes(tmp);
	if (!t->str)
	{
		lstclear_token(&tmp);
		return (0);
	}
	return (1);
}

int	check_tokens(t_prompt *p)
{
	t_list_tokens	*tmp;

	tmp = p->tokens;
	while (tmp->next != NULL)
	{
		if ((tmp->prev == NULL && tmp->type == PIPE) || (tmp->type == PIPE
				&& (tmp->next->type == END || tmp->next->type == PIPE)))
			return (set_syntax_error(-1, "|"));
		if ((tmp->type == HEREDOC && (tmp->next->type != H_DELIMITER
					&& tmp->next->type != H_DELIMITER_QUOTES))
			|| (tmp->type == INPUT && tmp->next->type != STRING)
			|| (tmp->type == R_DREDIR && tmp->next->type != STRING)
			|| (tmp->type == R_REDIR && tmp->next->type != STRING))
		{
			if (tmp->next->type == END)
				return (1);
			return (set_syntax_error(-1, tmp->next->str));
		}
		tmp = tmp->next;
	}
	return (1);
}

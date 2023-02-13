/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:49:54 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/02 14:41:09 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_separator(char *line, int i)
{
	if (ft_isspace(line[i]))
		return (SPACES);
	else if (line[i] == '|')
		return (PIPE);
	else if (line[i] == '<' && line[i + 1] == '<')
		return (HEREDOC);
	else if (line[i] == '>' && line[i + 1] == '>')
		return (R_DREDIR);
	else if (line[i] == '<')
		return (INPUT);
	else if (line[i] == '>')
		return (R_REDIR);
	else if (line[i] == '\0')
		return (END);
	else
		return (0);
}

static int	save_word(t_prompt *p, int index, int start)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(sizeof(char) * (index - start + 1));
	if (!word)
		return (0);
	while (start < index)
	{
		word[i] = p->line[start];
		start++;
		i++;
	}
	word[i] = '\0';
	if (!lstadd_back_token(&p->tokens, add_new_token(word, STRING)))
		return (0);
	return (1);
}

static int	save_separator(t_prompt *p, int index, int type)
{
	int		i;
	int		len;
	char	*sep;

	i = 0;
	if (type == R_DREDIR || type == HEREDOC)
		len = 3;
	else
		len = 2;
	sep = malloc(sizeof(char) * len);
	if (!sep)
		return (0);
	while (i < (len - 1))
		sep[i++] = p->line[index++];
	sep[i] = '\0';
	if (!lstadd_back_token(&p->tokens, add_new_token(sep, type)))
		return (0);
	return (1);
}

int	set_status(int status, char *line, int i)
{
	if (line[i] == '\'' && status == STRING)
		status = S_QUOTE;
	else if (line[i] == '\"' && status == STRING)
		status = D_QUOTE;
	else if (line[i] == '\'' && status == S_QUOTE)
		status = STRING;
	else if (line[i] == '\"' && status == D_QUOTE)
		status = STRING;
	return (status);
}

int	save_word_or_sep(int *i, int start, t_prompt *p)
{
	int	type;

	type = is_separator(p->line, (*i));
	if (type)
	{
		if ((*i) != 0 && is_separator(p->line, (*i) - 1) == 0)
			if (!save_word(p, (*i), start))
				return (-1);
		if (type == R_DREDIR || type == HEREDOC || type == PIPE
			|| type == INPUT || type == R_REDIR || type == END)
		{
			if (!save_separator(p, (*i), type))
				return (-1);
			if (type == R_DREDIR || type == HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:49:54 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/27 18:33:48 by mflores-         ###   ########.fr       */
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
		return (1);
	while (start < index)
	{
		word[i] = p->line[start];
		start++;
		i++;
	}
	word[i] = '\0';
	lstadd_back_token(&p->tokens, add_new_token(word, STRING));
	return (0);
}

static int	set_status(int status, char *line, int i)
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
		return (1);
	while (i < (len - 1))
		sep[i++] = p->line[index++];
	sep[i] = '\0';
	lstadd_back_token(&p->tokens, add_new_token(sep, type));
	return (0);
}

static int	save_word_or_sep(int *i, int start, t_prompt *p)
{
	int	type;

	type = is_separator(p->line, (*i));
	if (type)
	{
		if ((*i) != 0 && is_separator(p->line, (*i) - 1) == 0)
			save_word(p, (*i), start);
		if (type == R_DREDIR || type == HEREDOC || type == PIPE
			|| type == INPUT || type == R_REDIR || type == END)
		{
			save_separator(p, (*i), type);
			if (type == R_DREDIR || type == HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}

static int	pre_tokenize(t_prompt *p)
{
	int	i;
	int	end;
	int	start;
	int	status;

	i = -1;
	start = 0;
	end = ft_strlen(p->line);
	status = STRING;
	while (++i <= end)
	{
		status = set_status(status, p->line, i);
		if (status == STRING)
			start = save_word_or_sep(&i, start, p);
	}
	if (status != STRING)
	{
		if (status == D_QUOTE)
			ft_putendl_fd("Syntax error: while looking for matching \"", 2);
		else if (status == S_QUOTE)
			ft_putendl_fd("Syntax error: while looking for matching \'", 2);
		add_history(p->line);
		return (0);
	}
	return (1);
}

// Function to parse line from user input
int	parse_line(t_prompt *p)
{
	if (!pre_tokenize(p))
		return (0);
	//if (!tokenize(p))
	//	return (0);
	//if (!handle_quotes(p));
	//	return (0);
	// handle quotes "" '' "''" '""'
	//as soon as we get a list of tokens we need to check if we find any
	//variables we need to expand ex. $USER 
	//create list of commands to send to execution ex, echo, ls etc
	add_history(p->line);
	return (1);
}
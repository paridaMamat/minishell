/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:24:01 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/20 13:51:08 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	keep_string(int *i, char *str, t_list_tokens **sub_tok)
{
	int	start;

	start = (*i);
	while (str[(*i)] && !(str[(*i)] == '\'' || str[(*i)] == '\"'))
		(*i)++;
	str = ft_substr(str, start, ((*i) - start));
	if (!str)
		return (0);
	if (!lstadd_back_token(sub_tok, add_new_token(str, STRING)))
		return (0);
	return (1);
}

static int	delete_quotes(int *i, char *str, t_list_tokens **sub_tok, char c)
{
	int	start;
	int	type;

	if (c == '\'')
		type = S_QUOTE;
	else if (c == '\"')
		type = D_QUOTE;
	start = ++(*i);
	while (str[(*i)] && str[(*i)] != c)
		(*i)++;
	str = ft_substr(str, start, ((*i) - start));
	if (!str)
		return (-1);
	if (!lstadd_back_token(sub_tok, add_new_token(str, type)))
		return (-1);
	(*i)++;
	return (1);
}

int	check_string(t_list_tokens **sub_tokens, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] && !(str[i] == '\'' || str[i] == '\"'))
		{
			if (keep_string(&i, str, sub_tokens) == -1)
				return (0);
		}
		else if (str[i] && str[i] == '\'')
		{
			if (delete_quotes(&i, str, sub_tokens, '\'') == -1)
				return (0);
		}
		else if (str[i] && str[i] == '\"')
		{
			if (delete_quotes(&i, str, sub_tokens, '\"') == -1)
				return (0);
		}
	}	
	return (1);
}

static char	*check_for_dollars(char **env, t_list_tokens *s_tokens)
{
	t_list_tokens	*curr;

	curr = s_tokens;
	while (curr)
	{
		if ((curr->type == D_QUOTE || curr->type == STRING)
			&& (curr->str[0] == '$' && curr->str[1] == '\0') && curr->next)
		{
			free(curr->str);
			curr->str = ft_strdup("");
			if (!curr->str)
				return (NULL);
		}
		else if ((curr->type == D_QUOTE || curr->type == STRING)
			&& ft_strchr(curr->str, '$'))
		{
			curr->str = get_dollar(env, curr->str, curr->type, 0);
			if (!curr->str)
				return (NULL);
		}
		curr = curr->next;
	}
	return (join_nodes(s_tokens));
}

/*
	We removed in each node the quotes depending where they are located
	ex. "'toto'" => 'toto'
	ex. 'lolo' => lolo
	ex. '"'$USER'"' = > "mflores-"
	If we find $ inside "" or in a normal string we go see if the variable exists
	in the env and then we expand it otherwise we send back nothing
	ex. $USERPOPO lolo => lolo ($USERPOPO was not found in env)
	ex. original token: "lolo"'$USERtot'"$USER,popo"
	we send back the modified token as: lolo$USERtotmflores-,popo
*/
int	handle_nodes(t_prompt *p, t_list_tokens *n)
{
	t_list_tokens	*sub_tokens;

	sub_tokens = NULL;
	if (!check_string(&sub_tokens, n->str))
		return (0);
	free(n->str);
	n->str = check_for_dollars(p->env, sub_tokens);
	if (!n->str)
	{
		lstclear_token(&sub_tokens);
		return (0);
	}
	n->type = STRING;
	return (1);
}

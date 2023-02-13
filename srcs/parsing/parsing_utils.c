/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 13:27:39 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/08 11:00:55 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_error(char *err_msg, char *line, t_list_tokens **tokens)
{
	ft_putendl_fd(err_msg, 2);
	add_history(line);
	lstclear_token(tokens);
	return (0);
}

int	check_tokens(t_prompt *p)
{
	t_list_tokens	*tmp;

	tmp = p->tokens;
	while (tmp->next != NULL)
	{
		if ((tmp->prev == NULL && tmp->type == PIPE) || (tmp->type == PIPE
				&& (tmp->next->type == END || tmp->next->type == PIPE)))
			return (set_error(ERR_SYNTAX"`|'", p->line, &p->tokens));
		if ((tmp->type == HEREDOC && tmp->next->type != STRING)
			|| (tmp->type == INPUT && tmp->next->type != STRING)
			|| (tmp->type == R_DREDIR && tmp->next->type != STRING)
			|| (tmp->type == R_REDIR && tmp->next->type != STRING))
			return (set_error(ERR_SYNTAX"`newline'", p->line, &p->tokens));
		if (tmp->type == STRING && tmp->prev && (tmp->prev->type == INPUT
				|| tmp->prev->type == R_DREDIR || tmp->prev->type == R_REDIR))
			tmp->type = IS_FILE;
		if (tmp->type == STRING && tmp->prev && tmp->prev->type == HEREDOC)
			tmp->type = H_DELIMITER;
		tmp = tmp->next;
	}
	return (1);
}

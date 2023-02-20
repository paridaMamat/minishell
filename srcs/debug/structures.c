/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 13:51:45 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/17 21:37:49 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	if_separator(char **str, int type)
{
	if (type == R_REDIR)
		*str = "R_REDIR";
	else if (type == R_DREDIR)
		*str = "R_DREDIR";
	else if (type == PIPE)
		*str = "PIPE";
	else if (type == HEREDOC)
		*str = "HEREDOC";
	else if (type == INPUT)
		*str = "INPUT";
	else if (type == SPACES)
		*str = "SPACES";
	else if (type == END)
		*str = "END";
}

static char	*show_type(int type)
{
	char	*str;

	str = NULL;
	if_separator(&str, type);
	if (type == STRING)
		str = "STRING";
	else if (type == D_QUOTE)
		str = "D_QUOTE";
	else if (type == S_QUOTE)
		str = "S_QUOTE";
	else if (type == BUILTINS)
		str = "BUILTINS";
	else if (type == IS_DIR)
		str = "IS_DIR";
	else if (type == NOT_DIR)
		str = "NOT_DIR";
	else if (type == IS_FILE)
		str = "IS_FILE";
	else if (type == H_DELIMITER)
		str = "H_DELIMITER";
	else if (type == H_DELIMITER_QUOTES)
		str = "H_DELIMITER_QUOTES";
	return (str);
}

void	print_list(t_list_tokens *list, char *name_struct, char *color)
{
	t_list_tokens	*tmp;

	printf("\n%s<-----------Structure %s: --------->\n", color, name_struct);
	tmp = list;
	while (tmp != NULL)
	{
		printf("\nstr: %s\n", tmp->str);
		printf("type: %s\n\n", show_type(tmp->type));
		tmp = tmp->next;
	}
	printf("\n<-------------------------------------------->%s\n", DEFAULT);
	printf("\n\n");
}

void	print_structs_debug(t_prompt **p, int with_env)
{
	int				i;

	if (p)
	{
		i = 0;
		printf("%s<******* GENERAL STRUCTURE (PROMPT): *******>\n\n", BLUE);
		if ((*p)->line)
			printf("Line from readline: %s\n", (*p)->line);
		if ((*p)->pwd)
			printf("Pwd: %s\n", (*p)->pwd);
		if ((*p)->user)
			printf("User: %s\n", (*p)->user);
		if (with_env && (*p)->env)
		{
			printf("ENV:\n");
			while ((*p)->env[i])
				printf("%s\n", (*p)->env[i++]);
		}
		if ((*p)->tokens)
			print_list((*p)->tokens, "TOKENS", BLUE);
		printf("%s<**************************************>%s\n", BLUE, DEFAULT);
	}
}

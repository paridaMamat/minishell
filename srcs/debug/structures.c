/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 13:51:45 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/27 14:13:21 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *show_type(int type)
{
    char *str;
    
    str = NULL;
    if (type == STRING)
        str = "STRING";
    else if (type == R_REDIR)
        str = "R_REDIR";
    else if (type == R_DREDIR)
        str = "R_DREDIR";
    else if (type == PIPE)
        str = "PIPE";
    else if (type == HEREDOC)
        str = "HEREDOC";
    else if (type == INPUT)
        str = "INPUT";
    else if (type == BUILTINS)
        str = "BUILTINS";
    else if (type == D_QUOTE)
        str = "D_QUOTE";
    else if (type == S_QUOTE)
        str = "S_QUOTE";
    else if (type == IS_DIR)
        str = "IS_DIR";
    else if (type == NOT_DIR)
        str = "NOT_DIR";
    else if (type == SPACES)
        str = "SPACES";
    else if (type == END)
        str = "END";
    return (str);
}


void print_structs_debug(t_prompt **p, int with_env)
{
	int i;
	t_list_tokens   *curr;
	
	if (p)
	{
		i = 0;
		if ((*p)->line)
			printf("Line: %s\n", (*p)->line);
		if ((*p)->pwd)
			printf("Pwd: %s\n", (*p)->pwd);
		if ((*p)->user)
			printf("User: %s\n", (*p)->user);
        if (with_env)
        {
            if ((*p)->env)
            {
                printf("Env:\n");
                while ((*p)->env[i])
                    printf("%s\n", (*p)->env[i++]);
            } 
        }
		if ((*p)->tokens)
		{
			curr = (*p)->tokens;
			printf("\nTokens:\n");
			while (curr)
			{
				printf("str: %s\ntype: %s\n\n", curr->str, \
					show_type(curr->type));
				curr = curr->next;
			}
		}
	}
}
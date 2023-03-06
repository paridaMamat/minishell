/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 09:21:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/06 15:01:30 by parida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int    execute_export(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
    int	i;
	char	**tmp;
	
	if (e_tokens->type == END && e_tokens->type == PIPE)
        print_export(p, fd);
    else
        export_arg(p, e_tokens, fd);
        
}
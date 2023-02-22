/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 09:21:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/02/21 15:03:14 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int    execute_export(t_prompt *p, t_list_tokens *e_tokens)
{
    t_list_tokens   *tmp;
    
    tmp = e_tokens;
    tmp = tmp->next;
    if (tmp)
}
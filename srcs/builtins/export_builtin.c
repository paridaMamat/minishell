/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 09:21:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/08 11:51:00 by parida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int    minishell_export(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
    while (e_tokens->type != STRING && e_tokens->type != END && e_tokens->type != PIPE)
		e_tokens = e_tokens->next;
	if (e_tokens->type == END || e_tokens->type == PIPE)
        print_export(p, fd);
    else
        export_arg(p, e_tokens, fd);
    return (0);
}

int print_export(t_prompt *p, int fd)
{
    int i;
    int j;
    char    **env;

    i = 0;
    env = p->env;
    while (env[i])
    {
        j = 0;
        ft_putstr_fd("declare -x ", fd);
        while (env[i][j])
        {
            ft_putchar_fd(env[i][j], fd);
            if (env[i][j] == '=')
                ft_putchar_fd('"', fd); 
            j++;
        }
        ft_putstr_fd("\"\n", fd);
        i++;
    }
    return (0);
}

int export_arg(t_prompt *p, t_list_tokens *e_tokens, int fd)
{
    char    *str;
    char    *arg;
    int i;
    
    str = NULL;
	while (e_tokens->type != END && e_tokens->type != PIPE)
	{
		if (e_tokens->type == STRING)
		{
            i = 0;
            arg = e_tokens->str;
            while (arg[i] && (ft_isalpha(arg[i]) || ft_isalnum(arg[i]) || arg[i] == '_'))
		        i++;
            if (arg[i] == '=' || (arg[i] == '+' && arg[i + 1] == '='))
                str = ft_substr(arg, 0, i);
            else //if (arg[0] == '=')
            {
                ft_putstr_fd("bash: export :'", fd);
                ft_putstr_fd(arg, fd);
                ft_putstr_fd("' not a valide identifier", fd);
            } 
            printf("str = %s\n", str);   
            if (check_in_env(p, str) == -1)
                ft_extend_matrix(p->env, arg);
            else if (check_in_env(p, str) > -1)
                add_or_replace_env(p, arg, str);
            i++;
		}
		e_tokens = e_tokens->next;
	}
	return (0);
}

int add_or_replace_env(t_prompt *p, char *line, char *str)
{
    int i;
    int index;

    i = 0;
    index = check_in_env(p, str);
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
    if (line[i] == '=')
        p->env[index] = line;
    if (line[i] == '+' && line[i + 1] == '=')
    {
        str = ft_substr(line, (i+2), ft_strlen(line));
        p->env[i] = ft_strjoin(p->env[i], str);
    }
    return (0);         
}
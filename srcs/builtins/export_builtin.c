/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 09:21:00 by pmaimait          #+#    #+#             */
/*   Updated: 2023/03/09 11:48:25 by parida           ###   ########.fr       */
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
                ft_putendl_fd("' not a valide identifier", fd);
                return (1);
            }   
            if (check_in_env(p, str) == -1)
                p->env = ft_extend_matrix(p->env, arg);
            else if (check_in_env(p, str) > -1)
            {
                add_or_replace_env(p, arg, str);
            }
            free(str);
            i++;
		}
		e_tokens = e_tokens->next;
	}
	return (0);
}

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new_str;
	size_t	len_s1_s2;
	size_t	i;
	size_t	x;

	if (!s1 || !s2)
		return (NULL);
	len_s1_s2 = ft_strlen(s1) + ft_strlen(s2) + 1;
	new_str = malloc(sizeof(char) * len_s1_s2);
	if (!new_str)
		return (NULL);
	i = 0;
	x = 0;
	while (s1[i])
		new_str[x++] = s1[i++];
	i = 0;
	while (s2[i])
		new_str[x++] = s2[i++];
	new_str[x] = 0;
    free(s1);
    free(s2);
	return (new_str);
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
        p->env[index] = ft_strdup(line);
    if (line[i] == '+' && line[i + 1] == '=')
    {
        str = ft_substr(line, (i+2), ft_strlen(line));
        p->env[index] = ft_strjoin_free(p->env[index], str);
    }
    return (0);         
}
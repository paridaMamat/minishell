/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 12:11:10 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/16 13:39:31 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_name(int h_nb, int	*fd)
{
	char		*name;
	char		*number;

	number = ft_itoa(h_nb);
	if (!number)
		return (NULL);
	name = ft_strjoin(HEREDOC_NAME, number);
	free(number);
	if (!name)
		return (NULL);
	*fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*fd == -1)
	{
		free_ptr(name);
		return (NULL);
	}
	return (name);
}

static int	evaluate_line(t_list_tokens *delimiter, char **line, int *ret)
{
	if (*line == NULL)
	{
		ft_putstr_fd(ERR_HEREDOC, STDERR_FILENO);
		ft_putstr_fd(delimiter->str, STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
		*ret = 1;
		return (0);
	}
	if (ft_strncmp(*line, delimiter->str, (ft_strlen(delimiter->str) + 1)) == 0)
	{
		*ret = 1;
		return (0);
	}
	if (delimiter->type == H_DELIMITER && ft_strchr(*line, '$'))
	{
		*line = get_dollar(*line, delimiter->type, 1);
		if (!*line)
		{
			free_ptr(*line);
			*ret = 0;
			return (0);
		}
	}
	return (1);
}

static void	update_node(t_list_tokens *heredoc, t_list_tokens *delimiter, \
char *infile, int *fd)
{
	dup2(*fd, STDIN_FILENO);
	close(*fd);
	free(heredoc->str);
	heredoc->str = infile;
	heredoc->next = delimiter->next;
	delimiter->next->prev = heredoc;
	free(delimiter->str);
	free_ptr(delimiter);
}

static int	fill_heredoc(t_list_tokens *heredoc, t_list_tokens *delimiter, \
int h_nb, int *ret)
{
	char	*line;
	char	*infile;
	int		tmp_fd;
	int		old_stdin;

	old_stdin = 0;
	infile = get_heredoc_name(h_nb, &tmp_fd);
	setup_signal_heredoc();
	old_stdin = dup(STDIN_FILENO);
	if (!infile)
		return (0);
	g_exit_code = 0;
	line = readline(PROMPT_S2);
	while (g_exit_code != 130 && evaluate_line(delimiter, &line, ret))
	{
		ft_putendl_fd(line, tmp_fd);
		free_ptr(line);
		line = readline(PROMPT_S2);
	}
	update_node(heredoc, delimiter, infile, &old_stdin);
	free_ptr(line);
	close(tmp_fd);
	if (g_exit_code == 130)
	{
		printf("g_exit_code: %d\n", g_exit_code);
		return (0);
	}
	return (*ret);
}

int	continue_checks(t_prompt *p, int flag, int *ret)
{
	t_list_tokens	*curr;
	int				heredoc_nb;

	curr = p->tokens;
	heredoc_nb = 0;
	while (curr->next != NULL)
	{
		if ((curr->next->type == END && (curr->type == HEREDOC
					|| curr->type == INPUT || curr->type == R_DREDIR
					|| curr->type == R_REDIR)) && flag == 0)
			return (set_syntax_error(-1, "newline"));
		if (curr->type == STRING && curr->prev && (curr->prev->type == INPUT
				|| curr->prev->type == R_DREDIR || curr->prev->type == R_REDIR))
			curr->type = IS_FILE;
		if (curr->type == HEREDOC && (curr->next->type == H_DELIMITER_QUOTES
				|| curr->next->type == H_DELIMITER))
		{
			if (!fill_heredoc(curr, curr->next, heredoc_nb, ret))
				return (0);
			heredoc_nb++;
		}
		curr = curr->next;
	}
	return (1);
}

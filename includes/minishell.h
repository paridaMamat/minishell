/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:38:40 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/06 15:11:44 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/******************************************************************************/
/*                                                                            */
/*                                LIBS                                        */
/*                                                                            */
/******************************************************************************/

# include <libft.h>
# include <ft_printf.h>
# include <gnl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <errno.h>

/******************************************************************************/
/*                                                                            */
/*                                MACROS                                      */
/*                                                                            */
/******************************************************************************/

/* OTHER MESSAGES */
# define READ_END 0
# define WRITE_END 1
# define MSG_EXIT "exit\n"

/* ERROR MESSAGES */
# define ERR_USAGE "Usage: ./minishell"
# define ERR_MSG "Error: "
# define ERR_MALLOC "Could not allocate memory.\n"
# define ERR_GETCWD "Getcwd: "

/* Colors */
# define DEFAULT "\001\e[00;39m\002"
# define GREY "\001\e[1;90m\002"
# define RED "\001\e[1;91m\002"
# define GREEN "\001\e[0;92m\002"
# define YELLOW "\001\e[1;93m\002"
# define BLUE "\001\e[1;94m\002"
# define MAGENTA "\001\e[1;95m\002"
# define PINK "\001\e[0;35m\002"
# define PURPLE "\001\e[0;34m\002"
# define CYAN "\001\e[1;96m\002"
# define WHITE "\001\e[0;97m\002"

/* Prompt */
# define PROMPT_HOSTNAME "\001\e[0;36m\002\001\e[1m\002@minishell:\001\e[0m\002"
# define DEFAULT_PROMPT "\001\e[0;36m\002\001\e[1m\002>minishell$\001\e[0m\002 "
# define PROMPT_S2 "> "

/******************************************************************************/
/*                                                                            */
/*                         	GLOBAL VARIABLES                                  */
/*                                                                            */
/******************************************************************************/

extern int	g_exit_code;

/******************************************************************************/
/*                                                                            */
/*                              STRUCTURES                                    */
/*                                                                            */
/******************************************************************************/

typedef enum e_tokens	t_tokens;
typedef struct s_prompt	t_prompt;
typedef struct s_list_tokens	t_list_tokens;
typedef struct s_fds  t_fds;
typedef struct s_pipex  t_pipex;


struct s_pipex
{
	pid_t			pid2[50];
	int				end[2];
	char			**path;
	char			*cmd;
	char			**cmd_arg;
};

struct s_fds
{
	char			*infile;
	char			*outfile;
	int				fd_in;
	int				fd_out;
	int				stdin_backup;
	int				stdout_backup;
};

struct s_prompt
{
	char			*line;
	char			*pwd;
	char			*user;
	char			*p;
	char			**env;
	t_list_tokens	*tokens;
	int				(*pipes)[2];
	t_fds			*fds;
	t_pipex			*pipex;
};

struct s_list_tokens
{
	char			*str;
	int				type;
	int				index;
	t_list_tokens	*prev;
	t_list_tokens	*next;
};

enum e_tokens
{
	STRING,
	R_REDIR, // trunc
	R_DREDIR, //append
	PIPE,
	HEREDOC,
	INPUT,
	BUILTINS,
	D_QUOTE,
	S_QUOTE,
	IS_DIR,
	NOT_DIR,
	SPACES,
	END
};

/******************************************************************************/
/*                                                                            */
/*                               PROTOTYPES                                   */
/*                                                                            */
/******************************************************************************/

/*-------------------------------- ENV ---------------------------------------*/

/*------------------------------ END ENV -------------------------------------*/

/*------------------------------ PARSING -------------------------------------*/

int				parse_line(t_prompt *p);
int				tokenize(t_prompt *p);
//int				handle_quotes(t_prompt *p);
t_list_tokens	*add_new_token(char *str, int type);
void			lstadd_back_token(t_list_tokens **lst, t_list_tokens *new_node);
void			lstclear_token(t_list_tokens **lst);
void 			print_structs_debug(t_prompt **p, int with_env);

/*----------------------------- END PARSING ----------------------------------*/

/*------------------------------- PROMPT -------------------------------------*/
/**
	Initializes the prompt structure.
*/
int		init_prompt(t_prompt *p, char **env);

/**
	Gets the current prompt: recovers the current user and current working 
	directory. On failure, returns NULL.
*/
char	*get_prompt(t_prompt *p);

/*----------------------------- END PROMPT -----------------------------------*/

/*------------------------------ SIGNALS -------------------------------------*/
/**
	Set up the signal handlers
*/
void	setup_signal_handlers(void);

/*----------------------------- END SIGNALS ----------------------------------*/

/*------------------------------- UTILS --------------------------------------*/
/**
	Frees a matrix properly.
*/
void	ft_free_matrix(char **m);

/**
	Modifies current matrix: returns a new matrix with the new string.
*/
char	**ft_extend_matrix(char **in, char *newstr);

/**
	Allocates new matrix.
*/
char	**ft_dup_matrix(char **m);

void	free_line(t_prompt *p);

/**
	Frees and exits minishell program.
*/
void	exit_shell(t_prompt *data, int nb);

/**
	Frees all structures.
*/
void	free_all(t_prompt *p);

/*----------------------------- END UTILS ------------------------------------*/

/*----------------------------- PRE_EXECUTION ------------------------------------*/
/**
   start execute the command 
 */
int		start_execute(t_prompt *p);
void	open_redirection(t_prompt *p, t_list_tokens *e_tokens);
int     count_pipe(t_prompt *p, t_list_tokens *e_tokens);


/*----------------------------- END PRE_EXECUTION --------------------------------*/
#endif

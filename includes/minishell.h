/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:38:40 by mflores-          #+#    #+#             */
/*   Updated: 2023/02/14 14:47:20 by pmaimait         ###   ########.fr       */
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
# define ERR_SYNTAX "minishell syntax error: near unexpected token "
# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126

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

typedef struct s_pipex
{
	pid_t			pid2[50];
	int				(*fd)[2];
	char			**path;
	char			*cmd;
	char			**cmd_arg;
}	t_pipex;

typedef struct s_fds
{
	int				infile;
	int				outfile;
}	t_fds;
	// int				fd_in;
	// int				fd_out;
	// int				stdin_backup;
	// int				stdout_backup;

typedef struct s_prompt
{
	char					*line;
	char					*pwd;
	char					*user;
	char					*p;
	char					**env;
	struct s_list_tokens	*tokens;
	struct s_fds			*fds;
	struct s_pipex			*pipex;
}	t_prompt;


typedef struct s_list_tokens
{
	char					*str;
	int						type;
	int						index;
	int						nbr_pipe;
	struct s_list_tokens	*prev;
	struct s_list_tokens	*next;
}	t_list_tokens;

typedef enum e_tokens
{
	STRING,
	R_REDIR,
	R_DREDIR,
	PIPE,
	HEREDOC,
	H_DELIMITER,
	INPUT,
	BUILTINS,
	D_QUOTE,
	S_QUOTE,
	IS_DIR,
	NOT_DIR,
	SPACES,
	IS_FILE,
	END
}	t_tokens;

/******************************************************************************/
/*                                                                            */
/*                               PROTOTYPES                                   */
/*                                                                            */
/******************************************************************************/

/*-------------------------------- ENV ---------------------------------------*/

/*------------------------------ END ENV -------------------------------------*/

/*------------------------------ PARSING -------------------------------------*/

int				parse_line(t_prompt *p);
int				set_status(int status, char *line, int i);
int				save_word_or_sep(int *i, int start, t_prompt *p);
int				handle_nodes(t_list_tokens *n);
char			*get_dollar(char *str, int type);
int				isolate_var(t_list_tokens **ptr, char *str, int type);
int				check_tokens(t_prompt *p);
t_list_tokens	*add_new_token(char *str, int type);
int				lstadd_back_token(t_list_tokens **lst, t_list_tokens *new_node);
void			lstclear_token(t_list_tokens **lst);
char			*join_nodes(t_list_tokens *lst);

/*----------------------------- END PARSING ----------------------------------*/

/*------------------------------- PROMPT -------------------------------------*/
/**
	Initializes the prompt structure.
*/
int				init_prompt(t_prompt *p, char **env);

/**
	Gets the current prompt: recovers the current user and current working 
	directory. On failure, returns NULL.
*/
char			*get_prompt(t_prompt *p);

/*----------------------------- END PROMPT -----------------------------------*/

/*------------------------------ SIGNALS -------------------------------------*/
/**
	Set up the signal handlers
*/
void			setup_signal_handlers(void);

/*----------------------------- END SIGNALS ----------------------------------*/

/*------------------------------- UTILS --------------------------------------*/
/**
	Frees a matrix properly.
*/
void			ft_free_matrix(char **m);
void			ft_free_fd(int	(*fd)[2]);

/**
	Modifies current matrix: returns a new matrix with the new string.
*/
char			**ft_extend_matrix(char **in, char *newstr);

/**
	Allocates new matrix.
*/
char			**ft_dup_matrix(char **m);

void			free_ptr(void *thing);

void			free_line(t_prompt *p);

/**
	Frees and exits minishell program.
*/
void			exit_shell(t_prompt *data, int nb);

/**
	Frees all structures.
*/
void			free_all(t_prompt *p);

/*----------------------------- END UTILS ------------------------------------*/

/*-------------------------------- DEBUG -------------------------------------*/

void			print_list(t_list_tokens *list, char *name_struct, char *color);
void			print_structs_debug(t_prompt **p, int with_env);

/*------------------------------- END DEBUG ----------------------------------*/

/*----------------------------- PRE_EXECUTION --------------------------------*/
/**
   Prepare for execution
 */
int				init_data(t_prompt *p);
int				start_execute(t_prompt *p);
void			open_file(t_prompt *p, t_list_tokens *e_tokens);
int				count_pipe(t_list_tokens *e_tokens);

/*---------------------------- END PRE_EXECUTION -----------------------------*/

/*---------------------------- EXECUTION_SYS_BIN -----------------------------*/
/**
   execute the system's command 
 */
int				execute_sys(t_prompt *p, t_list_tokens *e_tokens);
char			**get_path(t_pipex *pipex, char **envp);
char			*get_cmd(char **path, char *cmd);
char			**create_cmd_arg(t_list_tokens *e_tokens);

/*---------------------------- END EXECUTION_SYS_BIN -------------------------*/

/*----------------------------- EXECUTION ------------------------------------*/
/**
   execute 
 */
int				child_process(t_prompt *p, t_list_tokens *e_tokens);
int				parent_process(t_prompt *p);
int				multiple_pipe(t_prompt *p, t_list_tokens *e_tokens);
int				execute(t_prompt *p, t_list_tokens *e_tokens);
int				one_command(t_prompt *p, t_list_tokens *e_tokens);

/*----------------------------- END EXECUTION --------------------------------*/
#endif

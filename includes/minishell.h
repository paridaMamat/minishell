/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:38:40 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/17 16:09:29 by pmaimait         ###   ########.fr       */
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
# include <sys/types.h>
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
# define MSG_EXIT "exit"
//# define HEREDOC_NAME "/tmp/.minishell_heredoc_"
# define HEREDOC_NAME ".minishell_heredoc_"

/* ERROR MESSAGES */
# define HELL_NO "No way Jose, but nice try!"
# define ERR_USAGE "Usage: ./minishell"
# define ERR_MSG "Error: "
# define ERR_MALLOC "Could not allocate memory.\n"
# define ERR_GETCWD "Getcwd: "
# define ERR_SYNTAX "minishell: syntax error: near unexpected token `"
# define ERR_HEREDOC "minishell: warning: here-document delimited by \
end-of-file: (wanted `"
# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126
# define ERR_NOT_ASKED "Not asked in the mandatory part of minishell"
# define ERR_EXIT "minishell: exit: "
# define ERR_EXIT_MSG1 ": numeric argument required"
# define ERR_EXIT_MSG2 "too many arguments"
# define ERR_CD "minishell: cd: too many arguments"
# define ERR_CD_MSG1 "minishell: cd: HOME not set"
# define ERR_CD_MSG2 "minishell: cd: OLDPWD not set"

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
	pid_t			pid;
	int				**fd;
	char			**path;
	char			*cmd;
	char			**cmd_arg;
}	t_pipex;

typedef struct s_prompt
{
	char					*line;
	char					*pwd;
	char					*user;
	char					*p;
	char					**env;
	struct s_list_tokens	*tokens;
	int						nbr_pipe;
	int						infile;
	int						outfile;
	struct s_pipex			*pipex;
}	t_prompt;

typedef struct s_list_tokens
{
	char					*str;
	int						type;
	int						index;
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
	H_DELIMITER_QUOTES,
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

int				set_syntax_error(int status, char *err_msg);
int				parse_line(t_prompt *p);
int				set_status(int status, char *line, int i);
int				save_word_or_sep(int *i, int start, t_prompt *p);
int				check_string(t_list_tokens **sub_tokens, char *str);
int				handle_nodes(t_list_tokens *n);
char			*get_dollar(char *str, int type, int heredoc);
int				isolate_var(t_list_tokens **ptr, char *str, int type);
int				handle_heredoc_delimiter(t_list_tokens *t);
int				check_tokens(t_prompt *p);
int				continue_checks(t_prompt *p, int flag, int *ret);
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
void			setup_signal_heredoc(void);

/*----------------------------- END SIGNALS ----------------------------------*/

/*------------------------------- UTILS --------------------------------------*/
/**
	Frees a matrix properly.
*/
void			ft_free_matrix(char **m);
void			ft_free_fd(t_prompt *p);

/**
	Modifies current matrix: returns a new matrix with the new string.
*/
char			**ft_extend_matrix(char **in, char *newstr);

/**
	Allocates new matrix.
*/
char			**ft_dup_matrix(char **m);

/**
	Frees pointer and sets it to NULL.
*/
void			free_ptr(void *thing);

/**
	Frees the prompt, the list of tokens and execution structure.
	Returns a new prompt and waits for new input.
*/
void			free_line(t_prompt *p);

/**
	Frees and exits minishell program.
*/
void			exit_shell(t_prompt *data, int nb);

/**
	Frees all structures.
*/
void			free_all(t_prompt *p);

/**
	close all pipe
*/
void			close_pipe(t_prompt *p);
int				close_free_pipe(t_prompt *p);

/**
	is this a function of builtings
*/
int				is_built(char *str);

/**
	certain function for builtins
*/
char			*ft_strjoin_free(char *s1, char *s2);
int				print_export(t_prompt *p, int fd);
/*----------------------------- END UTILS ------------------------------------*/

/*------------------------------- DEBUG --------------------------------------*/

/**
	FOR DEBUG PURPOSES ONLY
	Prints the structure, with a specific name and specific color passed 
	as parameters.
*/
void			print_list(t_list_tokens *list, char *name_struct, char *color);

/**
	FOR DEBUG PURPOSES ONLY
	Prints the main structure of minishell, with or without the environment.
*/
void			print_structs_debug(t_prompt **p, int with_env);

/*----------------------------- END DEBUG ------------------------------------*/

/*------------------------------ BUILTINS ------------------------------------*/

int				minishell_echo(t_prompt *p, t_list_tokens *e_tokens, int fd);
int				minishell_pwd(t_prompt *p, t_list_tokens *e_tokens, int fd);
int				minishell_env(t_prompt *p, t_list_tokens *e_tokens, int fd);
int				minishell_cd(t_prompt *p, t_list_tokens *e_tokens, int fd);
int				minishell_unset(t_prompt *p, t_list_tokens *e_tokens);
int				minishell_exit(t_prompt *p, t_list_tokens *e_tokens, int fd);
int				minishell_export(t_prompt *p, t_list_tokens *e_tokens, int fd);

/*---------------------------- END BUILTINS ----------------------------------*/

/*----------------------------- PRE_EXECUTION --------------------------------*/
/**
   Prepare for execution
 */
int				open_pipe(t_prompt *p);
int				init_data(t_prompt *p);
int				start_execute(t_prompt *p);
int			open_file(t_prompt *p, t_list_tokens *e_tokens);
int				count_pipe(t_prompt *p);

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
int				child_process(t_prompt *p, t_list_tokens *e_tokens, int *ib);
int				execute_cmd(t_prompt *p, t_list_tokens *e_tokens, int *ib);
int				execute(t_prompt *p, t_list_tokens *e_tokens);
int				one_command(t_prompt *p, t_list_tokens *e_tokens, int *ib);

/*----------------------------- END EXECUTION -------------------------------*/

/*----------------------------- EXECUTE_ONE_CMD -----------------------------*/
/**
   execute one command
 */
int				execute_built(t_prompt *p, t_list_tokens *e_tokens);
int				execute_one_sys(t_prompt *p, t_list_tokens *e_tokens);
/*----------------------------- END  EXECUTE_ONE_CMD  ------------------------*/

/*----------------------------- UTILS_EXECUTION ------------------------------*/
/**
   utils for execution
 */
void			init_file_fd(t_prompt *p);
void			open_infile_outfile(t_prompt *p, t_list_tokens *tmp);
void			wait_signal(pid_t wpid, t_prompt *p, int save_status, int ib);
int				get_result(t_prompt *p, t_list_tokens *e_tokens, int result);
/*----------------------------- END  UTILS_EXECUTION  -----------------------*/

int				check_in_env(t_prompt *p, char *str);
int				print_export(t_prompt *p, int fd);
int				export_arg(t_prompt *p, t_list_tokens *e_tokens);
int				add_or_replace_env(t_prompt *p, char *line, char *str);
int				ft_matrixlen(char **m);
int				print_perror_export(char *arg);
#endif

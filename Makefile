# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/16 11:46:30 by mflores-          #+#    #+#              #
#    Updated: 2023/03/16 17:41:56 by pmaimait         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#------------------------------------------------------------------------------#
#									GENERAL               				       #
#------------------------------------------------------------------------------#

NAME	= minishell
CC		= cc
FLAGS	= -Wall -Wextra -Werror -g
RM		= rm -f

#------------------------------------------------------------------------------#
#								HEADER FILES            				       #
#------------------------------------------------------------------------------#

HEADER_FILES	= minishell
HEADERS_PATH 	= includes/
HEADERS			= $(addsuffix .h, $(addprefix $(HEADERS_PATH), $(HEADER_FILES)))
HEADERS_INC		= $(addprefix -I, $(HEADERS_PATH) $(LIB_HEADER_PATH))

#------------------------------------------------------------------------------#
#									LIBFT           				   	   	   #
#------------------------------------------------------------------------------#

LIB_NAME 	= ft
LIB_PATH	= libft/
LIB			= -L$(LIB_PATH) -l$(LIB_NAME) -lreadline
LIB_HEADER_PATH = $(LIB_PATH)includes/

#------------------------------------------------------------------------------#
#								MINISHELL FILES           				   	   #
#------------------------------------------------------------------------------#

# List of all .c source files
ROOT_FILES = main
PARSING_FILES = parsing parsing_utils lexer tokens list_actions expansion \
				expansion_utils heredoc
PARSING_FOLDER = parsing/
ENV_FILES = env
ENV_FOLDER = env/
PROMPT_FILES = prompt
PROMPT_FOLDER = prompt/
SIGNALS_FILES = signal
SIGNALS_FOLDER = signals/
UTILS_FILES = utils utils2 utils3
UTILS_FOLDER = utils/
BUILTINS_FILES = echo_builtin env_builtin export_builtin unset_builtin \
				 exit_builtin pwd_builtin cd_builtin
BUILTINS_FOLDER = builtins/
DEBUG_FILES = structures
DEBUG_FOLDER = debug/
EXECUTION_FILES = pre_execute execution_sys execution execute_one_cmd \
				utils_execution
EXECUTION_FOLDER = execution/

SRCS_PATH = srcs/
SRCS_NAMES 	= $(addsuffix .c, $(ROOT_FILES) \
							$(addprefix $(PARSING_FOLDER), $(PARSING_FILES)) \
							$(addprefix $(ENV_FOLDER), $(ENV_FILES)) \
							$(addprefix $(PROMPT_FOLDER), $(PROMPT_FILES)) \
							$(addprefix $(SIGNALS_FOLDER), $(SIGNALS_FILES)) \
							$(addprefix $(UTILS_FOLDER), $(UTILS_FILES)) \
							$(addprefix $(EXECUTION_FOLDER), $(EXECUTION_FILES)) \
							$(addprefix $(DEBUG_FOLDER), $(DEBUG_FILES)) \
							$(addprefix $(BUILTINS_FOLDER), $(BUILTINS_FILES))) 

# All .o files go to objs directory
OBJS_NAMES	= $(SRCS_NAMES:.c=.o)
OBJS_FOLDERS = $(addprefix $(OBJS_PATH), $(PARSING_FOLDER) \
							 $(ENV_FOLDER) $(PROMPT_FOLDER) $(SIGNALS_FOLDER) \
							 $(UTILS_FOLDER) $(DEBUG_FOLDER) $(EXECUTION_FOLDER) \
							 $(BUILTINS_FOLDER)) 
OBJS_PATH 	= objs/
OBJS		= $(addprefix $(OBJS_PATH), $(OBJS_NAMES))

# Gcc/Clang will create these .d files containing dependencies
DEPS		= $(addprefix $(OBJS_PATH), $(SRCS_NAMES:.c=.d))

#------------------------------------------------------------------------------#
#								BASCIC RULES	        				       #
#------------------------------------------------------------------------------#

all:	header $(NAME)
	@echo "\n$(GREEN)[ ✔ ] MINISHELL$(WHITE)"
	@echo "\n▶ TO LAUNCH:\t./minishell\n"

# Actual target of the binary - depends on all .o files
$(NAME): lib $(HEADERS) $(OBJS)
# Link all the object files
	@$(CC) $(FLAGS) $(HEADERS_INC) $(OBJS) $(LIB) -o $(NAME)
# Build target for every single object file
# The potential dependency on header files is covered
# by calling `-include $(DEPS)`
$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
# Create objs directory
	@mkdir -p $(OBJS_FOLDERS)
  # The -MMD flags additionaly creates a .d file with
  # the same name as the .o file.
	@$(CC) $(FLAGS) $(HEADERS_INC) -MMD -MP -o $@ -c $<
	@printf "$(YELLOW). . . COMPILING MINISHELL OBJECTS . . . $(GREY)%-33.33s\r$(DEF_COLOR)" $@

lib:
	@$(MAKE) --no-print-directory -C $(LIB_PATH)
	@echo "\n$(GREEN)[ ✔ ] LIBFT$(DEF_COLOR)"

clean:
	@echo "$(YELLOW)\n. . . CLEANING OBJECTS . . .\n$(DEF_COLOR)"
	@$(MAKE) --no-print-directory clean -C $(LIB_PATH)
	@$(RM) -rd $(OBJS_PATH)
	@echo "$(GREEN)[ ✔ ] OBJECTS CLEANED$(DEF_COLOR)"

fclean:	clean
	@echo "$(YELLOW)\n. . . CLEANING REST . . .\n$(DEF_COLOR)"
	@$(MAKE) --no-print-directory fclean -C $(LIB_PATH)
	@$(RM) $(NAME)
	@echo "$(GREEN)[ ✔ ] ALL CLEANED$(DEF_COLOR)"

re:	fclean all

# Include all .d files
-include $(DEPS)

.PHONY:	all clean fclean re header norme check lib

#------------------------------------------------------------------------------#
#								CUSTOM RULES    					           #
#------------------------------------------------------------------------------#

define HEADER_PROJECT

		███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     
		████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     
		██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     
		██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     
		██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
		╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
                                                                   
endef
export HEADER_PROJECT

header:
		clear
		@echo "$$HEADER_PROJECT"

check:
	@grep -qe ${USER} -e ${MAIL} srcs/* includes/* && \
	echo "$(GREEN)[ ✔ ]$(WHITE)	Username and email" \
	|| echo "$(RED)[ ✗ ]$(BLUE)	Username and email"
	@ls | grep -q -U $(NAME) && \
	echo "$(GREEN)[ ✔ ]$(WHITE)	Executable name" || \
	echo "$(RED)[ ✗ ]$(BLUE)	Executable name"
	@$(MAKE) norme | grep -B 1 Error && \
	echo "$(RED)[ ✗ ]$(BLUE)	Norme" || \
	echo "$(GREEN)[ ✔ ]$(WHITE)	Norme"

norme:
	norminette $(HEADERS) $(SRCS_PATH)

#Colors
DEF_COLOR = \033[0;39m
GREY = \033[0;90m
RED = \033[0;91m
GREEN = \033[1;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[1;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
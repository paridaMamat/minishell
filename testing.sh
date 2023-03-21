#!/bin/bash
make && valgrind --suppressions=readline_leaks.supp --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
#make && env -i valgrind --suppressions=readline_leaks.supp --leak-check=full --show-leak-kinds=all ./minishell
#make && env -i valgrind --suppressions=readline_leaks.supp --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
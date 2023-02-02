/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mflores- <mflores-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 17:50:29 by mflores-          #+#    #+#             */
/*   Updated: 2023/01/26 15:11:59 by mflores-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/******************************************************************************/
/*                                                                            */
/*                                LIBS                                        */
/*                                                                            */
/******************************************************************************/
// General
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

/******************************************************************************/
/*                                                                            */
/*                              STRUCTURES                                    */
/*                                                                            */
/******************************************************************************/

/* typedef struct s_list	t_list;

struct s_list
{
	void			*content;
	t_list			*next;
}; */

/******************************************************************************/
/*                                                                            */
/*                               PROTOTYPES                                   */
/*                                                                            */
/******************************************************************************/

/*------------------------------ BOOLEANS -----------------------------------*/
// Checks for an alphabetic character
int		ft_isalpha(int c);

// Checks for a digit (0 through 9)
int		ft_isdigit(int c);

// Checks if it is an alphanumeric character
int		ft_isalnum(int c);

// Checks fot a lowercase character
int		ft_islower(int c);

// Checks for an uppercase letter.
int		ft_isupper(int c);

// Checks whether 'c' is a 7-bit unsigned char value that fits into the
// ASCII character set
int		ft_isascii(int c);

// Checks for any printable character including space.
int		ft_isprint(int c);

// Checks if 'c' is a space (\t, \n, \v, \f, \r)
int		ft_isspace(int c);
/*---------------------------- END BOOLEANS ----------------------------------*/

/*------------------------------ LISTS ---------------------------------------*/
// Returns the number of nodes in a linked list
//int		ft_lstsize(t_list *lst);

// Adds the node at the beginning of the linked list
//void	ft_lstadd_front(t_list **lst, t_list *new);

// Adds the node at the end of the linked list
//void	ft_lstadd_back(t_list **lst, t_list *new);

// Frees the allocated memory of the node passed as parameter using
// the functions del() and free()
//void	ft_lstdelone(t_list *lst, void (*del)(void *));

// Deletes and frees the memory allocated of the node passed as parameter
// and all the following nodes using ft_lstdelone() and free()
//void	ft_lstclear(t_list **lst, void (*del)(void *));

// Iterates over a linked list and applies the function passed as parameter
// to each node of the linked list
//void	ft_lstiter(t_list *lst, void (*f)(void *));

// Allocates memory and returns a new node. On failure returns NULL.
//t_list	*ft_lstnew(void *content);

// Returns the last node of the linked list. If empty list, returns NULL.
//t_list	*ft_lstlast(t_list *lst);

// Creates and returns a new list resulting from the changes applied 
// to the contents of each node of the function 'f'.
// On failure, returns NULL.
//t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
/*------------------------------ END LISTS --------------------------------*/

/*-------------------------------- MEM ------------------------------------*/
// Fills the first 'n' bytes of the memory area pointed to by 's' with the
// constant byte 'c'.
void	*ft_memset(void *s, int c, size_t n);

// Copies 'n' bytes from memory area 'src' to memory area 'dest'.
void	*ft_memcpy(void *dest, const void *src, size_t n);

// Copies 'n' bytes from memory area 'src' to memory area 'dest'
void	*ft_memmove(void *dest, const void *src, size_t n);

// Scans the initial 'n' bytes of the memory area pointed to by 's' for the 
// first instance of 'c'
void	*ft_memchr(const void *s, int c, size_t n);

// Allocates memory for an array of 'nmmb' elements of 'size' bytes each and
// returns a pointer to the allocated memory. Memory is set to zero.
void	*ft_calloc(size_t nmemb, size_t size);

// Compares the first 'n' bytes of the memory areas 's1' and 's2'.
int		ft_memcmp(const void *s1, const void *s2, size_t n);
/*------------------------------ END MEM -----------------------------------*/

/*------------------------------ STRINGS -----------------------------------*/
// If 'c' is a lowercase letter, the function returns its 
// uppercase equivalent otherwise it returns 'c'.
int		ft_toupper(char c);

// If 'c' is an uppercase letter, the function returns its 
// lowercase equivalent otherwise it returns 'c'.
int		ft_tolower(char c);

// Compares the two strings 's1' and 's2' except it compares only the first
// (at most) 'n' bytes of 's1' and 's2'.
int		ft_strncmp(const char *s1, const char *s2, size_t n);

// Converts a string to an integer.
int		ft_atoi(const char *nptr);

// Returns a pointer to the first occurrence of the character 'c' in the 
// string 's'.
char	*ft_strchr(const char *s, int c);

// Returns a pointer to the last occurrence of the character 'c' in the 
// string 's'.
char	*ft_strrchr(const char *s, int c);

// Locates the first occurrence of the null-terminated string little in the 
// string 'big', where not more than 'len' characters are searched.
char	*ft_strnstr(const char *big, const char *little, size_t len);

// Returns a pointer to a new string which is a duplicate of the string 's'.
char	*ft_strdup(const char *s);

// Allocates and returns a string from the string 's'
// This new string starts at index 'start' and has a maximum length of 'len'
char	*ft_substr(char const *s, unsigned int start, size_t len);

// Allocates and returns an array of strings obtained by
// separating a string 's' using the 'c' character as a delimiter.
char	**ft_split(char const *s, char c);

// Allocates and returns a string representing the integer 'n' received
// in argument.
char	*ft_itoa(int n);

// Applies the 'f' function to each character in the string
// passed as an argument to create a new string of characters resulting
// successive applications of 'f'
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

// Allocates and returns a new string, result of the concatenation of s1 and s2
char	*ft_strjoin(char const *s1, char const *s2);

// Allocates and returns a copy of the string 's1' without the 
// specified characters in 'set' at the beginning and end of the string.
char	*ft_strtrim(char const *s1, char const *set);

// Erases the data in the 'n' bytes of the memory starting at the location
// pointed to by 's' by writing zeros '\0' to that area.
void	ft_bzero(void *s, size_t n);

// Applies the 'f' function to each character in the string
// passed as an argument, and passing its index as the first argument
void	ft_striteri(char *s, void (*f)(unsigned int, char*));

// Write the character 'c' on the given file descriptor.
void	ft_putchar_fd(char c, int fd);

// Writes the string 's' to the given file descriptor.
void	ft_putstr_fd(char *s, int fd);

// Write the string 's' to the given file descriptor
// followed by a newline.
void	ft_putendl_fd(char *s, int fd);

// Writes the integer 'n' to the given file descriptor.
void	ft_putnbr_fd(int n, int fd);

// Copies up to 'size' - 1 characters from the NUL-terminated string 'src'
// to 'dst', NUL-terminating the result.
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

// Calculates the length of the string pointed to by 's', excluding the 
// terminating null byte '\0'.
size_t	ft_strlen(const char *s);

// Appends the NUL-terminated string 'src' to the end of 'dst'.
// It will append at most 'size' - strlen(dst) - 1 bytes, NUL-terminating 
// the result.
size_t	ft_strlcat(char *dst, const char *src, size_t size);
/*-------------------------- END STRINGS -----------------------------------*/
#endif

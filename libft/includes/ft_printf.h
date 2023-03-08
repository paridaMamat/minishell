/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parida <parida@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 22:48:31 by mflores-          #+#    #+#             */
/*   Updated: 2023/03/07 16:21:38 by parida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

/******************************************************************************/
/*                                                                            */
/*                                LIBS                                        */
/*                                                                            */
/******************************************************************************/

# include "libft.h"
# include <stdarg.h>

/******************************************************************************/
/*                                                                            */
/*                                MACROS                                      */
/*                                                                            */
/******************************************************************************/

# define BASE_DEC "0123456789"
# define HEXA_MIN "0123456789abcdef"
# define HEXA_MAJ "0123456789ABCDEF"
# define NIL_PTR "(nil)"
# define CONV "cspdiuxX%"

/******************************************************************************/
/*                                                                            */
/*                               PROTOTYPES                                   */
/*                                                                            */
/******************************************************************************/

// Mimics the original printf() but doesn't implement the buffer management.
// Handles the following convertions: (cspdiuxX%)
//  %c Prints a single character
//  %s Prints a string
//  %p The  void * pointer argument is printed in hexadecimal format
//  %d Prints a decimal (base 10) number
//  %i Prints an integer in base 10
//  %u Prints an unsigned decimal (base 10) number
//  %x Prints a number in hexadecimal (base 16) lowercase format
//  %X Prints a number in hexadecimal (base 16) uppercase format
//  %% Prints a percent sign
int		ft_printf(const char *format, ...);
int		ft_print_arg(const char *format, int i, va_list args);
int		ft_printchar(char c);
int		ft_printstr(char *str);
int		ft_printnbr(int nbr);
int		ft_printbase_u(unsigned int nbr, int base, const char *hexa);
int		ft_printptr_u(unsigned long nbr, int base, const char *hexa);
int		ft_putptr_u(unsigned long nbr, int base, const char *hexa);


#endif
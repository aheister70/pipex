/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   errors.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/09 10:35:54 by aheister      #+#    #+#                 */
/*   Updated: 2021/09/18 08:12:29 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** ERROR_HANDLING - writes an error message to given fd and exits with type = 2
*/

#include "pipex.h"

void	error_handling(int fd, char *message, char *value, int type)
{
	write(fd, "ERROR - ", 8);
	if (value != 0)
	{
		write(fd, value, ft_strlen(value));
		write(fd, ": ", 2);
	}	
	write(fd, message, ft_strlen(message));
	write(fd, "\n", 1);
	if (type == 2)
		exit (1);
}

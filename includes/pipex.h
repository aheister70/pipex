/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/02 16:19:10 by aheister      #+#    #+#                 */
/*   Updated: 2021/09/20 04:32:21 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "../lib/libft/libft.h"

typedef struct s_input
{
	char	*file1;
	char	*file2;
	t_list	*list_paths;
	t_list	*list_cmds;
	char	**envp;
	int		fds[2];
}			t_input;

typedef struct s_cmd
{
	char	*cmd[3];
}			t_cmd;

int		array_amount(char **split);
void	error_handling(int fd, char *message, char *value, int type);
void	free_array(char **object);
void	free_list(t_input *input);
void	init_input(int argc, char **argv, t_input *input);
int		init_paths(t_input *input);
void	pipex(t_input *input);
int		store_paths(char *path, t_input *input);

#endif
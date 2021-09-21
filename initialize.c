/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialize.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/08 16:07:23 by aheister      #+#    #+#                 */
/*   Updated: 2021/09/21 11:06:25 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** STORE_PATHS - stores paths in struct 
** INIT_PATHS - retrieves possible paths of the command binaries
** ADAPT_INPUT_CMD - puts the cmd input in the right format
** INIT_INPUT - initializes the input from the terminal 
**		and stores them in struct
*/

#include "pipex.h"

int	store_paths(char *path, t_input *input)
{
	int		i;
	char	**split_paths;

	i = 0;
	split_paths = ft_split(path, ':');
	if (split_paths == NULL)
		return (-1);
	while (split_paths[i] != NULL)
	{
		path = ft_strjoin(split_paths[i], "/");
		if (path != NULL)
			ft_lstadd_back(&input->list_paths, ft_lstnew(path));
		else
		{
			free_array(split_paths);
			return (-1);
		}	
		i++;
	}
	free_array(split_paths);
	return (0);
}	

int	init_paths(t_input *input)
{
	int		i;
	char	*path;

	i = 0;
	while (input->envp[i] != NULL)
	{
		path = ft_strnstr(input->envp[i], "PATH", 4);
		if (path != NULL)
		{
			path = ft_strtrim(path, "PATH=");
			if (path == 0)
				return (-1);
			if (store_paths(path, input) == -1)
			{
				free(path);
				return (-1);
			}	
			free(path);
			return (0);
		}	
		else
			i++;
	}
	return (-1);
}

void	adapt_input_cmd(char *s_cmd, t_input *input)
{
	t_cmd	*cmd;
	char	*cmd_0;
	char	*cmd_1;
	int		len;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		error_handling(1, "Malloc failed.", 0, 2);
	if (ft_strchr(s_cmd, ' ') != 0)
	{
		cmd_1 = ft_strchr(s_cmd, ' ');
		len = ft_strlen(s_cmd) - ft_strlen(cmd_1);
		cmd->cmd[0] = ft_substr(s_cmd, 0, len);
		cmd_0 = ft_strtrim(cmd_1, " ");
		cmd_1 = ft_strtrim(cmd_0, "'");
		cmd->cmd[1] = ft_strtrim(cmd_1, "\"");
		//free(s_cmd);
		free(cmd_0);
		free(cmd_1);
	}
	else
	{
		if (ft_strrchr(s_cmd, '/') != 0)
			cmd->cmd[0] = ft_strrchr(s_cmd, '/');
		else
			cmd->cmd[0] = s_cmd;
		cmd->cmd[1] = NULL;
	}
	cmd->cmd[2] = NULL;
	ft_lstadd_back(&input->list_cmds, ft_lstnew(cmd));
}

void	init_input(int argc, char **argv, t_input *input)
{
	int	i;

	input->file1 = argv[1];
	input->file2 = argv[argc - 1];
	i = 2;
	while (i < (argc - 1))
	{
		adapt_input_cmd(argv[i], input);
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/02 16:18:48 by aheister      #+#    #+#                 */
/*   Updated: 2021/09/20 20:15:26 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// waitpid gaat niet goed bij middelste cmds
// test 7 gaat nog steeds niet goed
// alle malloc protecten (in initiate)
// norminette van alles nog niet goed
// memory leaks oplossen
// test met een hele grote input file ivm maximale grootte 64kb
// opsplitsen in bonus en mandatory

// optioneel: als eerste parameter here_doc is << en >> ondersteunen

/*
** EXECUTE_CMD - executes the cmds
** CHILD_FORK - function calles by pipex for every cmd to fork the process
** CHILD_PROCESS - dupps pipe ends to the right fds
** PIPEX - opens the pipe and forks into 2 children
** MAIN - sends input stdin to initialize and opens 2 files to send to pipex
*/

#include "pipex.h"

void	execute_cmd(t_input *input, char **cmd)
{
	char	*path;

	while (input->list_paths)
	{
		path = ft_strjoin(input->list_paths->content, cmd[0]);
		if (path == 0)
			error_handling(STDERR_FILENO, "Malloc failed.", cmd[0], 2);
		if (access(path, (F_OK & X_OK)) != -1)
		{
			if (execve(path, cmd, input->envp) == -1)
				error_handling(STDERR_FILENO, "Execution failed.", cmd[0], 2);
		}
		free(path);
		input->list_paths = input->list_paths->next;
	}
	error_handling(STDERR_FILENO, "Command not found.", cmd[0], 2);
}

void	child_process(int *pipes_end, t_input *input, char **cmd, int type)
{
	int		dup[2];

	if (type == 1)
	{
		dup[0] = dup2(input->fds[0], STDIN_FILENO);
		dup[1] = dup2(pipes_end[1], STDOUT_FILENO);
		close(input->fds[0]);
	}
	else if (type == 2)
	{
		dup[0] = dup2(pipes_end[0], STDIN_FILENO);
		dup[1] = dup2(pipes_end[1], STDOUT_FILENO);
	}
	else if (type == 3)
	{
		dup[0] = dup2(pipes_end[0], STDIN_FILENO);
		dup[1] = dup2(input->fds[1], STDOUT_FILENO);
		close(input->fds[0]);
	}
	close(pipes_end[0]);
	close(pipes_end[1]);
	execute_cmd(input, cmd);
}

int	child_fork(int *pipes_end, t_input *input, char **cmd, int type)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		error_handling(1, "Fork has failed.", 0, 2);
	if (pid == 0)
	{
		if (type == 1)
			child_process(pipes_end, input, cmd, 1);
		else if (type == 3)
			child_process(pipes_end, input, cmd, 3);
		else
			child_process(pipes_end, input, cmd, 2);
	}
	if (pid > 0)
	{
		if (type == 3)
			close(pipes_end[1]);
	}
	return (pid);
}

void	pipex(t_input *input)
{
	int		pipes_end[2];
	int		i;
	int		len;
	int		status;

	if (pipe(pipes_end) < 0)
		error_handling(1, "Pipe has failed.", 0, 2);
	i = 1;
	status = 0;
	len = ft_lstsize(input->list_cmds);
	while (input->list_cmds)
	{
		if (i == 1)
			child_fork(pipes_end, input, input->list_cmds->content, 1);
		else if (i == len)
			child_fork(pipes_end, input, input->list_cmds->content, 3);
		else
			child_fork(pipes_end, input, input->list_cmds->content, 2);
		if (i == 1 || i == len)
			waitpid(-1, &status, 0);
		free_array(input->list_cmds->content);
		input->list_cmds = input->list_cmds->next;
		i++;
	}
	close(pipes_end[0]);
}

int	main(int argc, char	**argv, char **envp)
{
	t_input	input;

	if (argc < 5)
		error_handling(1, "Usage: './pipex file1 cmd1 cmd2 file2'.\n", 0, 2);
	input.envp = envp;
	init_input(argc, argv, &input);
	if (init_paths(&input) == -1)
		error_handling(1, "Malloc fails.\n", 0, 2);
	input.fds[0] = open(input.file1, O_RDONLY);
	if (input.fds[0] < 0)
		error_handling(1, "File 1 cannot be opened.\n", 0, 2);
	input.fds[1] = open(input.file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (input.fds[1] < 0)
		error_handling(1, "File 2 cannot be opened or created.\n", 0, 2);
	pipex(&input);
	close(input.fds[0]);
	close(input.fds[1]);
	free_list(&input);
	system ("leaks pipex");
	return (0);
}

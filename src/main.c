/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:26 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/27 13:43:54 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	here_doc(char *limiter)
{
	char	*line;
	int		fd[2];

	if (pipe(fd) == -1)
		return (perror("Failed to create pipe"), 1);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(line) - 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}

int	check_input(int argc, char **argv, int *fd, int *i)
{
	if (argc < 5)
		return (perror("Not enough arguments"), 1);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		*fd = here_doc(argv[2]);
		*i = 2;
		return (0);
	}
	*fd = open(argv[1], O_RDONLY);
	if (*fd == -1)
		return (perror("Failed to open file"), 1);
	if (access(argv[1], F_OK) == -1)
		return (perror("Input file does not exist"), 1);
	if (access(argv[1], R_OK) == -1)
		return (perror("Input file is not readable"), 1);
	return (0);
}

int	exec_cmd_redirect_out(int input_fd, char **cmd_args, \
						char *cmd_path, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		handle_error_exit(0, input_fd, cmd_args, cmd_path);
	pid = fork();
	if (pid == -1)
		handle_error_exit(1, input_fd, cmd_args, cmd_path);
	if (pid == 0)
	{
		close(fd[0]);
		if (dup2(input_fd, STDIN_FILENO) == -1)
			handle_error_exit(2, input_fd, cmd_args, cmd_path);
		close(input_fd);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			handle_error_exit(0, input_fd, cmd_args, cmd_path);
		close(fd[1]);
		execve(cmd_path, cmd_args, envp);
	}
	close(fd[1]);
	close(input_fd);
	return (fd[0]);
}

int	redirect_execute_cat(char *outfile, int fd, char **envp)
{
	pid_t	pid;
	int		file_fd;

	pid = fork();
	if (pid == 0)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("Failed to redirect standard input"), close(fd), 1);
		close(fd);
		file_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd == -1)
			return (perror("Failed to open output file"), 1);
		if (dup2(file_fd, STDOUT_FILENO) == -1)
			return (perror("Failed to redirect standard output"), \
					close(file_fd), 1);
		close(file_fd);
		execve("/bin/cat", (char *[]){"cat", NULL}, envp);
		perror("Failed to execute cat");
		exit(1);
	}
	close(fd);
	wait(NULL);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd;
	int		i;
	char	*path;
	char	**cmd_args;
	char	*cmd_path;

	i = 1;
	if (check_input(argc, argv, &fd, &i) == 1)
		return (1);
	path = get_path(envp);
	while (argv[++i] && argv[i + 1])
	{
		if (is_valid_cmd(argv[i], path, fd) == 0)
			continue ;
		cmd_args = ft_split(argv[i], ' ');
		cmd_path = get_cmd_path(cmd_args[0], path);
		fd = exec_cmd_redirect_out(fd, cmd_args, cmd_path, envp);
		free(cmd_path);
		free_split(cmd_args);
	}
	if (i == argc - 1)
		return (redirect_execute_cat(argv[argc - 1], fd, envp), 0);
	return (1);
}

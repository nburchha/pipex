/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:26 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/14 14:40:08 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void check_fds(int start, int end) {
    for (int fd = start; fd < end; fd++) {
        if (fcntl(fd, F_GETFD) != -1 || errno != EBADF) {
            printf("File descriptor %d is open\n", fd);
        }
    }
}

int	exec_cmd_redirect_out(int input_fd, char **cmd_args, char *cmd_path, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (perror("Failed to create pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("Failed to fork"), 1);
	if (pid == 0)
	{
		//child process
		close(fd[0]);
		if (dup2(input_fd, STDIN_FILENO) == -1)
			return (perror("Failed to redirect standard input"), 1);
		close(input_fd);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (perror("Failed to redirect standard output"), 1);
		close(fd[1]);
		execve(cmd_path, cmd_args, envp);
		exit(1); // Exit if execve fails
	}
	close(fd[1]);
	close(input_fd);
	return (fd[0]);
}

char	*get_path(char **envp)
{
	int		i;

	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return envp[i] + 5;
	return NULL;
}

int	check_input(int argc, char **argv)
{
	if (argc < 5)
		return (perror("Not enough arguments"), 1);
	if (access(argv[1], F_OK) == -1)
		return (perror("Input file does not exist"), 1);
	if (access(argv[1], R_OK) == -1)
		return (perror("Input file is not readable"), 1);
	// if (access(argv[argc - 1], W_OK) == -1)
	// 	return (perror("Output file is not writable"), 1);
	return (0);
}

int	last_step(char *outfile, int fd, char **envp)
{
	pid_t	pid;
	int		file_fd;
	
	// Final process to read from the pipe and write to file2
	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("Failed to redirect standard input"), close(fd), 1);
		close(fd);
		file_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd == -1)
			return (perror("Failed to open output file"), 1);
		if (dup2(file_fd, STDOUT_FILENO) == -1)
			return (perror("Failed to redirect standard output"), close(file_fd), 1);
		close(file_fd);
		execve("/bin/cat", (char *[]){"cat", NULL}, envp);
		exit(1); // Exit if execve fails
	}
	close(fd);	// Close the read end of the pipe in parent
	wait(NULL); // Wait for the child process to finish
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	int		fd;
	int		i;
	char	*path;
	char	**cmd_args;
	char	*cmd_path;

	if (check_input(argc, argv) == 1)
		return (1);
	path = get_path(envp);
	if (!path)
		return (perror("Failed to get path"), 1);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (perror("Failed to open file"), 1);
	i = 1;
	while (is_valid_cmd(argv[++i], path) == 1)
	{
		cmd_args = ft_split(argv[i], ' ');
		cmd_path = get_cmd_path(cmd_args[0], path);
		fd = exec_cmd_redirect_out(fd, cmd_args, cmd_path, envp);
		free(cmd_path);
		free_split(cmd_args);
	}
	if (i != argc - 1)
		return (perror("Invalid arguments"), close(fd), 1);
	if (last_step(argv[i], fd, envp) == 1)
		return (1);
	return (0);
	}

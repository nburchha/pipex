/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:26 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/14 12:13:07 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

// void	init_cmd(char **argv, char **envp, t_command *command)
// {
// 	char	**dir;
// 	char	*path;
// 	char	fullpath[1024];
// 	char	**split_cmd;
// 	int		i;

// 	command->path = NULL;
// 	i = -1;
// 	path = search_path(envp);
// 	if (!path)
// 		handle_error(2);
// 	dir = ft_split(path, ':');
// 	command->cmd = ft_split(argv, ' ');
// 	i = -1;
// 	while (dir && dir[++i])
// 	{
// 		ft_strlcpy(fullpath, dir[i], ft_strlen(dir[i]));
// 		ft_strlcat(fullpath, split_cmd[0], ft_strlen(fullpath) + ft_strlen(split_cmd[0]));
// 		if (access(path, X_OK) == 0)
// 			return (free_split(dir), 1);
// 	}
// 	free_split(split_cmd);
// 	free_split(dir);
// 	return (0);
// }

// void	exec_cmd_fd(char **argv, int fd, char **envp)
// {
// 	pid_t		pid;
// 	t_command	cmd;

// 	init_cmd(argv, envp, &cmd);
// 	pid = fork();
// 	if (pid == -1)
// 		return (perror("Failed to fork"), 1);
// 	if (pid == 0)
// 	{
// 		//child process
// 		if (dup2(fd, STDIN_FILENO) == -1)
// 			return (perror("Failed to redirect standard input"), 1);
// 		execve()
// 	}
// }

// int main(int argc, char **argv, char **envp)
// {
// 	// if (argc > 5 || !envp || !*envp)
// 	// 	return (1);
// 	if (argc == 0)
// 		return (1);
// 	ft_printf("validcmd: %d\n", is_valid_cmd(argv[1], search_path(envp)));
// 	return 0;
// }

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

void check_fds(int start, int end) {
    for (int fd = start; fd < end; fd++) {
        if (fcntl(fd, F_GETFD) != -1 || errno != EBADF) {
            printf("File descriptor %d is open\n", fd);
        }
    }
}

int main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		fd;
	int		i;
	char	*path;
	char	**cmd_args;
	char	*cmd_path;

	if (argc < 5 || !envp || !*envp)
		return (1);
	path = get_path(envp);
	if (!path)
		return (perror("Failed to get path"), 1);
	// open file and redirect output to pipe
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (perror("Failed to open file"), 1);
	// execute commands and redirect output to pipe
	i = 1;
	while (is_valid_cmd(argv[++i], path) == 1)
	{
		cmd_args = ft_split(argv[i], ' ');
		cmd_path = get_cmd_path(cmd_args[0], path);
		fd = exec_cmd_redirect_out(fd, cmd_args, cmd_path, envp);
		free(cmd_path);
		free_split(cmd_args);
	}
		// Final process to read from the pipe and write to file2
		pid = fork();
		if (pid == 0)
		{
			// Child process
			if (dup2(fd, STDIN_FILENO) == -1)
				return (perror("Failed to redirect standard input"), 1);
			close(fd);

			int file_fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (file_fd == -1)
				return (perror("Failed to open file2"), 1);

			if (dup2(file_fd, STDOUT_FILENO) == -1)
				return (perror("Failed to redirect standard output"), 1);
			close(file_fd);

			execve("/bin/cat", (char *[]){"cat", NULL}, envp);
			exit(1); // Exit if execve fails
		}
		close(fd);	// Close the read end of the pipe in parent
		wait(NULL); // Wait for the child process to finish
		check_fds(0, 1024);
		return (0);
	}

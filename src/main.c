/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:26 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/11 10:56:18 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	init_cmd(char **argv, char **envp, t_command *command)
{
	char	**dir;
	char	*path;
	char	fullpath[1024];
	char	**split_cmd;
	int		i;

	command->path = NULL;
	i = -1;
	path = search_path(envp);
	if (!path)
		handle_error(2);
	dir = ft_split(path, ':');
	command->cmd = ft_split(argv, ' ');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(fullpath, dir[i], ft_strlen(dir[i]));
		ft_strlcat(fullpath, split_cmd[0], ft_strlen(fullpath) + ft_strlen(split_cmd[0]));
		if (access(path, X_OK) == 0)
			return (free_split(dir), 1);
	}
	free_split(split_cmd);
	free_split(dir);
	return (0);
}

void	exec_cmd_fd(char **argv, int fd, char **envp)
{
	pid_t		pid;
	t_command	cmd;

	init_cmd(argv, envp, &cmd);
	pid = fork();
	if (pid == -1)
		return (perror("Failed to fork"), 1);
	if (pid == 0)
	{
		//child process
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("Failed to redirect standard input"), 1);
		execve()
	}
}

int main(int argc, char **argv, char **envp)
{
	if (argc != 5 || !envp || !*envp)
		return (1);
	if (argc == 0)
		return (1);
	check_parse_input(argv, envp);
	ft_printf("valid input!\n");
	
	return 0;
}

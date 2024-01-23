/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/23 16:26:55 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*get_cmd_path(char *cmd, char *path)
{
	char	**dir;
	char	c_path[1024];
	char	**splt_cmd;
	int		i;

	splt_cmd = ft_split(cmd, ' ');
	if (access(splt_cmd[0], X_OK) == 0)
	{
		path = ft_strdup(splt_cmd[0]);
		return (free_split(splt_cmd), path);
	}
	dir = ft_split(path, ':');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(c_path, dir[i], ft_strlen(dir[i]) + 1);
		ft_strlcat(c_path, "/", ft_strlen(c_path) + 2);
		ft_strlcat(c_path, splt_cmd[0], ft_strlen(c_path) + \
					ft_strlen(splt_cmd[0]) + 1);
		if (access(c_path, X_OK) == 0)
			return (free_split(dir), free_split(splt_cmd), ft_strdup(c_path));
	}
	free_split(splt_cmd);
	free_split(dir);
	return (perror("Command not found"), NULL);
}

void	empty_pipe(int fd)
{
	char	*buffer[1024];
	int		i;

	i = 1;
	while (i > 0)
	{
		i = read(fd, buffer, 1024);
	}
}

int	is_valid_cmd(char *cmd, char *path, int fd)
{
	char	**dir;
	char	c_path[1024];
	char	**split_cmd;
	int		i;

	if (!path || !cmd)
		return (0);
	split_cmd = ft_split(cmd, ' ');
	if (access(split_cmd[0], X_OK) == 0)
		return (free_split(split_cmd), 1);
	dir = ft_split(path, ':');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(c_path, dir[i], ft_strlen(dir[i]) + 1);
		ft_strlcat(c_path, "/", ft_strlen(c_path) + 2);
		ft_strlcat(c_path, split_cmd[0], ft_strlen(c_path) + \
					ft_strlen(split_cmd[0]) + 1);
		if (access(c_path, X_OK) == 0)
			return (free_split(dir), free_split(split_cmd), 1);
	}
	free_split(split_cmd);
	free_split(dir);
	empty_pipe(fd);
	return (perror("Command not found"), 0);
}

void	handle_error_exit(int errnum, int fd, char **cmd_args, char *c_path)
{
	char	*msg[5];

	msg[0] = "Failed to create pipe";
	msg[1] = "Failed to fork";
	msg[2] = "Failed to redirect standard input";
	perror(msg[errnum]);
	if (fd > 2)
		close(fd);
	free_split(cmd_args);
	free(c_path);
	exit(1);
}

char	*get_path(char **envp)
{
	int		i;

	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
	return (NULL);
}

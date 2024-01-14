/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/14 16:25:47 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*get_cmd_path(char *cmd, char *path)
{
	char	**dir;
	char	fullpath[1024];
	char	**split_cmd;
	int		i;

	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	split_cmd = ft_split(cmd, ' ');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(fullpath, dir[i], ft_strlen(dir[i]) + 1);
		ft_strlcat(fullpath, "/", ft_strlen(fullpath) + 2);
		ft_strlcat(fullpath, split_cmd[0], ft_strlen(fullpath) + \
					ft_strlen(split_cmd[0]) + 1);
		if (access(fullpath, X_OK) == 0)
			return (free_split(dir), free_split(split_cmd), \
					ft_strdup(fullpath));
	}
	free_split(split_cmd);
	free_split(dir);
	return (NULL);
}

int	is_valid_cmd(char *cmd, char *path)
{
	char	**dir;
	char	fullpath[1024];
	char	**split_cmd;
	int		i;

	if (!path)
		return (0);
	dir = ft_split(path, ':');
	split_cmd = ft_split(cmd, ' ');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(fullpath, dir[i], ft_strlen(dir[i]) + 1);
		ft_strlcat(fullpath, "/", ft_strlen(fullpath) + 2);
		ft_strlcat(fullpath, split_cmd[0], ft_strlen(fullpath) + \
					ft_strlen(split_cmd[0]) + 1);
		if (access(fullpath, X_OK) == 0)
			return (free_split(dir), free_split(split_cmd), 1);
		if (errno == 2)
			errno = 0;
	}
	free_split(split_cmd);
	free_split(dir);
	return (0);
}

void	handle_error_exit(int errnum, int fd, char **cmd_args, char *cmd_path)
{
	char	*msg[5];

	msg[0] = "Failed to create pipe";
	msg[1] = "Failed to fork";
	msg[2] = "Failed to redirect standard input";
	perror(msg[errnum]);
	if (fd > 2)
		close(fd);
	free_split(cmd_args);
	free(cmd_path);
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

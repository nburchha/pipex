/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parse_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/11 10:16:54 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"


char	*search_path(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
	return (NULL);
}

int	check_valid_cmd(char *cmd, char **envp)
{
	char	*path;
	char	**dir;
	char	fullpath[1024];
	char	**split_cmd;
	int		i;

	path = NULL;
	i = -1;
	path = search_path(envp);
	if (!path)
		return (0);
	dir = ft_split(path, ':');
	split_cmd = ft_split(cmd, ' ');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(fullpath, dir[i], ft_strlen(dir[i]));
		ft_strlcat(fullpath, split_cmd[0], ft_strlen(fullpath) + ft_strlen(split_cmd[0]));
		if (access(fullpath, X_OK) == 0)
			return (free_split(dir), 1);
	}
	free_split(split_cmd);
	free_split(dir);
	return (0);
}

int	check_file_permission(char *file, int access_type)
{
	int	fd;
	fd = open(file, access_type);
	if (fd == -1)
		return (close(fd), 0);
	else
		return (close(fd), 1);
}

/*
handle the input parsing.
*/
void	check_parse_input(char **argv, char **envp)
{
	//if input is not right
	if (check_file_permission(argv[1], O_RDONLY) == 0)
	{
		handle_error(0);
		exit(1);
	}
	else if (check_file_permission(argv[1], O_WRONLY) == 0)
	{
		handle_error(1);
		exit(1);
	}
	else if (check_valid_cmd(argv[2], envp) == 0)
	{
		handle_error(2);
		exit(1);
	}
	else if (check_valid_cmd(argv[3], envp) == 0)
	{
		handle_error(2);
		exit(1);
	}
}

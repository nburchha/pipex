/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niklasburchhardt <niklasburchhardt@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 18:44:19 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/13 22:24:48 by niklasburch      ###   ########.fr       */
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

int	is_valid_cmd(char *cmd, char *path)
{
	char	**dir;
	char	fullpath[1024];
	char	**split_cmd;
	int		i;

	
	i = -1;
	if (!path)
		return (1);
	dir = ft_split(path, ':');
	split_cmd = ft_split(cmd, ' ');
	i = -1;
	while (dir && dir[++i])
	{
		ft_strlcpy(fullpath, dir[i], ft_strlen(dir[i]));
		ft_strlcat(fullpath, split_cmd[0], ft_strlen(fullpath) + ft_strlen(split_cmd[0]));
		if (access(fullpath, X_OK) == 0)
			return (free_split(dir), free_split(split_cmd), 1);
	}
	free_split(split_cmd);
	free_split(dir);
	return (0);
}

int	check_file_redirect_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (close(fd), -1); //evtl handle_error_exit function
	else
		return (fd);
}
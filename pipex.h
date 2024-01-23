/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:07:03 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/23 16:15:05 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "libs42/libs42.h"

char	*get_path(char **envp);
int		is_valid_cmd(char *cmd, char *path, int fd);
char	*get_cmd_path(char *cmd, char *path);
void	handle_error_exit(int errnum, int fd, char **cmd_args, char *cmd_path);

#endif
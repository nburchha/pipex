/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:07:03 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/14 12:50:35 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

#include <unistd.h>    // for open, close, read, write, access, dup, dup2, execve, fork, pipe, unlink
#include <stdlib.h>    // for malloc, free, exit
#include <string.h>    // for strerror
#include <stdio.h>     // for perror
#include <sys/wait.h>  // for wait, waitpid
#include <fcntl.h>
#include <errno.h>


#include "libs42/libs42.h"

int		is_valid_cmd(char *cmd, char *path);
char	*search_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:07:03 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/11 10:54:32 by nburchha         ###   ########.fr       */
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

typedef struct s_command
{
	char	*path; //path to cmd binary file
	char	**cmd; //cmd[0] = cmd, cmd[1] = arg1, ...
}		t_command;

void	handle_error(int errnum);
void	check_parse_input(char **argv, char **envp);
int		check_file_permission(char *file, int access_type);
char	*search_path(char **envp);

#endif
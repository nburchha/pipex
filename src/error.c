/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:21:21 by nburchha          #+#    #+#             */
/*   Updated: 2024/01/11 10:55:52 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

// void	free_all()
// {
	
// }

void	handle_error(int errnum)
{
	char *error[5];

	error[0] = "No write Permissions\n";
	error[1] = "No Read Permissions\n";
	error[2] = "Not a valid Command\n";
	//maybe add string array with different error messages
	if (strerror(errno) && errno != 0)
		perror(strerror(errno));
	else if (errnum == 0)
		perror("Error!\n");
	else
		perror(error[errnum]);

	if (errnum >= 0 && errnum <= 2)
		exit(EXIT_FAILURE);
	else
		exit(errnum);
}

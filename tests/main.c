/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:01:19 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/03 15:15:59 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int	main(int ac, char *av[])
{
	int		fd;
	char	*line;

	if (ac != 2)
	{
		printf("provide filename\n");
		return (1);
	}
	if (!strncmp(av[1], "stdin", 5))
		fd = STDIN_FILENO;
	else
		fd = open(av[1], O_RDONLY);
	if (fd == -1)
	{
		printf("error opening %s\n", av[1]);
		return (1);
	}
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
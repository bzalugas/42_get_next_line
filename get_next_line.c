/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:21:21 by bazaluga          #+#    #+#             */
/*   Updated: 2024/11/07 08:26:43 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(char *s)
{
	size_t	len;
	char	*new;

	len = 0;
	while (s[len])
		len++;
	new = malloc(sizeof(char) * len + 1);
	if (!new)
		return (NULL);
	len = 0;
	while (s[len])
	{
		new[len] = s[len];
		len++;
	}
	new[len] = 0;
	return (new);
}

char	*get_next_line(int fd)
{
	static char	stash[BUFFER_SIZE + 1L];
	char		line[100000];
	static int	i_stash;
	static int	len_read;
	int			i;

	if (fd < 0 || (fd > 0 && read(fd, stash, 0) == -1))
		return (NULL);
	i = 0;
	while (1)
	{
		if (i_stash >= len_read)
		{
			len_read = read(fd, stash, BUFFER_SIZE);
			i_stash = 0;
			if (len_read <= 0)
				break ;
			stash[len_read] = 0;
		}
		line[i++] = stash[i_stash++];
		if (line[i - 1] == '\n')
			break ;
	}
	line[i] = 0;
	if (i == 0)
		return (NULL);
	return (ft_strdup(line));
}

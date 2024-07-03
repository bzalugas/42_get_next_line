/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:21:21 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/03 17:52:51 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

void	*ft_calloc(size_t n, size_t size)
{
	unsigned char	*ptr;
	size_t			i;
	size_t			len;

	len = size * n;
	ptr = (unsigned char *)malloc(len);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}

int	alloc_buf(t_gnl_buf *buf, char *stash)
{
	char	*tmp;
	size_t	i;

	tmp = NULL;
	if (buf->len == 0)
		buf->len = BUFFER_SIZE;
	else
	{
		tmp = buf->arr;
		buf->len *= 2;
	}
	buf->arr = (char *)ft_calloc(buf->len + 1, sizeof(char));
	if (!buf->arr)
		return (-1);
	if (!tmp)
		tmp = stash;
	i = 0;
	while (tmp[i++])
		buf->arr[i - 1] = tmp[i - 1];
	if (!stash)
		free(tmp);
	if (stash && stash[0])
		return (alloc_buf(buf, NULL));
	return (0);
}

char	*get_the_line(char **line, char *str, size_t *i, char get_nl)
{
	size_t	j;

	*i = 0;
	while(str && str[*i] && str[*i] != '\n')
		(*i)++;
	if ((!str[*i] && (get_nl || *i == 0)))
		return (*line = NULL, NULL);
	*line = (char *)ft_calloc((*i) + (str[*i] == '\n') + 1, sizeof(char));
	if (!*line)
		return (NULL);
	j = 0;
	while (j < *i + (str[*i] == '\n'))
	{
		(*line)[j] = str[j];
		j++;
	}
	j = 0;
	while (str[*i] && str[*i + 1 + j])
	{
		str[j] = str[*i + 1 + j];
		j++;
	}
	str[j] = 0;
	return (*line);
}

char	*end_gnl(t_gnl_buf *buf, char *line, char *stash)
{
	size_t	i;

	if (buf->len > 0)
	{
		i = 0;
		while (buf->arr[i])
		{
			stash[i] = buf->arr[i];
			i++;
		}
		stash[i] = 0;
	}
	free(buf->arr);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	stash[STASH_SIZE] = (char[STASH_SIZE]){0};
	t_gnl_buf	buf;
	char		*line;
	size_t		nl;
	int			res;

	if (read(fd, NULL, 0) == -1)
		return (NULL);
	buf = (t_gnl_buf){0, NULL};
	if (get_the_line(&line, stash, &nl, 1))
		return (end_gnl(&buf, line, stash));
	if (alloc_buf(&buf, stash) == -1)
		return (NULL);
	res = read(fd, &buf.arr[nl], BUFFER_SIZE);
	while (res == BUFFER_SIZE && !get_the_line(&line, buf.arr, &nl, 1))
	{
		if (alloc_buf(&buf, NULL) == -1)
			return (free(buf.arr), NULL);
		res = read(fd, &buf.arr[nl], BUFFER_SIZE);
	}
	if (res < BUFFER_SIZE && res != -1)
	{
		get_the_line(&line, buf.arr, &nl, 0);
		return (end_gnl(&buf, line, stash));
	}
	if (line)
		return (end_gnl(&buf, line, stash));
	return (end_gnl(&buf, line, stash));
}

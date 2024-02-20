/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/20 18:26:51 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	buff_init(t_buffer *buf)
{
	ft_memset(buf, '\0', sizeof(t_buffer));
}

static t_buffer	*buff_get_last(t_buffer *buf)
{
	t_buffer	*tmp;

	tmp = buf;
	while(tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

static t_buffer	*buff_get_new(t_buffer *buf)
{
	t_buffer	*new;
	t_buffer	*last;

	new = (t_buffer *)malloc(sizeof(t_buffer));
	if (!new)
		return (NULL);
	buff_init(new);
	last = buff_get_last(buf);
	last->next = new;
	new->n = last->n + 1;
	return (new);
}

static char	*create_save_line(t_buffer *buf, t_buffer *last, char *remainder)
{
	size_t		len;
	char		*line;
	t_buffer	*tmp;
	char *start;
	int len2;

	len = BUFFER_SIZE * last->n + (last->nl - last->read) + 1;
	line = (char *)ft_calloc(len + 1, sizeof(char));
	if (!line)
		return (NULL);
	while (buf)
	{
		start = line + buf->n * BUFFER_SIZE;
		len2 = BUFFER_SIZE -((buf->read + BUFFER_SIZE) - buf->nl) + 1;
		ft_memcpy(start, buf->read, len2);
		tmp = buf->next;
		if (buf->nl)
			ft_memcpy(remainder, buf->nl + 1, (buf->read + BUFFER_SIZE) - buf->nl);
		if (buf->n > 0)
			free(buf);
		buf = tmp;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	remainder[FD_MAX][BUFFER_SIZE + 1L];
	t_buffer	buf;
	t_buffer	*last;
	ssize_t		ret;
	char		*line;

	if (fd < 0 || read(fd, buf.read, 0) < 0)
		return (NULL);
	buff_init(&buf);
	//get remainder in buf
	if (remainder[fd][0])
		ft_memcpy(buf.read, remainder[fd], BUFFER_SIZE);
	ret = read(fd, buf.read, BUFFER_SIZE);
	last = &buf;
	last->nl = ft_strchr(last->read,'\n');
	while (ret > 0 && !last->nl)
	{
		ret = read(fd, buff_get_new(&buf)->read, BUFFER_SIZE);
		last = buff_get_last(&buf);
		last->nl = ft_strchr(last->read,'\n');
	}
	if (ret == -1)
		return (NULL);
	line = create_save_line(&buf, last, remainder[fd]);
	return (line);
}

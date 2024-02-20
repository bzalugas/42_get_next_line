/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/20 13:50:52 by bazaluga         ###   ########.fr       */
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

	len = BUFFER_SIZE + (BUFFER_SIZE * last->n - (last->nl - last->read));
	line = (char *)ft_calloc(len, sizeof(char));
	if (!line)
		return (NULL);
	while (buf)
	{
		ft_memcpy(line + buf->n * BUFFER_SIZE, buf->read, BUFFER_SIZE -
				  (BUFFER_SIZE - (size_t)buf->nl));
		tmp = buf->next;
		if (buf->nl)
			ft_memcpy(remainder, buf->nl, BUFFER_SIZE - (size_t)buf->nl);
		if (buf->n > 0)
			free(buf);
		buf = tmp;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	remainder[BUFFER_SIZE + 1L][FD_MAX];
	t_buffer	buf;
	t_buffer	*last;
	ssize_t		ret;
	char		*line;

	if (fd < 0 || read(fd, buf.read, 0) < 0)
		return (NULL);
	buff_init(&buf);
	//get remainder in buf
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

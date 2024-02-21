/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/21 03:02:37 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	ft_memset(new, '\0', sizeof(t_buffer));
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

	if (!last->nl)
		last->nl = last->read + ft_strlen(last->read);
	len = ft_strlen(remainder) + BUFFER_SIZE * last->n
		+ (last->nl - last->read) + 1;
	line = (char *)ft_calloc(len + 1, sizeof(char));
	if (!line)
		return (NULL);
	while (buf)
	{
		if (buf->nl)
			ft_memmove(line + buf->n * BUFFER_SIZE, buf->read, buf->nl - buf->read + 1);
		else
			ft_memmove(line + buf->n * BUFFER_SIZE, buf->read, ft_strlen(buf->read));
		tmp = buf->next;
		if (buf->nl)
			ft_memmove(remainder, buf->nl + 1, ft_strlen(buf->nl + 1));
		if (buf->n > 0)
			free(buf);
		buf = tmp;
	}
	return (line);
}

static char	*get_remaining_line(char *remainder)
{
	char	*line;
	char	*nl;

	nl = ft_strchr(remainder, '\n');
	line = (char *)ft_calloc((nl - remainder) + 2, sizeof(char));
	if (!line)
		return (NULL);
	ft_memmove(line, remainder, nl - remainder + 1);
	ft_memmove(remainder, nl + 1, ft_strlen(nl + 1));//pb here solved (about len)
	return (line);
}

char	*get_next_line(int fd)
{
	static char	remainder[4][BUFFER_SIZE + 1L];
	t_buffer	buf;
	t_buffer	*last;
	ssize_t		ret;
	char		*line;

	if (fd < 0 || read(fd, buf.read, 0) < 0)
		return (NULL);
	ft_memset(&buf, '\0', sizeof(t_buffer));
	if (ft_strchr(remainder[fd], '\n'))
		return (get_remaining_line(remainder[fd]));
	ret = read(fd, buf.read, BUFFER_SIZE);
	last = &buf;
	last->nl = ft_strchr(last->read,'\n');
	while (ret > 0 && !last->nl)
	{
		ret = read(fd, buff_get_new(&buf)->read, BUFFER_SIZE);
		last = buff_get_last(&buf);
		last->nl = ft_strchr(last->read,'\n');
	}
	if (ret == -1 || (ret == 0 && !*last->read && !remainder[fd][0]))
		return (NULL);
	line = create_save_line(&buf, last, remainder[fd]);
	return (line);
}

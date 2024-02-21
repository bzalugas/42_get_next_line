/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/21 23:00:36 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_buffer	*buff_get_last(t_buffer *buf)
{
	t_buffer	*tmp;

	tmp = buf;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

static t_buffer	*buff_get_new(t_buffer *buf)
{
	t_buffer	*new;
	t_buffer	*last;
	char		*str;

	new = (t_buffer *)ft_calloc(1, sizeof(t_buffer));
	str = (char *)ft_calloc(BUFFER_SIZE + 1UL, sizeof(char));
	if (!new || !str)
		return (free(new), free(str), NULL);
	new->read = str;
	new->nl = NULL;
	new->next = NULL;
	if (!buf)
		return (new);
	last = buff_get_last(buf);
	last->next = new;
	new->n = last->n + 1;
	return (new);
}

static char	*get_remaining_line(char *remainder, t_buffer *buf)
{
	char		*line;
	char		*nl;
	size_t		len_nl;
	t_buffer	*tmp;

	if (!remainder || !remainder[0])
	{
		free(remainder);
		buf = buf->next;
		while (buf)
		{
			tmp = buf->next;
			free(buf->read);
			free(buf);
			buf = tmp;
		}
		return (NULL);
	}
	nl = buf->nl;
	line = (char *)ft_calloc((nl - remainder) + 2UL, sizeof(char));
	if (!line)
		return (NULL);
	ft_memmove(line, remainder, nl - remainder + 1UL);
	len_nl = ft_strlen(nl + 1UL);
	ft_memmove(remainder, nl + 1UL, len_nl);
	ft_memset(remainder + (size_t)len_nl, '\0', (size_t)BUFFER_SIZE - len_nl);
	return (line);
}

static char	*create_save_line(t_buffer *buf, t_buffer *last, char **remainder)
{
	size_t		len;
	size_t		len_remain;
	char		*line;
	t_buffer	*tmp;

	if (!*remainder && last->nl[0] == '\n')
		*remainder = (char *)ft_calloc(BUFFER_SIZE + 1UL, sizeof(char));
	if (!*remainder)
		return (get_remaining_line(NULL, buf));
	len_remain = ft_strlen(*remainder);
	len = len_remain + BUFFER_SIZE * last->n + (last->nl - last->read) + 1;
	line = (char *)ft_calloc(len + 1UL, sizeof(char));
	if (!line)
		return (get_remaining_line(NULL, buf));
	ft_memmove(line, *remainder, len_remain);
	ft_memset(*remainder, '\0', BUFFER_SIZE);
	while (buf)
	{
		ft_memmove(line + len_remain + buf->n * BUFFER_SIZE, buf->read,
			buf->nl - buf->read + 1UL);
		tmp = buf->next;
		if (buf->nl[0] == '\n')
			ft_memmove(*remainder, buf->nl + 1UL, ft_strlen(buf->nl + 1UL));
		free(buf->read);
		free(buf);
		buf = tmp;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*remainder[FD_MAX];
	t_buffer	*buf;
	t_buffer	*last;
	ssize_t		ret;
	int			found;

	buf = buff_get_new(NULL);
	if (fd < 0 || read(fd, buf->read, 0) < 0 || !buf)
		return (NULL);
	if (ft_find_nl(remainder[fd], &buf->nl) > 0)
		return (get_remaining_line(remainder[fd], buf));
	ret = read(fd, buf->read, BUFFER_SIZE);
	last = buf;
	found = ft_find_nl(last->read, &last->nl);
	while (ret == BUFFER_SIZE && !found)
	{
		ret = -1;
		last = buff_get_new(last);
		if (last)
			ret = read(fd, last->read, BUFFER_SIZE);
		found = ft_find_nl(last->read, &last->nl);
	}
	if (ret == -1 || (ret == 0 && last->n == 0 && !remainder[fd][0]))
		return (get_remaining_line(remainder[fd], buf));
	return (create_save_line(buf, last, &remainder[fd]));
}

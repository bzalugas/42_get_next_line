/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/21 16:14:31 by bazaluga         ###   ########.fr       */
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
	size_t		len_remain;
	char		*line;
	t_buffer	*tmp;

	len_remain = ft_strlen(remainder);
	len = len_remain + BUFFER_SIZE * last->n + (last->nl - last->read) + 1;
	line = (char *)ft_calloc(len + 1, sizeof(char));
	if (!line)
		return (NULL);
	ft_memmove(line, remainder, len_remain);
	ft_memset(remainder, '\0', BUFFER_SIZE);
	while (buf)
	{
		ft_memmove(line + len_remain + buf->n * BUFFER_SIZE, buf->read,
				   buf->nl - buf->read + 1);
		tmp = buf->next;
		if (buf->nl[0] == '\n')
			ft_memmove(remainder, buf->nl + 1, ft_strlen(buf->nl + 1));
		if (buf->n > 0)
			free(buf);
		buf = tmp;
	}
	return (line);
}

static char	*get_remaining_line(char *remainder, t_buffer *buf)
{
	char		*line;
	char		*nl;
	size_t		len_nl;
	t_buffer	*tmp;

	if (!remainder)
	{
		buf = buf->next;
		while (buf)
		{
			tmp = buf->next;
			free(buf);
			buf = tmp;
		}
		return (NULL);
	}
	nl = buf->nl;
	line = (char *)ft_calloc((nl - remainder) + 2, sizeof(char));
	if (!line)
		return (NULL);
	ft_memmove(line, remainder, nl - remainder + 1);
	len_nl = ft_strlen(nl + 1);
	ft_memmove(remainder, nl + 1, len_nl);
	ft_memset(remainder + len_nl, '\0', BUFFER_SIZE - len_nl);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	remainder[4][BUFFER_SIZE + 1L]; //Don't forget to change 4 by FD_MAX
	t_buffer	buf;
	t_buffer	*last;
	ssize_t		ret;
	int			found;

	if (fd < 0 || read(fd, buf.read, 0) < 0)
		return (NULL);
	ft_memset(&buf, '\0', sizeof(t_buffer));
	if (ft_find_nl(remainder[fd], &buf.nl))
		return (get_remaining_line(remainder[fd], &buf));
	ret = read(fd, buf.read, BUFFER_SIZE);
	last = &buf;
	found = ft_find_nl(last->read, &last->nl);
	while (ret == BUFFER_SIZE && !found)
	{
		//Invert 2 lines to get protection
		ret = read(fd, buff_get_new(&buf)->read, BUFFER_SIZE);
		last = buff_get_last(&buf);
		found = ft_find_nl(last->read, &last->nl);
	}
	if (ret == -1 || (ret == 0 && last->n == 0 && !remainder[fd][0]))
		return (get_remaining_line(NULL, &buf));
	return (create_save_line(&buf, last, remainder[fd]));
}

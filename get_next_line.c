/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/22 01:15:14 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	*clear_all(t_buffer *buf, char *remainder, char free_remain)
{
	t_buffer	*tmp;

	while (buf)
	{
		tmp = buf->next;
		free(buf->read);
		free(buf);
		buf = tmp;
	}
	if (free_remain && remainder)
		free(remainder);
	return (NULL);
}

static t_buffer	*buff_get_new(t_buffer *last)
{
	t_buffer	*new;
	char		*str;

	new = (t_buffer *)ft_calloc_gnl(NULL, 1, sizeof(t_buffer));
	str = (char *)ft_calloc_gnl(NULL, BUFFER_SIZE + 1UL, sizeof(char));
	if (!new || !str)
		return (free(new), free(str), NULL);
	new->read = str;
	new->nl = NULL;
	new->next = NULL;
	if (!last)
		return (new);
	last->next = new;
	new->n = last->n + 1;
	return (new);
}

static char	*get_remaining_line(char *remain, t_buffer *buf)
{
	char	*line;
	size_t	len_remain;
	size_t	len_nl;

	if (!remain || !*remain)
		return (clear_all(buf, remain, 1));
	len_remain = buf->nl - remain + 1;
	if (!ft_calloc_gnl(&line, len_remain + 1UL, sizeof(char)))
		return (clear_all(buf, remain, 1));
	ft_memmove(line, remain, len_remain);
	len_nl = ft_strlen(buf->nl + 1);
	ft_memmove(remain, buf->nl + 1UL, len_nl);
	ft_memset(remain + len_nl, '\0', BUFFER_SIZE - len_nl);
	clear_all(buf, remain, 0);
	return (line);
}

static char	*create_save_line(t_buffer *buf, t_buffer *last, char **mem)
{
	size_t		len_line;
	size_t		len_mem;
	char		*line;
	size_t		i;
	t_buffer	*tmp;

	len_mem = ft_strlen(*mem);
	len_line = len_mem + (last->n * BUFFER_SIZE) + (last->nl - last->read);
	if (!ft_calloc_gnl(&line, len_line + *last->nl == '\n' + 1, sizeof(char)))
		return (clear_all(buf, *mem, 1));
	ft_memmove(line, *mem, len_mem);
	ft_memset(*mem, '\0', BUFFER_SIZE);
	while (buf)
	{
		i = len_mem + (buf->n * BUFFER_SIZE);
		ft_memmove(line + i, buf->read, buf->nl - buf->read + (*buf->nl == '\n'));
		if (*buf->nl == '\n')
		{
			if (!*mem && !ft_calloc_gnl(mem, BUFFER_SIZE + 1UL, sizeof(char)))
				return (clear_all(buf, *mem, 0));
			ft_memmove(*mem, buf->nl + 1, ft_strlen(buf->nl + 1));
		}
		tmp = buf->next;
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

	if (fd < 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	buf = buff_get_new(NULL);
	if (!buf)
		return (NULL);
	if (ft_find_nl(remainder[fd], &buf->nl) > 0)
		return (get_remaining_line(remainder[fd], buf));
	ret = read(fd, buf->read, BUFFER_SIZE);
	last = buf;
	while (ret == BUFFER_SIZE && !ft_find_nl(last->read, &last->nl))
	{
		ret = -1;
		last->next = buff_get_new(last);
		last = last->next;
		if (last)
			ret = read(fd, last->read, BUFFER_SIZE);
	}
	if (ret == -1 || (ret == 0 && last->n == 0))
		return (clear_all(buf, remainder[fd], 1));
	return (create_save_line(buf, last, &remainder[fd]));
}

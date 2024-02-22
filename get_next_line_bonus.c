/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:06:51 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/22 17:22:38 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	*buff_clear(t_buffer *buf)
{
	t_buffer	*tmp;

	while (buf)
	{
		tmp = buf->next;
		free(buf->read);
		free(buf);
		buf = tmp;
	}
	return (NULL);
}

static t_buffer	*buff_get_new(t_buffer *last, char alloc_read)
{
	t_buffer	*new;
	char		*read;

	new = (t_buffer *)ft_calloc_gnl(NULL, 1, sizeof(t_buffer));
	read = NULL;
	if (alloc_read)
		read = (char *)ft_calloc_gnl(NULL, BUFFER_SIZE + 1UL, sizeof(char));
	if (!new || (alloc_read && !read))
		return (free(new), free(read), NULL);
	new->read = read;
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

	if (!*remain || !buf)
		return (buff_clear(buf));
	len_remain = (buf->nl - remain) + 1UL;
	if (!ft_calloc_gnl(&line, len_remain + 1UL, sizeof(char)))
		return (buff_clear(buf));
	ft_memmove(line, remain, len_remain);
	len_nl = ft_strlen(buf->nl + 1);
	ft_memmove(remain, buf->nl + 1UL, len_nl);
	ft_memset(remain + len_nl, '\0', BUFFER_SIZE - len_nl);
	buff_clear(buf);
	return (line);
}

static char	*create_save_line(t_buffer *buf, t_buffer *last, char *mem)
{
	size_t		len_l;
	size_t		len_mem;
	char		*l;
	size_t		i;
	t_buffer	*tmp;

	len_mem = ft_strlen(mem);
	len_l = len_mem + (last->n * BUFFER_SIZE) + (last->nl - last->read);
	if (!ft_calloc_gnl(&l, len_l + (*last->nl == '\n') + 1UL, sizeof(char)))
		return (buff_clear(buf));
	ft_memmove(l, mem, len_mem);
	ft_memset(mem, '\0', BUFFER_SIZE);
	while (buf)
	{
		i = len_mem + (buf->n * BUFFER_SIZE);
		ft_memmove(l + i, buf->read, buf->nl - buf->read + (*buf->nl == '\n'));
		if (*buf->nl == '\n')
			ft_memmove(mem, buf->nl + 1UL, ft_strlen(buf->nl + 1UL));
		tmp = buf->next;
		free(buf->read);
		free(buf);
		buf = tmp;
	}
	return (l);
}

char	*get_next_line(int fd)
{
	static char	remainder[FD_MAX][BUFFER_SIZE + 1UL];
	t_buffer	*buf;
	t_buffer	*last;
	ssize_t		ret;

	if (fd < 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	buf = buff_get_new(NULL, 0);
	if (ft_find_nl(remainder[fd], &buf->nl) > 0)
		return (get_remaining_line(remainder[fd], buf));
	if (!ft_calloc_gnl(&buf->read, BUFFER_SIZE + 1UL, sizeof(char)))
		return (NULL);
	ret = read(fd, buf->read, BUFFER_SIZE);
	last = buf;
	while (!ft_find_nl(last->read, &last->nl) && ret == BUFFER_SIZE)
	{
		ret = -1;
		last->next = buff_get_new(last, 1);
		last = last->next;
		if (last)
			ret = read(fd, last->read, BUFFER_SIZE);
	}
	if (ret == -1 || (ret == 0 && !*buf->read && !*remainder[fd]))
		return (buff_clear(buf));
	return (create_save_line(buf, last, remainder[fd]));
}

/*#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
int main()
{
	char *l;
	char *l2;

	int fd = open("text.txt", O_RDONLY);
	int fd2 = open("text2.txt", O_RDONLY);
	l = get_next_line(fd);
	l2 = get_next_line(fd2);
	while (l || l2)
	{
		printf("file1: %s\n", l);
		printf("file2: %s\n", l2);
		free(l);
		free(l2);
		l = get_next_line(fd);
		l2 = get_next_line(fd2);
	}
	close(fd);
	close(fd2);
	return 0;
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 19:06:48 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/21 19:07:19 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	*ft_memset(void	*s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

int	ft_find_nl(const char *s, char **nl)
{
	size_t	i;
	int		c;

	if (!s)
		return (-1);
	c = '\n';
	i = 0;
	while (s[i])
	{
		if ((unsigned char)c == (unsigned char)s[i])
			return (*nl = (char *)&s[i], 1);
		i++;
	}
	*nl = (char *)&s[i];
	if ((unsigned char)c == (unsigned char)s[i])
		return (1);
	return (0);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			i;

	if (size > 0 && nmemb > ULONG_MAX / size)
		return (NULL);
	ptr = (unsigned char *)malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < nmemb * size)
	{
		ptr[i] = '\0';
		i++;
	}
	return ((void *)ptr);
}

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (src < dst)
	{
		i = n;
		while (i > 0)
		{
			*(char *)(dst + (i - 1)) = *(char *)(src + (i - 1));
			i--;
		}
	}
	else
	{
		i = 0;
		while (i < n)
		{
			*(char *)(dst + i) = *(char *)(src + i);
			i++;
		}
	}
	return (dst);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

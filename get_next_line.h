/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:39 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/19 20:26:21 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# include <unistd.h>

typedef struct s_buffer
{
	char			read[BUFFER_SIZE];
	int				fd;
	struct s_buffer	*next;
}				t_buffer;

char	*get_next_line(int fd);

#endif

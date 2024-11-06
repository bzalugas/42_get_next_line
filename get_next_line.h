/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 10:21:29 by bazaluga          #+#    #+#             */
/*   Updated: 2024/11/06 10:24:12 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif
/* # define STASH_SIZE BUFFER_SIZE + 1L */
# define STASH_SIZE 5000000
# include <stdlib.h>

char	*get_next_line(int fd);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getnextline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 22:24:52 by mhuszar           #+#    #+#             */
/*   Updated: 2025/04/12 15:25:48 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hotrace.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

#define EXPECTED_LINE_SIZE 64

static inline void	__attribute__((always_inline))
	free_and_null(char **ptr)
{
	if (!ptr)
		return ;
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static inline size_t	__attribute__((always_inline))
	ft_strlcpy(char *dst, char *src, size_t size)
{
	size_t	counter;

	counter = 0;
	if (size <= 0)
		return (1);
	while (counter < size)
	{
		dst[counter] = src[counter];
		counter++;
	}
	return (0);
}

static inline char	__attribute__((always_inline))
	*resize_vec(char *line, int *len)
{
	static int	vec_size = EXPECTED_LINE_SIZE;
	char		*realloc_line;

	if (*len == vec_size)
		vec_size <<= 1;
	else if (line)
		return (line);
	realloc_line = malloc(vec_size);
	if (!realloc_line)
	{
		free_and_null(&line);
		*len = -1;
		return (NULL);
	}
	if (line)
	{
		ft_strlcpy(realloc_line, line, *len);
		free(line);
	}
	return (realloc_line);
}

static inline int	__attribute__((always_inline))
	read_into_buf(char *buf, int fd, int *buf_len, int *buf_idx)
{
	if (*buf_idx >= *buf_len)
	{
		*buf_len = read(fd, buf, BUFFER_SIZE);
		*buf_idx = 0;
	}
	return (*buf_len);
}

t_line	get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	static int	buf_len = 0;
	static int	buf_idx = 0;
	t_line		line;
	int			len;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (line.raw = NULL, line);
	line.raw = NULL;
	len = 0;
	while (1)
	{
		if (read_into_buf(buffer, fd, &buf_len, &buf_idx) <= 0)
			break ;
		line.raw = resize_vec(line.raw, &len);
		if (len == -1)
			return (line.raw = NULL, line);
		line.raw[len++] = buffer[buf_idx++];
		if (line.raw[len - 1] == '\n')
			return (line.raw[len - 1] = '\0', line.size = len - 1, line);
	}
	if (!len || !line.raw || buf_len < 0)
		return (free_and_null(&line.raw), line);
	line.raw[len] = '\0';
	return (line.size = len, line);
}

// int	main(void)
// {
//     int		fd;
//     t_line	line;

//     fd = open("so_long.txt", O_RDONLY);
//     if (fd < 0)
//         return (1);

// 	line = get_next_line(fd);
//     while (line.raw)
//     {
//         printf("%s", line.raw);
//         free(line.raw);
// 		line = get_next_line(fd);
//     }

//     close(fd);
//     return (0);
// }

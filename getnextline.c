/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getnextline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 22:24:52 by mhuszar           #+#    #+#             */
/*   Updated: 2025/04/13 19:55:26 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hotrace.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 8192
#endif

#define EXPECTED_LINE_SIZE 5

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

	if (*len >= vec_size)
	{
		if (*len > vec_size << 1)
			vec_size = *len + 1;
		else
			vec_size <<= 1;
	}
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

static void	__attribute__((noinline)) __attribute__((hot))
	move(char *dest, char *src, size_t bytes)
{
	__asm__ volatile (
		"cld; rep movsb"
		:
		: "S"(src), "D"(dest), "c"(bytes)
		: "memory", "cc", "flags"
	);
}

t_line	get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	static int	buf_len = 0;
	static int	buf_idx = 0;
	t_gnl		s;

	block_memset((uint64_t *)&s, 0, 3);
	while (1)
	{
		if (read_into_buf(buffer, fd, &buf_len, &buf_idx) <= 0)
			break ;
		s.copy = 0;
		while (buf_idx + s.copy < buf_len && buffer[buf_idx + s.copy] != '\n')
			s.copy++;
		s.len += s.copy;
		s.line.raw = resize_vec(s.line.raw, &s.len);
		if (s.len == -1)
			return (s.line.raw = NULL, s.line);
		move(&s.line.raw[s.len - s.copy], &buffer[buf_idx], s.copy);
		buf_idx += s.copy;
		if (buf_idx < buf_len && buffer[buf_idx] == '\n')
            return (buf_idx++, s.line.raw[s.len] = '\0', s.line.size = s.len, s.line);
	}
	if (!s.len || !s.line.raw || buf_len < 0)
		return (free_and_null(&s.line.raw), s.line);
	return (s.line.raw[s.len] = '\0', s.line.size = s.len, s.line);
}

// int	main(void)
// {
//     int		fd;
//     t_line	line;

//     fd = open("input.txt", O_RDONLY);
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

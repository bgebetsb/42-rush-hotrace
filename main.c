/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:34:57 by mhuszar           #+#    #+#             */
/*   Updated: 2025/04/12 19:00:38 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define PRINTBUF_SIZE 2048

static bool	parse_key_value_pairs(t_hashmap *hashmap)
{
	t_line	key;
	t_line	value;

	while (true)
	{
		key = get_next_line(0);
		if (!key.raw)
			return (false);
		if (!key.size) 
			return (true);
		value = get_next_line(0);
		if (!value.raw)
			return (free(key.raw), false);
		if (!value.size)
			return (free(key.raw), true);
		if (!hashmap_insert(hashmap, key, value))
			return (free(key.raw), free(value.raw), false);
	}
}

static inline void __attribute__ ((always_inline)) __attribute__ ((hot))
	move(uint8_t *src, uint8_t *dest, size_t bytes)
{
	__asm__ (
		"rep movsb"
		:
		: "S"(src), "D"(dest), "c"(bytes)
		: "memory");
}

static inline void __attribute__ ((always_inline))
	print_buf(t_line str, bool found)
{
	char buf[PRINTBUF_SIZE];

	if (__builtin_expect(found && str.size < PRINTBUF_SIZE, 1))
	{
		move((uint8_t *)str.raw, (uint8_t *)buf, str.size);
		buf[str.size] = '\n';
		write(1, buf, str.size + 1);
	}
	else if (found)
	{
		write(1, str.raw, str.size);
		write(1, "\n", 1);
	}
	else if (str.size < PRINTBUF_SIZE - 12)
	{
		move((uint8_t *)str.raw, (uint8_t *)buf, str.size);
		move((uint8_t *)": not found\n", (uint8_t *)buf + str.size, 13);
		write(1, buf, str.size + 12);
	}
	else
	{
		write(1, str.raw, str.size);
		write(1, ": not found\n", 1);
	}
}

static void parse_searches(t_hashmap *hashmap)
{
	t_line	line;
	t_line	result;
	// char	buffer[100];
	// int		ret;

	while (true)
	{
		line = get_next_line(0);
		if (!line.raw)
			return;
		if (!line.size)
		{
			free(line.raw);
			continue;
		}
		result = hashmap_get_value(hashmap, line);
		if (!result.raw)
			print_buf(line, false);//ret = snprintf(buffer, 100, "%s: not found\n", line.raw);
			// printf("%s: not found\n", line.raw);
		else
			print_buf(result, true);// printf("%s: %s\n", line.raw, result);
			//ret = snprintf(buffer, 100, "%s: %s\n", line.raw, result);
		// write(1, buffer, ret);
	}
}

int	main(void)
{
	t_hashmap	*hashmap;

	hashmap = malloc(HASHMAP_SIZE * sizeof(t_hashmap));
	if (!hashmap)
		return (1);
	block_memset((u_int64_t *)hashmap, 0, HASHMAP_SIZE << 1);
	if (!parse_key_value_pairs(hashmap))
		return (1);
	parse_searches(hashmap);
	// TODO: free hashmap
}

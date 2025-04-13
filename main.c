/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:34:57 by mhuszar           #+#    #+#             */
/*   Updated: 2025/04/13 22:13:51 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static bool	parse_key_value_pairs(t_hashmap *hashmap)
{
	t_line	key;
	t_line	value;

	while (true)
	{
		key = get_next_line(0);
		if (!key.raw)
			return (ft_putstr("Unexpected EOF"), false);
		if (!key.size)
			return (free(key.raw), true);
		value = get_next_line(0);
		if (!value.raw)
			return (free(key.raw), ft_putstr("Unexpected EOF"), false);
		if (!value.size)
			return (free(key.raw),
				free(value.raw), ft_putstr("Unexpected empty line"), true);
		if (!hashmap_insert(hashmap, key, value))
			return (free(key.raw), free(value.raw), false);
	}
}

/*this should not cause an issue, as src pointer is always a copy of the
one stored in the tree here*/
static inline void	__attribute__((always_inline)) __attribute__((hot))
	move(uint8_t *src, uint8_t *dest, size_t bytes)
{
	__asm__ volatile (
		"cld; rep movsb"
		:
		: "S"(src), "D"(dest), "c"(bytes)
		: "memory", "cc", "flags"
	);
}

static inline void	__attribute__((always_inline)) __attribute__((hot))

	print_buf(t_line str, bool found)
{
	char	buf[PRINTBUF_SIZE];

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
	else if (str.size < PRINTBUF_SIZE - 13)
	{
		move((uint8_t *)str.raw, (uint8_t *)buf, str.size);
		move((uint8_t *)": Not found.\n", (uint8_t *)buf + str.size, 14);
		write(1, buf, str.size + 13);
	}
	else
	{
		write(1, str.raw, str.size);
		write(1, ": Not found.\n", 13);
	}
}

static void	parse_searches(t_hashmap *hashmap)
{
	t_line	line;
	t_line	result;

	while (true)
	{
		line = get_next_line(0);
		if (!line.raw)
			return ;
		if (!line.size)
		{
			free(line.raw);
			continue ;
		}
		result = hashmap_get_value(hashmap, line);
		if (!result.raw)
			print_buf(line, false);
		else
			print_buf(result, true);
		free(line.raw);
	}
}

int	main(void)
{
	t_hashmap	*hashmap;

	hashmap = malloc(HASHMAP_SIZE * sizeof(t_hashmap));
	if (!hashmap)
		return (ft_putstr(strerror(errno)), 1);
	block_memset((u_int64_t *)hashmap, 0, HASHMAP_SIZE << 1);
	if (!parse_key_value_pairs(hashmap))
		return (free_hashmap(&hashmap), 1);
	parse_searches(hashmap);
	free_hashmap(&hashmap);
}

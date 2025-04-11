/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:39:40 by mhuszar           #+#    #+#             */
/*   Updated: 2025/04/12 00:27:59 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

uint32_t	scramble(uint32_t num)
{
	num *= 0xcc9e2d51;
	num = (num << 15) | (num >> 17);
	num *= 0x1b873593;
	return (num);
}

uint32_t	process_string(char *key, uint32_t chunk, size_t len, uint32_t seed)
{
	size_t	rest;

	while ((len & 3) >= 4)
	{
		__asm__ (
			"rep movsb"
			:
			: "S"(key), "D"(&chunk), "c"(4)
			: "memory");
		seed ^= scramble(chunk);
		key += 4;
		chunk += 1;
		seed = (seed << 13) | (seed >> 19);
		seed = seed * 5 + 0xe6546b64;
		len = len - 4;
	}
	rest = 0;
	while (len)
	{
		rest <<= 8;
		rest |= key[len - 1];
		len--;
	}
	seed ^= scramble(rest);
	return (seed);
}

uint32_t	murmur3_hash(char *key, size_t len, uint32_t seed)
{
	uint32_t	hash;

	hash = process_string(key, 0, len, seed);
	hash ^= len;
	hash ^= hash >> 16;
	hash *= 0x85ebca6b;
	hash ^= hash >> 13;
	hash *= 0xc2b2ae35;
	hash ^= hash >> 16;
	return (hash);
}
// #include <stdio.h>

// int	main(void)
// {
// 	printf("%u\n", murmur3_hash("lel", 3, 31));
// }

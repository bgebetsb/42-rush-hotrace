/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 21:59:13 by mhuszar           #+#    #+#             */
/*   Updated: 2025/04/13 21:59:16 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint32_t	fnv1a_hash(const char *s)
{
	uint32_t	hash;

	hash = 2166136261u;
	while (*s)
	{
		hash ^= (uint8_t)(*s++);
		hash *= 16777619u;
	}
	return (hash);
}

uint32_t	djb2a_hash(const char *s)
{
	uint32_t		hash;
	unsigned char	c;

	hash = 5381;
	while (*s)
	{
		c = *s++;
		hash = ((hash << 5) + hash) ^ c;
	}
	return (hash);
}

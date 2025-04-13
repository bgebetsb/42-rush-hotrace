/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:42:27 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/13 12:58:43 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

void	ft_bzero(void *s, size_t n)
{
	size_t	*cur;
	char	*cur_char;

	cur = (size_t *)s;
	while (n > 7)
	{
		*cur = 0;
		++cur;
		n -= 8;
	}
	cur_char = (char *)cur;
	while (n--)
	{
		*cur_char = 0;
		++cur_char;
	}
}

void	__attribute__ ((noinline))
	*block_memset(uint64_t *s, size_t c, size_t n)
{
    __asm__ volatile (
        "cld; rep stosq"
        :
        : "c"(n), "D"(s), "a"(c)
        : "memory", "cc", "flags"
    );
    return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*mem;
	size_t	total_size;

	if (nmemb == 0 || size == 0)
	{
		return (malloc(0));
	}
	total_size = nmemb * size;
	if (total_size / size != nmemb)
	{
		return (NULL);
	}
	mem = malloc(total_size);
	if (mem == NULL)
	{
		return (NULL);
	}
	ft_bzero(mem, total_size);
	return (mem);
}

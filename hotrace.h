/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgebetsb <bgebetsb@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:42:19 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/11 21:42:20 by bgebetsb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOTRACE_H
# define HOTRACE_H

# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

# define HASHMAP_SIZE 65536

typedef struct line
{
	char	*raw;
	size_t	size;
}	t_line;

typedef struct s_list
{
	size_t		main_hash;
	size_t		collision_hash;
	t_line		key;
	t_line		value;
	void		*next;
}	t_list;

typedef struct s_hashmap
{
	t_list	*matches;
	size_t	amount;
}	t_hashmap;

void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
void		*block_memset(uint64_t *s, size_t c, size_t n);

uint32_t 	fnv1a_hash(const char* s);
uint32_t	djb2a_hash(const char *s);
uint32_t	murmur3_hash(char *key, size_t len, uint32_t seed);

t_hashmap	*create_hashmap(void);
char		*hashmap_get_value(t_hashmap *hashmap, t_line key);
bool		hashmap_insert(t_hashmap *hashmap, t_line key, t_line value);

t_line		get_next_line(int fd);

#endif

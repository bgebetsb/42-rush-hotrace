/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotrace.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:42:19 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/13 21:55:05 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOTRACE_H
# define HOTRACE_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

# define HASHMAP_SIZE 1048576

typedef struct s_line
{
	char			*raw;
	size_t			size;
}					t_line;

typedef enum e_directions
{
	LEFT = 0,
	RIGHT = 1,
}					t_directions;

typedef struct s_tree
{
	size_t			main_hash;
	t_line			key;
	t_line			value;
	struct s_tree	*parent;
	void			*left;
	void			*right;
}					t_tree;

typedef struct s_hashmap
{
	t_tree			*matches;
	size_t			amount;
}					t_hashmap;

typedef struct gnl
{
	t_line			line;
	int				len;
	int				copy;
}					t_gnl;

void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
void				*block_memset(uint64_t *s, size_t c, size_t n);
void				gnl_move(char *dest, const char *src, size_t bytes);

uint32_t			fnv1a_hash(const char *s);
// uint32_t	djb2a_hash(const char *s);
uint32_t			murmur3_hash(char *key, size_t len, uint32_t seed);

t_hashmap			*create_hashmap(void);
t_line				hashmap_get_value(t_hashmap *hashmap, t_line key);
bool				hashmap_insert(t_hashmap *hashmap, t_line key,
						t_line value);
void				free_hashmap(t_hashmap **hashmap);

t_line				get_next_line(int fd);

#endif

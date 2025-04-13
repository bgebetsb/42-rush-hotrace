/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:42:02 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/13 21:57:33 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdint.h>
#include <stdlib.h>

static uint32_t			djb2a_hash(const char *s);
static t_line			tree_lookup_hash(t_tree *list, t_line key, size_t hash);

t_line	hashmap_get_value(t_hashmap *hashmap, t_line key)
{
	size_t			main_hash;
	const t_hashmap	*cur;
	t_line			ret;

	main_hash = djb2a_hash(key.raw);
	cur = hashmap + main_hash % HASHMAP_SIZE;
	if (!cur->amount)
		return (ret.raw = NULL, ret.size = 0, ret);
	return (tree_lookup_hash(cur->matches, key, main_hash));
}

static inline uint32_t	__attribute__((always_inline))
	djb2a_hash(const char *s)
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

static inline int	__attribute__((always_inline))
	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

static inline t_line	__attribute__((always_inline))
	tree_lookup_hash(t_tree *tree, t_line key, size_t hash)
{
	t_tree	*cur;
	t_line	ret;
	int		diff;

	cur = tree;
	while (cur)
	{
		if (cur->main_hash == hash)
		{
			diff = ft_strcmp(key.raw, cur->key.raw);
			if (diff == 0)
				return (cur->value);
			else if (diff < 0)
				cur = cur->left;
			else
				cur = cur->right;
		}
		else if (hash < cur->main_hash)
			cur = cur->left;
		else
			cur = cur->right;
	}
	return (ret.raw = NULL, ret.size = 0, ret);
}

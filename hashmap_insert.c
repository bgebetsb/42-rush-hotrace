/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_insert.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgebetsb <bgebetsb@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:38:51 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/13 21:25:38 by bgebetsb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

static void		insert_in_tree(t_tree **start, t_tree *node);
static int	compare_nodes(t_tree *a, t_tree *b);

static inline uint32_t	__attribute__((always_inline)) djb2a_hash(const char *s)
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

bool	hashmap_insert(t_hashmap *hashmap, t_line key, t_line value)
{
	size_t	main_hash;
	t_tree	*hashmap_node = NULL;
	t_hashmap	*item;

	hashmap_node = malloc(sizeof(t_tree));
	if (!hashmap_node)
		return (false);
	block_memset((uint64_t *)hashmap_node, 0, 9);
	main_hash = djb2a_hash(key.raw);
	hashmap_node->main_hash = main_hash;
	hashmap_node->key = key;
	hashmap_node->value = value;
	item = hashmap + (main_hash % HASHMAP_SIZE);
	item->amount++;
	insert_in_tree(&item->matches, hashmap_node);
	return (true);
}

static void	insert_in_tree(t_tree **start, t_tree *node)
{
	t_tree			**cur;
	t_tree			*parent;
	int				cmp_ret;

	cur = start;
	parent = NULL;
	while (*cur)
	{
		cmp_ret = compare_nodes(node, *cur);
		if (cmp_ret == 0)
		{
    		(free((*cur)->key.raw), free((*cur)->value.raw), free(*cur));
    		break;
		}
		parent = *cur;
		if (cmp_ret < 0)
			cur = (t_tree **)&((*cur)->left);
		else
			cur = (t_tree **)&((*cur)->right);
	}
	node->parent = parent;
	*cur = node;
}

static inline int __attribute__((always_inline)) ft_strcmp(const char *s1, const char *s2)
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

static inline int __attribute__((always_inline)) compare_nodes(t_tree *a, t_tree *b)
{
	if (a->main_hash < b->main_hash)
		return (-1);
	else if (a->main_hash > b->main_hash)
		return (1);
	return (ft_strcmp(a->key.raw, b->key.raw));
}

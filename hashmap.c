/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgebetsb <bgebetsb@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:42:02 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/11 21:45:43 by bgebetsb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static void	insert_at_pos(t_hashmap *hashmap, t_tree *node, size_t pos);
static char	*tree_lookup_hash(t_tree *list, const char *key, size_t hash);
static int	compare_nodes(t_tree *a, t_tree *b);
static void	insert_in_tree(t_tree **start, t_tree *node);

bool	hashmap_insert(t_hashmap *hashmap, char *key, char *value)
{
	size_t	main_hash;
	t_tree	*hashmap_node;

	main_hash = fnv1a_hash(key);
	hashmap_node = ft_calloc(1, sizeof(t_tree));
	if (!hashmap_node)
		return (false);
	hashmap_node->main_hash = main_hash;
	hashmap_node->key = key;
	hashmap_node->value = value;
	hashmap_node->collision_hash = SIZE_MAX;
	insert_at_pos(hashmap, hashmap_node, main_hash % HASHMAP_SIZE);
	return (true);
}

char	*hashmap_get_value(t_hashmap *hashmap, const char *key)
{
	size_t			main_hash;
	const t_hashmap	*cur;

	main_hash = fnv1a_hash(key);
	cur = hashmap + main_hash % HASHMAP_SIZE;
	if (!cur->amount)
		return (NULL);
	return (tree_lookup_hash(cur->matches, key, main_hash));
}

static void	insert_at_pos(t_hashmap *hashmap, t_tree *node, size_t pos)
{
	t_hashmap	*item;

	item = hashmap + pos;
	item->amount++;
	insert_in_tree(&item->matches, node);
}

static void	insert_in_tree(t_tree **start, t_tree *node)
{
	t_tree	**cur;
	int		cmp_ret;

	cur = start;
	while (*cur)
	{
		cmp_ret = compare_nodes(node, *cur);
		if (cmp_ret == 0)
			break;
		else if (cmp_ret == -1)
			cur = (t_tree **)&((*cur)->left);
		else
			cur = (t_tree **)&((*cur)->right);
	}
	*cur = node;
}

#include <unistd.h>

// TODO: Could be optimized in Assembly so it uses only 1 `cmp` instruction
inline static int __attribute__ ((always_inline))
cmp(uint32_t a, uint32_t b)
{
	if (a < b)
		return (-1);
	if (a > b)
		return (1);
	return (0);
}

static int	compare_nodes(t_tree *a, t_tree *b)
{
	int	main_cmp;

	main_cmp = cmp(a->main_hash, b->main_hash);
	if (main_cmp != 0)
		return (main_cmp);
	if (a->collision_hash == SIZE_MAX)
		a->collision_hash = djb2a_hash(a->key);
	if (b->collision_hash == SIZE_MAX)
		b->collision_hash = djb2a_hash(b->key);
	return (cmp(a->collision_hash, b->collision_hash));
}

static char	*tree_lookup_hash(t_tree *tree, const char *key, size_t hash)
{
	t_tree	*cur;
	size_t	collision_hash;
	int		cmp_ret;

	cur = tree;
	collision_hash = SIZE_MAX;
	while (cur)
	{
		if (cur->main_hash == hash)
		{
			if (collision_hash == SIZE_MAX)
				collision_hash = djb2a_hash(key);
			if (cur->collision_hash == SIZE_MAX)
				cur->collision_hash = djb2a_hash(cur->key);
			if (collision_hash == cur->collision_hash)
				return (cur->value);
			else if (collision_hash < cur->collision_hash)
				cur = cur->left;
			else
				cur = cur->right;
		}
		else if (hash < cur->main_hash)
			cur = cur->left;
		else
			cur = cur->right;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:42:02 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/12 18:56:16 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

static void		insert_at_pos(t_hashmap *hashmap, t_tree *node, size_t pos);
static t_line	tree_lookup_hash(t_tree *list, t_line key, size_t hash);
static int		compare_nodes(t_tree *a, t_tree *b);
static void		insert_in_tree(t_tree **start, t_tree *node);

bool	hashmap_insert(t_hashmap *hashmap, t_line key, t_line value)
{
	size_t	main_hash;
	t_tree	*hashmap_node;

	main_hash = murmur3_hash(key.raw, key.size, 31);
	hashmap_node = ft_calloc(1, sizeof(t_tree));
	if (!hashmap_node)
		return (false);
	hashmap_node->main_hash = main_hash;
	hashmap_node->key = key;
	hashmap_node->value = value;
	insert_at_pos(hashmap, hashmap_node, main_hash % HASHMAP_SIZE);
	return (true);
}

t_line	hashmap_get_value(t_hashmap *hashmap, t_line key)
{
	size_t			main_hash;
	const t_hashmap	*cur;
	t_line			ret;

	// main_hash = fnv1a_hash(key.raw);
	main_hash = murmur3_hash(key.raw, key.size, 31);
	cur = hashmap + main_hash % HASHMAP_SIZE;
	if (!cur->amount)
		return (ret.raw = NULL, ret.size = 0, ret);
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
	t_tree			**cur;
	t_tree			*parent;
	int				cmp_ret;
	t_directions	direction;

	cur = start;
	parent = NULL;
	direction = LEFT;
	while (*cur)
	{
		cmp_ret = compare_nodes(node, *cur);
		if (cmp_ret == 0)
			break;
		parent = *cur;
		if (cmp_ret < 0)
		{
			cur = (t_tree **)&((*cur)->left);
			direction = LEFT;
		}
		else
		{
			cur = (t_tree **)&((*cur)->right);
			direction = RIGHT;
		}
	}
	node->parent = parent;
	node->parent_direction = direction;
	*cur = node;
}


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

#include <string.h>

static int	compare_nodes(t_tree *a, t_tree *b)
{
	int	main_cmp;

	main_cmp = cmp(a->main_hash, b->main_hash);
	if (main_cmp != 0)
		return (main_cmp);
	return (strcmp(a->key.raw, b->key.raw));
}

static t_line	tree_lookup_hash(t_tree *tree, t_line key, size_t hash)
{
	t_tree	*cur;
	t_line	ret;

	cur = tree;
	while (cur)
	{
		if (cur->main_hash == hash)
		{
			int diff = strcmp(key.raw, cur->key.raw);
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

#include <stdio.h>
void	free_tree(t_tree **start)
{
	t_tree	*cur;
	t_tree	*tmp;

	cur = *start;
	while (cur)
	{
		if (cur->left)
			cur = cur->left;
		else if (cur->right)
			cur = cur->right;
		else
		{
			if (cur->parent)
			{
				if (cur->parent_direction == LEFT)
					cur->parent->left = NULL;
				else
					cur->parent->right = NULL;
			}
			tmp = cur->parent;
			free(cur->key.raw);
			free(cur->value.raw);
			free(cur);
			cur = tmp;
		}
	}
	*start = NULL;
}

void	free_hashmap(t_hashmap **hashmap)
{
	size_t	i;
	t_tree	*tree;

	i = 0;
	while (i < HASHMAP_SIZE)
	{
		tree = (*hashmap)[i].matches;
		if (tree && (*hashmap)[i].amount)
			free_tree(&tree);
		i++;
	}
	free(*hashmap);
	*hashmap = NULL;
}

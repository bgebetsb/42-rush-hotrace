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
#include <stdbool.h>
#include <stdint.h>

static void	insert_at_pos(t_hashmap *hashmap, t_list *node, size_t pos);
t_line		list_lookup_hash(t_list *list, t_line key, size_t hash);

bool	hashmap_insert(t_hashmap *hashmap, t_line key, t_line value)
{
	size_t	main_hash;
	t_list	*hashmap_node;

	main_hash = murmur3_hash(key.raw, key.size, 31);
	hashmap_node = ft_calloc(1, sizeof(t_list));
	if (!hashmap_node)
		return (false);
	hashmap_node->main_hash = main_hash;
	hashmap_node->key = key;
	hashmap_node->value = value;
	hashmap_node->collision_hash = SIZE_MAX;
	insert_at_pos(hashmap, hashmap_node, main_hash & (HASHMAP_SIZE - 1));
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
	return (list_lookup_hash(cur->matches, key, main_hash));
}

static void	insert_at_pos(t_hashmap *hashmap, t_list *node, size_t pos)
{
	t_hashmap	*item;

	item = hashmap + pos;
	item->amount++;
	if (!item->matches)
	{
		item->matches = node;
	}
	else
	{
		node->next = item->matches;
		item->matches = node;
	}
}

t_line	list_lookup_hash(t_list *list, t_line key, size_t hash)
{
	t_list	*cur;
	size_t	collision_hash;
	t_line	ret;

	cur = list;
	collision_hash = SIZE_MAX;
	while (cur)
	{
		if (cur->main_hash == hash)
		{
			if (collision_hash == SIZE_MAX)
				collision_hash = djb2a_hash(key.raw);
			if (cur->collision_hash == SIZE_MAX)
				cur->collision_hash = djb2a_hash(cur->key.raw);
			if (collision_hash == cur->collision_hash)
				return (cur->value);
		}
		cur = cur->next;
	}
	return (ret.raw = NULL, ret.size = 0, ret);
}

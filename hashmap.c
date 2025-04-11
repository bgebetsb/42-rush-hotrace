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

static void	insert_at_pos(t_hashmap *hashmap, t_list *node, size_t pos);
static char	*list_lookup_hash(const t_list *list, const char *key, size_t hash);

bool	hashmap_insert(t_hashmap *hashmap, char *key, char *value)
{
	size_t	main_hash;
	t_list	*hashmap_node;

	main_hash = fnv1a_hash(key);
	hashmap_node = ft_calloc(1, sizeof(t_list));
	if (!hashmap_node)
		return (false);
	hashmap_node->main_hash = main_hash;
	hashmap_node->key = key;
	hashmap_node->value = value;
	hashmap_node->collision_hash = djb2a_hash(key);
	insert_at_pos(hashmap, hashmap_node, main_hash % HASHMAP_SIZE);
	return (true);
}

char	*hashmap_get_value(const t_hashmap *hashmap, const char *key)
{
	size_t			main_hash;
	const t_hashmap	*cur;

	main_hash = fnv1a_hash(key);
	cur = hashmap + main_hash % HASHMAP_SIZE;
	if (!cur->amount)
		return (NULL);
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

static char	*list_lookup_hash(const t_list *list, const char *key, size_t hash)
{
	const t_list	*cur;
	size_t			collision_hash;

	cur = list;
	collision_hash = SIZE_MAX;
	while (cur)
	{
		if (cur->main_hash == hash)
		{
			if (collision_hash == SIZE_MAX && cur->collision_hash != SIZE_MAX)
				collision_hash = djb2a_hash(key);
			if (collision_hash == cur->collision_hash)
				return (cur->value);
		}
		cur = cur->next;
	}
	return (NULL);
}

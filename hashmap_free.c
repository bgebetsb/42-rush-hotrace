/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgebetsb <bgebetsb@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:46:18 by bgebetsb          #+#    #+#             */
/*   Updated: 2025/04/13 21:59:34 by bgebetsb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <stdlib.h>

static inline void	__attribute__((always_inline)) free_tree(t_tree **start)
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
				if (cur == cur->parent->left)
					cur->parent->left = NULL;
				else
					cur->parent->right = NULL;
			}
			tmp = cur->parent;
			(free(cur->key.raw), free(cur->value.raw));
			free(cur);
			cur = tmp;
		}
	}
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

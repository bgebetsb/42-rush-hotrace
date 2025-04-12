#include "hotrace.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static bool	parse_key_value_pairs(t_hashmap *hashmap)
{
	t_line	key;
	t_line	value;

	while (true)
	{
		key = get_next_line(0);
		if (!key.raw)
			return (false);
		if (!key.size) 
			return (true);
		value = get_next_line(0);
		if (!value.raw)
			return (free(key.raw), false);
		if (!value.size)
			return (free(key.raw), true);
		if (!hashmap_insert(hashmap, key.raw, value.raw))
			return (free(key.raw), free(value.raw), false);
	}
}

static void parse_searches(t_hashmap *hashmap)
{
	t_line	line;
	char	*result;

	while (true)
	{
		line = get_next_line(0);
		if (!line.raw)
			return;
		if (!line.size)
		{
			free(line.raw);
			continue;
		}
		result = hashmap_get_value(hashmap, line.raw);
		if (!result)
			printf("%s: not found\n", line.raw);
		else
			printf("%s: %s\n", line.raw, result);
	}
}

int	main(void)
{
	t_hashmap	*hashmap;

	hashmap = malloc(HASHMAP_SIZE * sizeof(t_hashmap));
	if (!hashmap)
		return (1);
	block_memset((u_int64_t *)hashmap, 0, HASHMAP_SIZE << 1);
	if (!parse_key_value_pairs(hashmap))
		return (1);
	parse_searches(hashmap);
	// TODO: free hashmap
}

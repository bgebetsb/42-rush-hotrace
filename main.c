#include "hotrace.h"
#include <stdio.h>

int	main(void)
{
	t_hashmap	*hashmap;

	hashmap = ft_calloc(HASHMAP_SIZE, sizeof(t_hashmap));
	if (!hashmap)
		return (1);
	if (!hashmap_insert(hashmap, "costarring", "test"))
		printf("Unable to insert in hashmap\n");
	printf("%s\n", hashmap_get_value(hashmap, "costarring"));
	printf("%s\n", hashmap_get_value(hashmap, "liquid"));
}

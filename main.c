#include "hotrace.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int	main(void)
{
	t_hashmap	*hashmap;

	hashmap = malloc(HASHMAP_SIZE * sizeof(t_hashmap));
	if (!hashmap)
		return (1);
	block_memset((u_int64_t *)hashmap, 0, HASHMAP_SIZE << 1);
	if (!hashmap_insert(hashmap, "costarring", "test"))
		printf("Unable to insert in hashmap\n");
	printf("%s\n", hashmap_get_value(hashmap, "costarring"));
	printf("%s\n", hashmap_get_value(hashmap, "liquid"));
}

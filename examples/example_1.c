
#include <stdio.h>
#include "zvec.h"
#include "zstr.h"
#include "zmap.h"

DEFINE_VEC_TYPE(int, Int)
DEFINE_MAP_TYPE(int, const char*, StrMap)

#define Z_SHORT_OPS
#include "zops.h"

uint32_t int_hash(int k, uint32_t seed) 
{
    return zmap_default_hash(&k, sizeof(k), seed);
}

int int_cmp(int a, int b) 
{
    return (a > b) - (a < b);
}

int main(void) 
{
    vec_Int nums = vec_init(Int);
    zstr text = zstr_from("Data:");
    map_StrMap database = map_init(StrMap, int_hash, int_cmp);

    push(&nums, 10);
    push(&nums, 20);
    
    push(&text, ' ');
    push(&text, '>');

    map_put(&database, 10, "User_A");
    map_put(&database, 20, "User_B");

    printf("%s\n", zstr_cstr(&text));

    printf("Vec[1]: %d\n", *at(&nums, 1));
    printf("Str[0]: %c\n", *at(&text, 0));

    const char **val = at(&database, 10);
    if (val) 
    {
        printf("Map[10]: %s\n", *val);
    } 
    else 
    {
        printf("Map[10]: Not Found\n");
    }

    del(&nums);
    del(&text);
    del(&database);

    return 0;
}

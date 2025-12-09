
#include <stdio.h>
#include "zvec.h"
#include "zstr.h"

DEFINE_VEC_TYPE(int, Int)

#define Z_SHORT_OPS
#include "zops.h"

int main(void) 
{
    vec_Int nums = vec_init(Int);
    zstr s = zstr_from("Hello");

    push(&nums, 10);
    push(&nums, 20);

    printf("Vector: ");
    foreach(it, &nums) 
    {
        printf("%d, ", *it);
    }
    printf("\n");

    printf("String: ");
    foreach(it, &s) 
    {
        printf("'%c', ", *it);
    }
    printf("\n");

    del(&nums);
    del(&s);
    return 0;
}


#include <stdio.h>
#include "zvec.h"
#include "zstr.h"
#include "zlist.h"

DEFINE_VEC_TYPE(int, Int)
DEFINE_LIST_TYPE(float, Float)

#define Z_SHORT_OPS
#include "zops.h"

int main(void) 
{
    vec_Int v = vec_init(Int);
    zstr s = zstr_from("Hello");
    list_Float l = list_init(Float);

    push(&v, 42);
    push(&s, '!');
    push(&l, 3.14f);

    printf("Vec Front: %d\n", *front(&v));
    printf("List Front: %.2f\n", *front(&l)); 
    printf("String Front: %c\n", *front(&s));

    del(&v);
    del(&s);
    del(&l);

    return 0;
}

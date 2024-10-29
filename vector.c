#include <stdlib.h>
#include <string.h>

#include "vector.h"

/// @file vector.c

static char *elem_at(struct vector *vec, uint32_t idx)
{
    return ((char *)vec->arr) + (idx * vec->memb_size);
}

int vector_init(struct vector *vec, size_t memb_size, uint32_t nmemb)
{
    vec->len = 0;
    vec->cap = nmemb * 2;
    vec->memb_size = memb_size;
    vec->arr = calloc(vec->cap, vec->memb_size);
    if (vec->arr == NULL)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int vector_insert(struct vector *vec, void *memb)
{
    if (vec->len == vec->cap)
    {
        vec->cap *= 2;
        vec->arr = realloc(vec->arr, vec->memb_size * vec->cap);
        if (vec->arr == NULL)
        {
            vec->len = 0;
            vec->cap = 0;
            return -1;
        }
        memset(elem_at(vec, vec->len), 0, (vec->cap - vec->len) * vec->memb_size);
    }
    char *elem = elem_at(vec, vec->len);
    memcpy(elem, memb, vec->memb_size);
    vec->len += 1;
    return 0;
}

void vector_destroy(struct vector *vec)
{
    free(vec->arr);
    vec->arr = NULL;
    vec->cap = 0;
    vec->len = 0;
}

#ifdef INCLUDE_MAIN
#include <stdio.h>
int main(void)
{
    struct vector v = {0};
    vector_init(&v, sizeof(int), 10);
    for (int i = 0; i < 32; i++)
    {
        vector_insert(&v, &i);
    }
    FOR_EACH(int *, ent, &v)
    {
        printf("%d Vector entry: %p=%d\n", ent_idx, (void *)ent, *ent);
    }
    vector_destroy(&v);
}
#endif

#include "dynamic_array.h"

int dynamic_array_init(dynamic_array_t *array, unsigned int n, size_t size)
{
    array->nelts = 0;
    array->elt_size = size;
    array->nalloc = n;

    array->elts = malloc(n * size);
    if (array->elts == NULL) {
        return DYNAMIC_ARRAY_ERROR;
    }

    return DYNAMIC_ARRAY_OK;
}

void dynamic_array_uninit(dynamic_array_t *array)
{
    array->nelts = 0;
    array->elt_size = 0;
    array->nalloc = 0;
    if (array->elts)
    {
        free(array->elts);
    }
}

dynamic_array_t *dynamic_array_create(unsigned int n, size_t size)
{
    dynamic_array_t *array;

    array = (dynamic_array_t *)malloc(sizeof(dynamic_array_t));
    if (array == NULL) {
        return NULL;
    }

    if (dynamic_array_init(array, n, size) != DYNAMIC_ARRAY_OK) {
        return NULL;
    }

    return array;
}
void dynamic_array_destroy(dynamic_array_t *array)
{
    dynamic_array_uninit(array);

    if (array)
    {
        free(array);
        array = NULL;
    }
}

void *dynamic_array_push(dynamic_array_t *array)
{
    void        *elt, *new;
    size_t       size;

    if (array->nelts == array->nalloc) {
        // 数组满了
        // 当前大小
        size = array->elt_size * array->nalloc;

        new = realloc(array->elts, 2 * size);           // 具体一次分配多少，这个算法可以再调，这里是直接增加一倍
        if (new == NULL) {
            return NULL;
        }

        array->elts = new;
        array->nalloc *= 2;
    }

    elt = (char *) array->elts + array->elt_size * array->nelts;
    array->nelts++;

    return elt;
}


void *dynamic_array_push_n(dynamic_array_t *array, unsigned int n)
{
    void            *elt, *new;
    size_t          size;
    unsigned int    nalloc;

    // 需要的大小
    size = n * array->elt_size;

    if (array->nelts + n > array->nalloc) {
        // 数组不够了

        nalloc = 2 * ((n >= array->nalloc) ? n : array->nalloc);
        new = realloc(array->elts, nalloc * array->elt_size);
        if (new == NULL) {
            return NULL;
        }

        array->elts = new;
        array->nalloc = nalloc;
    }

    elt = (char *) array->elts + array->elt_size * array->nelts;
    array->nelts += n;

    return elt;
}

void *dynamic_array_elts(dynamic_array_t *array)
{
    return array->elts;
}

unsigned int dynamic_array_nelts(dynamic_array_t *array)
{
    return array->nelts;
}

unsigned int dynamic_array_size(dynamic_array_t *array)
{
    return array->nalloc;
}

void *dynamic_array_pop(dynamic_array_t *array)
{
    void *elt = NULL;
    if (array->nelts > 0)
    {
        array->nelts--;
        elt = (char *)array->elts + array->elt_size * array->nelts;
    }

    return elt;
}

void *dynamic_array_pop_n(dynamic_array_t *array, unsigned int n)
{
    void *elt = NULL;

    if (array->nelts >= n)
    {
        array->nelts -= n;
        elt = (char *)array->elts + array->elt_size * array->nelts;
    }

    return elt;
}
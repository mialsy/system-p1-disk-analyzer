#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "elist.h"
#include "logger.h"

#define DEFAULT_INIT_SZ 10
#define RESIZE_MULTIPLIER 2

struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
};

bool idx_is_valid(struct elist *list, size_t idx);

struct elist *elist_create(size_t list_sz, size_t item_sz)
{
    // create space for the container struct
    struct elist *list = malloc(sizeof(struct elist));

    // check if the pointer is null for malloc
    if (list == NULL) {
        perror("malloc");
        return NULL;
    }

    // if list_sz is 0, just use default
    if (list_sz == 0) {
        list_sz = DEFAULT_INIT_SZ;
    }

    list->capacity = list_sz;
    list->item_sz = item_sz;
    list->size = 0;

    size_t storage_bytes = list->capacity * list->item_sz;

    LOG("Inittializing new elist: capacity=[%zu], item_size=[%zu], byte=[%zu]\n",
        list->capacity,
        list->item_sz,
        storage_bytes);

    list->element_storage = malloc(storage_bytes);

    // check if there is enough space for elemnt_storage
    if (list->element_storage == NULL) {
        perror("malloc");
        free(list);
        return NULL;
    }

    return list;
}

void elist_destroy(struct elist *list)
{
    free(list->element_storage);
    free(list);
}

// TODO: set capacity
int elist_set_capacity(struct elist *list, size_t capacity)
{
    return -1;
}

size_t elist_capacity(struct elist *list)
{
    return list->capacity;
}

ssize_t elist_add(struct elist *list, void *item)
{   
    if (list->size >= list->capacity) {
        list->capacity *= RESIZE_MULTIPLIER;
        LOG("Resizing the list, new capacity %zu\n", list->capacity);
        // for realloc set to its previous reference
        list->element_storage = realloc(list->element_storage, list->item_sz * list->capacity);
        if (list->element_storage == NULL) {
            perror("cannot resize");
            return -1;
        }
    }
    size_t index = list->size++;
    void *item_ptr = list->element_storage + index * list->item_sz;
    memcpy(item_ptr, item, list->item_sz);

    return index;
}

void *elist_add_new(struct elist *list)
{
        if (list->size >= list->capacity) {
        list->capacity *= RESIZE_MULTIPLIER;
        LOG("Resizing the list, new capacity %zu\n", list->capacity);
        // for realloc set to its previous reference
        list->element_storage = realloc(list->element_storage, list->item_sz * list->capacity);
        if (list->element_storage == NULL) {
            perror("cannot resize");
            return NULL;
        }
    }
    size_t index = list->size++;
    void *item_ptr = list->element_storage + index * list->item_sz;
    
    return item_ptr;
}

int elist_set(struct elist *list, size_t idx, void *item)
{
    void *item_ptr = elist_get(list, idx);
    if (item_ptr == NULL) {
        return -1;
    }
    memcpy(item_ptr, item, list->item_sz);
    return 0;
}

void *elist_get(struct elist *list, size_t idx)
{
    if (!idx_is_valid(list, idx)) {
        return NULL;
    }
    void *item_ptr = list->element_storage + idx * list->item_sz;
    return item_ptr;
}

size_t elist_size(struct elist *list)
{
    return list->size;
}

int elist_remove(struct elist *list, size_t idx)
{
    // check edge case
    if (!idx_is_valid(list, idx)) {
        return -1;
    }
    while (idx_is_valid(list, idx + 1)) {
        elist_set(list, idx, elist_get(list, idx + 1));
        idx++;
    }
    return 0;
}

void elist_clear(struct elist *list)
{
    list->size = 0;
}

void elist_clear_mem(struct elist *list)
{
    memset(list->element_storage, 0, list->size * list->item_sz);
    list->size = 0;
}

ssize_t elist_index_of(struct elist *list, void *item)
{
    for (size_t idx = 0; idx < list->size; idx++) {
        if (elist_get(list, idx) != NULL && memcmp(item, elist_get(list, idx), list->item_sz) == 0) {
            return idx;
        } 
    }
    return -1;
}

// TODO: sort list
void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{
}

bool idx_is_valid(struct elist *list, size_t idx)
{
    return idx >= 0 && idx < list->size;
}


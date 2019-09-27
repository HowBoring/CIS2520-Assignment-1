#ifndef DS_LIST_H
#define DS_LIST_H

#include "ds_memory.h"

#define FILENAME "list.bin"
#define SIZEOF_LEN sizeof(long)
#define SIZEOF_LIST_ITEM sizeof(struct ds_list_item_struct)

struct ds_list_item_struct
{
    int item;
    long next;
};

void
ds_create_list();

int
ds_init_list();

int
ds_replace(int value, long index);

int
ds_insert(int value, long index);

int
ds_delete(long index);

int
ds_swap(long index1, long index2);

long
ds_find(int target);

int
ds_read_elements(char* filename);

int
ds_finish_list();

#endif // DS_LIST_H

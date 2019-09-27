#include "ds_list.h"

void
ds_create_list()
{
    ds_create(FILENAME, SIZEOF_LEN + SIZEOF_LIST_ITEM * 256);
    if (!ds_init(FILENAME)) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }
    long start = ds_malloc(SIZEOF_LEN);
    long buffer = -1;
    ds_write(start, &buffer, SIZEOF_LEN);
    ds_finish();
    return 0;
}

int
ds_init_list()
{
    return ds_init(FILENAME);
}

int
ds_replace(int value, long index)
{
}

int
ds_insert(int value, long index)
{
    
}
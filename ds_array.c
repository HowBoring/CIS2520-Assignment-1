#include "ds_array.h"

// This global variable will hold the number of elements currently in the array.
long elements;

int
ds_create_array()
{
    ds_init("array.bin");
    ds_malloc(sizeof(long));
    long* buffer;
    ds_read(buffer, 0, sizeof(long));
    ds_malloc(sizeof(int) * buffer[0]);
    ds_finish();
}

int
ds_init_array()
{}
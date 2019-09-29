#include "ds_array.h"

// This global variable will hold the number of elements currently in the array.
long elements;

/* int
main(int argc, char const* argv[])
{
    ds_create_array();
    ds_init_array();
    ds_read_elements("values/value_100.data");
    ds_finish_array();
    return 0;
} */

int
ds_create_array()
{
    ds_create("array.bin", SIZEOF_LEN + SIZEOF_VALUE * MAX_ELEMENTS);
    ds_init("array.bin");
    ds_malloc(SIZEOF_LEN);
    long length = 0;
    ds_write(0, &length, SIZEOF_LEN);
    ds_malloc(SIZEOF_VALUE * MAX_ELEMENTS);
    ds_finish();
}

int
ds_init_array()
{
    ds_init("array.bin");
    long length;
    if (!ds_read(&length, 0, SIZEOF_LEN)) {
        perror("Array length reading failed");
        return EXIT_FAILURE;
    }

    elements = length;

    return 0;
}

int
ds_replace(int value, long index)
{
    if (index >= elements) {
        perror("Data replacing failed, invalid index");
        return EXIT_FAILURE;
    }

    long start = index * SIZEOF_VALUE + SIZEOF_LEN;
    if (ds_write(start, &value, SIZEOF_VALUE) != start) {
        perror("Data replacing failed");
        return EXIT_FAILURE;
    }

    return 0;
}

int
ds_insert(int value, long index)
{
    if (elements > MAX_ELEMENTS) {
        perror("Elements reach the upper limit");
        return EXIT_FAILURE;
    }

    if (index < 0 || index > elements) {
        perror("Index out of range");
        return EXIT_FAILURE;
    }

    long start = index * SIZEOF_VALUE + SIZEOF_LEN;
    if (index == elements) {
        ds_write(start, &value, SIZEOF_VALUE);
        elements++;
        return 0;
    } else {
        int value_tmp;
        ds_read(&value_tmp, start, SIZEOF_VALUE);
        ds_write(start, &value, SIZEOF_VALUE);
        return ds_insert(value_tmp, index + 1);
    }
}

int
ds_delete(long index)
{
    if (index < 0 || index >= elements) {
        perror("Index out of range");
        return EXIT_FAILURE;
    }

    elements--;

    for (size_t len = index; len < elements; len++) {
        long start_curr = index * SIZEOF_VALUE + SIZEOF_LEN;
        long start_next = start_curr + SIZEOF_VALUE;
        int* ptr_buffer;
        ds_read(ptr_buffer, start_next, SIZEOF_VALUE);
        ds_write(start_curr, ptr_buffer, SIZEOF_VALUE);
    }

    return 0;
}

int
ds_swap(long index1, long index2)
{
    if (index1 < 0 || index1 >= elements || index2 < 0 || index2 >= elements) {
        perror("Index out of range");
        return EXIT_FAILURE;
    }

    int ptr_value1, ptr_value2;
    long start1 = index1 * SIZEOF_VALUE + SIZEOF_LEN;
    long start2 = index2 * SIZEOF_VALUE + SIZEOF_LEN;

    ds_read(&ptr_value1, start1, SIZEOF_VALUE);
    ds_read(&ptr_value2, start2, SIZEOF_VALUE);

    ds_write(start2, &ptr_value1, SIZEOF_VALUE);
    ds_write(start1, &ptr_value2, SIZEOF_VALUE);

    return 0;
}

long
ds_find(int target)
{
    for (size_t index = 0; index < elements; index++) {
        long start = index * SIZEOF_VALUE + SIZEOF_LEN;
        int* read_buffer;
        ds_read(read_buffer, start, SIZEOF_VALUE);
        if (*read_buffer == target) {
            return index;
        }
    }
}

int
ds_read_elements(char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Data file opening failed");
        return EXIT_FAILURE;
    }

    int val_temp[MAX_ELEMENTS] = { 0 }, len = 0;

    while (!feof(fp)) {
        if (len > MAX_ELEMENTS - elements) {
            printf("%d, %d", len, elements);
            perror("The number of line in file exceeding `MAX_ELEMENTS`");
            return EXIT_FAILURE;
        }
        fscanf(fp, "%d", &val_temp[len++]);
    }

    for (size_t i = 0; i < len; i++) {
        ds_insert(val_temp[i], elements);
        // printf("elements: %d\n", elements);
    }

    return 0;
}

int
ds_finish_array()
{
    ds_write(0, &elements, SIZEOF_LEN);
    if (!ds_finish()) {
        perror("Finishing failed");
        return EXIT_FAILURE;
    }
    return 0;
}

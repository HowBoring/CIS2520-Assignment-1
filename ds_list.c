#include "ds_list.h"

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)

long head, elements = 0;

/* int
main(int argc, char const* argv[])
{
    ds_create_list();
    return 0;
} */

int
ds_create_list()
{
    ds_create(FILENAME, SIZEOF_LEN + SIZEOF_LIST_ITEM * 256);
    ds_init(FILENAME);
    long start = head = ds_malloc(SIZEOF_LEN);
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
    if (index >= elements) {
        perror("Invalid index");
        return EXIT_FAILURE;
    }

    long i = 0;
    long curr_pos, next_pos = head;

    struct ds_list_item_struct read_item;
    while (i <= index) {
        curr_pos = next_pos;
        ds_read(&read_item, curr_pos, SIZEOF_LIST_ITEM);
        next_pos = read_item.next;
        i++;
    }

    read_item.item = value;
    ds_write(curr_pos, &read_item, SIZEOF_LIST_ITEM);

    return 0;
}

int
ds_insert(int value, long index)
{
    if (index > elements) {
        perror("Invalid index");
        return EXIT_FAILURE;
    }

    if (index == 0) {
        struct ds_list_item_struct new_item;
        new_item.item = value;
        new_item.next = head;
        head = ds_malloc(SIZEOF_LIST_ITEM);
        ds_write(0, &head, SIZEOF_LEN);
        ds_write(head, &new_item, SIZEOF_LIST_ITEM);
        elements++;
        return 0;
    }

    long i = 0;
    long curr_pos, next_pos = head;

    struct ds_list_item_struct read_item;
    while (i < index) {
        curr_pos = next_pos;
        ds_read(&read_item, curr_pos, SIZEOF_LIST_ITEM);
        next_pos = read_item.next;
        i++;
    }

    struct ds_list_item_struct new_item;
    new_item.item = value;
    new_item.next = read_item.next;
    read_item.next = ds_malloc(SIZEOF_LIST_ITEM);

    // if (read_item.next == -1) {
    //     perror("Memory used up");
    //     return EXIT_FAILURE;
    // }

    ds_write(curr_pos, &read_item, SIZEOF_LIST_ITEM);
    ds_write(read_item.next, &new_item, SIZEOF_LIST_ITEM);
    elements++;

    return 0;
}

int
ds_delete(long index)
{
    if (index > elements) {
        perror("Invalid index");
        return EXIT_FAILURE;
    }

    long i = 0;
    long curr_pos, next_pos = head;

    struct ds_list_item_struct read_item;
    while (i < index) {
        curr_pos = next_pos;
        ds_read(&read_item, curr_pos, SIZEOF_LIST_ITEM);
        next_pos = read_item.next;
        i++;
    }

    struct ds_list_item_struct del_item;
    ds_read(&del_item, next_pos, SIZEOF_LIST_ITEM);

    read_item.next = del_item.next;
    ds_write(curr_pos, &read_item, SIZEOF_LIST_ITEM);
    ds_free(next_pos);
    elements--;

    return 0;
}

int
ds_swap(long index1, long index2)
{
    if (index1 > elements || index2 > elements) {
        perror("Invalid index");
        return EXIT_FAILURE;
    }

    long i = 0;
    long curr_pos, next_pos = head;
    long index1_pos, index2_pos;
    struct ds_list_item_struct read_item, item1, item2;

    while (i <= MAX(index1, index2)) {
        curr_pos = next_pos;
        ds_read(&read_item, curr_pos, SIZEOF_LIST_ITEM);
        if (i == MIN(index1, index2)) {
            index1_pos = curr_pos;
            item1 = read_item;
        }
        next_pos = read_item.next;
        i++;
    }

    index2_pos = curr_pos;
    item2 = read_item;
    int tmp = item1.item;
    item1.item = item2.item;
    item2.item = tmp;

    ds_write(index1_pos, &item1, SIZEOF_LIST_ITEM);
    ds_write(index2_pos, &item2, SIZEOF_LIST_ITEM);

    return 0;
}

long
ds_find(int target)
{
    long i = 0;
    long curr_pos, next_pos = head;

    struct ds_list_item_struct read_item;
    while (i <= elements) {
        curr_pos = next_pos;
        ds_read(&read_item, curr_pos, SIZEOF_LIST_ITEM);
        if (read_item.item == target) {
            return curr_pos;
        }
        next_pos = read_item.next;
        i++;
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

    int val_temp[256], len = 0;

    while (!feof(fp)) {
        if (len >= 256) {
            perror("The number of line in file exceeding `MAX_ELEMENTS`");
            return EXIT_FAILURE;
        }
        fscanf(fp, "%d", &val_temp[len++]);
    }

    for (size_t i = 0; i < len; i++) {
        ds_insert(val_temp[i], elements);
    }

    return 0;
}

int
ds_finish_list()
{
    ds_write(0, &head, SIZEOF_LEN);
    return !ds_finish();
}
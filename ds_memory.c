#include "ds_memory.h"

// These are two global variables. The first holds the file pointer and the
// blocks array, the second the read and write counts.
struct ds_file_struct ds_file;
struct ds_counts_struct ds_counts;

int
main(int argc, char const* argv[])
{
    ds_create("test.bin", 100);
    return 0;
}

int
ds_create(char* filename, long size)
{
    FILE *fp = fopen(filename, "wb+");
    ds_file.block[0].start = 0;
    ds_file.block[0].length = size;
    ds_file.block[0].alloced = 0;

    short* _buffer = malloc(size);
    memset(_buffer, 0, size);

    for (size_t i = 1; i < MAX_BLOCKS; i++) {
        ds_file.block[0].start = 0;
        ds_file.block[0].length = 0;
        ds_file.block[0].alloced = 0;
    }

    fwrite(ds_file.block,
           sizeof(struct ds_blocks_struct),
           sizeof(ds_file.block) / sizeof(struct ds_blocks_struct),
           ds_file.fp);

    fwrite(_buffer, 4, size, ds_file.fp);

    fclose(ds_file.fp);

    return 0;
}

int
ds_init(char* filename)
{}

long
ds_malloc(long amount)
{}

void
ds_free(long start)
{}

void*
ds_read(void* ptr, long start, long bytes)
{}

long
ds_write(long start, void* ptr, long bytes)
{}

int
ds_finish()
{}
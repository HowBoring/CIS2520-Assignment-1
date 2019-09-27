#include "ds_memory.h"

// These are two global variables. The first holds the file pointer and the
// blocks array, the second the read and write counts.
struct ds_file_struct ds_file;
struct ds_counts_struct ds_counts;

/* int
main(int argc, char const* argv[])
{
    ds_create("test.bin", 100);
    return 0;
} */

int
ds_create(char* filename, long size)
{
    FILE* fp = fopen(filename, "wb+");
    if (!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    struct ds_blocks_struct block[MAX_BLOCKS];

    block[0].start = 0;
    block[0].length = size;
    block[0].alloced = 0;

    short* addi_buffer = malloc(size);
    memset(addi_buffer, 0, size);

    for (size_t i = 1; i < MAX_BLOCKS; i++) {
        block[0].start = 0;
        block[0].length = 0;
        block[0].alloced = 0;
    }

    fwrite(ds_file.block, sizeof(struct ds_blocks_struct), MAX_BLOCKS, fp);

    fwrite(addi_buffer, 1, size, fp);

    fclose(fp);

    return 0;
}

int
ds_init(char* filename)
{
    FILE* fp = fopen(filename, "wb+");
    if (!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    ds_file.fp = fp;
    fread(
      ds_file.block, sizeof(struct ds_blocks_struct), MAX_BLOCKS, ds_file.fp);

    ds_counts.reads = ds_counts.writes = 0;

    return 0;
}

long
ds_malloc(long amount)
{
    size_t usable_block_index;

    // Find the 1st block that is unused and large enough to hold amount.
    for (usable_block_index = 0;
         usable_block_index < MAX_BLOCKS &&
         (ds_file.block[usable_block_index].alloced != 0 ||
          ds_file.block[usable_block_index].length < amount);
         usable_block_index++)
        ;

    if (usable_block_index == MAX_BLOCKS) {
        return -1;
    }

    long origin_length = ds_file.block[usable_block_index].length;
    ds_file.block[usable_block_index].length = amount;
    ds_file.block[usable_block_index].alloced = 1;

    size_t sec_block_index;
    // Find the 1st block whose length equal to 0.
    for (sec_block_index = 0; sec_block_index < MAX_BLOCKS &&
                              ds_file.block[sec_block_index].length != 0;
         sec_block_index++)
        ;

    ds_file.block[sec_block_index].start =
      ds_file.block[usable_block_index].start + amount;

    ds_file.block[sec_block_index].length = origin_length - amount;

    return ds_file.block[usable_block_index].start;
}

void
ds_free(long start)
{
    for (size_t i = 0; i < MAX_BLOCKS; i++) {
        if (ds_file.block[i].start == start) {
            ds_file.block[i].alloced = 0;
            break;
        }
    }
}

void*
ds_read(void* ptr, long start, long bytes)
{
    // Move the position indicator for the file stream to `start`.
    if (fseek(ds_file.fp,
              sizeof(struct ds_blocks_struct) * MAX_BLOCKS + start,
              SEEK_SET)) {
        perror("Position indicator for the file stream moving failed");
        return NULL;
    }
    // Allocate space for read data.
    ptr = malloc(bytes);
    // Read `bytes` bytes data from `start`.
    if (fread(ptr, 1, bytes, ds_file.fp) != bytes) {
        perror("Data reading failed");
        return NULL;
    }

    ds_counts.reads++;

    return ptr;
}

long
ds_write(long start, void* ptr, long bytes)
{
    // Move the position indicator for the file stream to `start`.
    if (fseek(ds_file.fp,
              sizeof(struct ds_blocks_struct) * MAX_BLOCKS + start,
              SEEK_SET)) {
        perror("Position indicator for the file stream moving failed");
        return -1;
    }

    if (fwrite(ptr, 1, bytes, ds_file.fp) != bytes) {
        perror("Data reading failed");
        return -1;
    }

    ds_counts.writes++;

    return start;
}

int
ds_finish()
{
    // Move the position indicator for the file stream to `start`.
    if (fseek(ds_file.fp, 0, SEEK_SET)) {
        perror("Position indicator for the file stream moving failed");
        return 0;
    }

    if (fwrite(ds_file.block,
               sizeof(struct ds_blocks_struct),
               MAX_BLOCKS,
               ds_file.fp) != MAX_BLOCKS) {
        perror("Data reading failed");
        return 0;
    }

    fclose(ds_file.fp);

    printf("reads: %d\nwrites: %d\n", ds_counts.reads, ds_counts.writes);

    return 1;
}

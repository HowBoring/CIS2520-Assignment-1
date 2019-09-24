#include <stdio.h>

// This constant indicates the maximum number of blocks of memory that we will
// track in the file.
#define MAX_BLOCKS 4096

// This data structure should count the number of read and write operations
// performed.
struct ds_counts_struct
{
    int reads;
    int writes;
};

// This is a structure that keeps track of one block in the binary file.
struct ds_blocks_struct
{
    long start;
    long length;
    char alloced;
};

// This is a structure that holds a file pointer to a file, open in binary,
// read/write mode, and an array of blocks.
struct ds_file_struct
{
    FILE* fp;
    struct ds_blocks_struct block[MAX_BLOCKS];
};

// Create a file with the provided filename.
int
ds_create(char* filename, long size);

// Open the given filename in binary read/write mode, save the file pointer in
// the global variable and load the header into the global variable.
int
ds_init(char* filename);


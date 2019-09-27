#ifndef DS_MEMORY_H
#define DS_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Search through the block array (from lowest index to highest) until it finds
// a block with length larger than, or equal to amount, and alloced equal to 0.
// When it finds such a block, it will change the length of the block to amount,
// and alloced to 1. This indicates that the appropriate amount of memory has
// now been allocated.
long
ds_malloc(long amount);

// Search through the block array (from lowest index to highest) until it finds
// a block whose start value matches start. It will then set the value allocated
// value of that block to 0.
void
ds_free(long start);

// This function should read bytes bytes from the location given by start,
// offset by the length of the header in the binary file. This should increment
// the value of reads in the counts variable by 1. If successful, it should
// return the address ptr, if unsuccessful it should return NULL.
void*
ds_read(void* ptr, long start, long bytes);

// This function should move the file pointer to the start location offset by
// the length of the header in the binary file, and write bytes bytes to the
// file from the address ptr. This should increment the value of writes in the
// counts variable by 1. If successful, the function should return a value of
// start. If something goes wrong, it should return a value of -1.
long
ds_write(long start, void* ptr, long bytes);

// This function should write the block array into the header at the beginning
// of the file and close the file.
int
ds_finish();

#endif // DS_MEMORY_H

#include "ds_array.h"
#include <time.h>

int
create_rand_file()
{
    int size_pool[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    srand(time(NULL));
    int list_size = 0;
    for (size_t i = 0; i < sizeof(size_pool) / sizeof(int); i++) {
        list_size = size_pool[i];
        char size_str[24] = { 0 };
        sprintf(size_str, "values/value_%d.data", list_size);
        FILE* fp = fopen(size_str, "w+");
        if (!fp) {
            perror("File creating failed");
            return EXIT_FAILURE;
        }

        for (size_t j = 0; j < list_size; j++) {
            fprintf(fp, "%d\n", rand() % 101);
        }
        fclose(fp);
    }

    return 0;
}

int
main()
{
    printf("Debugging is on\n");
    // create_rand_file();

    int size_pool[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

    for (size_t j = 0; j < 10; j++) {
        int size = size_pool[j];
        printf("The size is %d\n", size_pool[j]);
        ds_create_array();
        ds_init_array();
        char filename[32] = { 0 };
        sprintf(filename, "values/value_%d.data", size);
        ds_read_elements(filename);
        for (size_t i = 0; i < 30; i++) {
            long index = rand() % size;
            long index2 = rand() % size;
            ds_swap(index, index2);
        }

        ds_finish_array();

        printf("-------------------\n");
    }

    return 0;
}

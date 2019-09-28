CC = gcc

a1: ds_array.o ds_list.o ds_memory.o
	$(CC) -o a1 a1.c ds_array.o ds_list.o ds_memory.o

ds_memory.o: ds_memory.c
	$(CC) -c ds_memory.h ds_memory.c

ds_array.o: ds_array.c ds_memory.o
	$(CC) -c ds_array.h ds_array.c

ds_list.o: ds_list.c ds_memory.o
	$(CC) -c ds_list.h ds_list.c

clean:
	rm *.o *.gch a1
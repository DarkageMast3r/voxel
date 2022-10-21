#ifndef UTIL_H
#define UTIL_H

typedef unsigned int uint;

typedef struct {
	uint element_size;
	uint element_count;
	uint element_max;
	void *elements;
} list;

list list_create(uint element_size);
void list_insert(list *list, void *val);
void *list_get(list *list, uint index);


#endif

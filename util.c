#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

list list_create(uint element_size) {
        void *elements = malloc(element_size);

        list list_new = {
                .element_size = element_size,
                .element_count = 0,
                .element_max = 1,
                .elements = elements
        };
        return list_new;
}

void list_insert(list *list, void *val) {
	if (list->element_count == list->element_max) {
		list->element_max *= 2;
		
		void *new = realloc(list->elements, list->element_max * list->element_size);
		if (new == NULL) {
			printf("Failed to realloc\n");
			list->element_max /= 2;
			return;
		} else {
			list->elements = new;
		}
	}

	memcpy(
		list->elements + list->element_count * list->element_size,
		val,
		list->element_max
	);
}

void *list_get(list *list, uint index) {
	if (index >= list->element_count) {
		printf("Index out of range!\n");
		return NULL;
	}
	return list->elements + index * list->element_size;
}

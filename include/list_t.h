#ifndef LIST_T_H
#define LIST_T_H

#include <stddef.h>

typedef void **list_t; 
list_t list_new();                      
void list_add(list_t *listp, void *x);
void *list_get(list_t list, size_t i);  
size_t list_len(list_t list);     
void list_free(list_t list);     

#endif

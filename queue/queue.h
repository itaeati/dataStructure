#pragma comment(lib, ".\\Linked_List_Lib1.lib")

#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef QUEUE_H
#define QUEUE_H

typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy

int queue_enqueue(Queue* queue, const void* data);
int queue_dequeue(Queue* queue, void** data);

#define queue_peek(queue) ((queue)->head == NULL ? NULL : (queue)->head->data)
#define queue_size list_size

#endif

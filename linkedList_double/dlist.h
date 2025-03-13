#define _CRT_SECURE_NO_WARNINGS
#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DListElmt_ // 각 노드들의 구성 요소 정의
{
	void* data;

	struct DListElmt_* prev;
	struct DListElmt_* next;

}DListElmt;


typedef struct DList_  // 이중 연결 리스트의 구성 요소 정의
{
	int size;
	int (*match)(const void* key1, const void* key2);
	void (*destroy)(void* data);

	DListElmt* head;
	DListElmt* tail;

}DList;

// 공개 인터페이스 (= 유저가 사용가능한 메소드나 함수 즉, Public 개념)
void dlist_init(DList* list, void(*destroy)(void* data));
void dlist_destroy(DList* list);
int dlist_ins_next(DList* list, DListElmt* element, const void* data);
int dlist_ins_prev(DList* list, DListElmt* element, const void* data);
int dlist_remove(DList* list, DListElmt* element, void** data);
int add_node(DList* list, const void* data, const void* input ,int mode);
int rem_node(DList* list, const void* data);
int choice(int* sequence);

#define dlist_size(list) ((list)->size)
#define dlist_head(list) ((list)->head)
#define dlist_tail(list) ((list)->tail)
#define dlist_is_head(element) ((element)->prev == NULL ? 1:0)
#define dlist_is_tail(element) ((element)->next == NULL ? 1:0)
#define dlist_data(element) ((element)->data)
#define dlist_next(element) ((element)->next)
#define dlist_prev(element) ((element)->prev)



#endif
#pragma comment(lib, "Linked_List_Lib1.lib")
#define _CRT_SECURE_NO_WARNINGS
#ifndef CHTBL_H
#define CHTBL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

typedef struct CHTbl_
{
	int buckets;

	int (*h)(const void* key);

	int (*match)(const void* key1, const void* key2);

	void (*destroy)(void* data);

	int size;

	List* table;

}CHTbl;

// 공개 인터페이스
int chtbl_init(CHTbl* htbl, int buckets, int (*h)(const void* key), int(*match)(const void* key1, const void* key2), void (*destroy)(void* data));
void chtbl_destroy(CHTbl* htbl);
int chtbl_insert(CHTbl* htbl, const void* data);
int chtbl_remove(CHTbl* htbl, void** data);
int chtbl_lookup(const CHTbl* htbl, void** data);

#define chtbl_size(htbl) ((htbl)->size)

#endif
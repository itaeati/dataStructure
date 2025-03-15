#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ListElmt_ // list node�� ���� ��� ����
{
	void* data; // ������
	struct ListElmt_* next; // ���� ����� �ּҰ�
}ListElmt;

typedef struct List_ // node���� ���Ḯ��Ʈ�� ��� �����ؾ��ϴ����� �����ϴ� ���赵
{
	int size; // ���� ����Ʈ�� ��� ����
	int (*match)(const void* key1, const void* key2); // Ű �� �Լ� ������ (�̴� �޼ҵ��� ��İ� ����)
	void (*destroy)(void* data); // ��� ������ ���� �Լ� ������
	ListElmt* head; // ���� ��� �ּҰ�
	ListElmt* tail; // ������ ��� �ּҰ�

}List;

// ���� �������̽� (= ������ ��밡���� �޼ҵ峪 �Լ� ��, Public ����)
void list_init(List* list, void (*destroy)(void* data)); // ���� ����Ʈ �ʱ�ȭ �Լ�
void list_destroy(List* list); // ���Ḯ��Ʈ ���� �Լ�
int list_ins_next(List* list, ListElmt* elemnet, const void* data); // ���� ����Ʈ �߰� �Լ� (�� ������ ���� ����� �ּҰ��� ������. �� ���� �� �Լ��� ����Ѵٸ�, ��� ��� ������ ��带 �߰��ϱ� ���ؼ��� ������ �Ǵ� ��忡 �߰��� ����� �ּҰ��� �ְ�, ���� ��� �ּҰ��� ������ �ִٰ� �߰��� ��忡���� ���� ����� �ּҰ��� �����Ѵ�.)
int list_rem_next(List* list, ListElmt* element, void** data); // ���� ����Ʈ ���� �Լ� (�ʿ� ���� ��带 ����� ���ؼ��� ������� �ϴ� ��带 �Ҵ� ���� ��Ų ��, ���� ��� �ּҰ��� ���� ��忡�� ����Ű�� ���� ��� �ּҰ����� ������ �ش�.)
int add_node(List* list, const void* data, const void* input);
int rem_node(List* list, const void* data);
int choice(int* sequence);

#define list_size(list) ((list) -> size)
#define list_head(list) ((list) -> head)
#define list_tail(list) ((list) -> tail)
#define list_is_head(list, element) ((element) == (list) -> head ? 1 : 0)
#define list_is_tail(list, element) ((element) -> next == NULL ? 1 : 0)
#define list_data(element) ((element) -> data)
#define list_next(element) ((element) -> next)

#endif;
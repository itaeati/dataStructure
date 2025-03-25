#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ListElmt_ // list node의 구성 요소 정의
{
	void* data; // 데이터
	struct ListElmt_* next; // 다음 노드의 주소값
}ListElmt;

typedef struct List_ // node들을 연결리스트로 어떻게 연결해야하는지를 설계하는 설계도
{
	int size; // 연결 리스트의 노드 개수
	int (*match)(const void* key1, const void* key2); // 키 비교 함수 포인터 (이는 메소드의 방식과 유사)
	void (*destroy)(void* data); // 노드 데이터 삭제 함수 포인터
	ListElmt* head; // 시작 노드 주소값
	ListElmt* tail; // 마지막 노드 주소값

}List;

// 공개 인터페이스 (= 유저가 사용가능한 메소드나 함수 즉, Public 개념)
void list_init(List* list, void (*destroy)(void* data)); // 연결 리스트 초기화 함수
void list_destroy(List* list); // 연결리스트 삭제 함수
int list_ins_next(List* list, ListElmt* elemnet, const void* data); // 연결 리스트 추가 함수 (각 노드들은 다음 노드의 주소값을 가진다. 이 때에 이 함수를 사용한다면, 어떠한 노드 다음에 노드를 추가하기 위해서는 기준이 되는 노드에 추가할 노드의 주소값을 넣고, 다음 노드 주소값을 가지고 있다가 추가한 노드에서의 다음 노드의 주소값에 삽입한다.)
int list_rem_next(List* list, ListElmt* element, void** data); // 연결 리스트 삭제 함수 (필요 없는 노드를 지우기 위해서는 지우고자 하는 노드를 할당 해제 시킨 후, 다음 노드 주소값을 이전 노드에서 가르키는 다음 노드 주소값으로 변경해 준다.)
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
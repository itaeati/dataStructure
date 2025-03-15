// 링커에게 ".\Linked_List_Lib1.lib" 라이브러리를 링크하도록 지시합니다.
#pragma comment(lib, ".\\Linked_List_Lib1.lib")

#ifndef STACK_H
#define STACK_H

#include "list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * 스택(Stack)은 내부적으로 연결 리스트(List)를 기반으로 구현됩니다.
 * 따라서, 기존 List 구조체를 Stack의 별칭(alias)으로 사용하기 위해 typedef를 사용합니다.
 */
typedef List Stack;

// 아래 매크로들은 기존 연결 리스트 관련 함수들을 스택 기능으로서 사용하도록 재정의합니다.
// 즉, 스택 초기화는 연결 리스트 초기화와 동일하게 처리합니다.
#define stack_init list_init      
#define stack_destroy list_destroy

/*
 * stack_push: 스택에 새로운 데이터를 푸쉬(삽입)하는 함수.
 * 매개변수:
 *   - stack: 데이터를 삽입할 대상 스택(연결 리스트 기반)
 *   - data: 스택에 삽입될 데이터. 일반적으로 void* 형태로 데이터를 전달하며,
 *           내부적으로 스택(연결 리스트)의 노드에 저장됩니다.
 *
 * 반환값:
 *   - 성공 시 0, 실패 시 0이 아닌 값을 반환.
 */
int stack_push(Stack* stack, const void* data);

/*
 * stack_pop: 스택에서 데이터를 팝(꺼내기)하는 함수.
 * 매개변수:
 *   - stack: 데이터를 꺼낼 스택(연결 리스트 기반)
 *   - data: 팝된 데이터를 저장할 포인터의 주소.
 *
 * 반환값:
 *   - 성공 시 0, 실패 시 0이 아닌 값을 반환.
 *   팝된 데이터는 동적 할당되어 있으면 사용 후 반드시 free()를 통해 메모리 해제해야 할 수 있음.
 */
int stack_pop(Stack* stack, void** data);

/*
 * stack_peek: 스택의 최상위(top) 데이터를 확인할 때 사용하는 매크로.
 * 매개변수:
 *   - stack: 확인할 스택의 포인터.
 * 반환값:
 *   - 스택이 비어있다면 NULL을, 그렇지 않으면 최상위 노드의 데이터를 반환합니다.
 */
#define stack_peek(stack) ((stack)->head == NULL ? NULL : (stack->head->data))

 /*
  * stack_size: 스택의 노드 개수를 리턴하는 매크로를 정의하려는 의도로 보입니다.
  * 여기서는 연결 리스트의 list_size와 동일한 기능을 수행하도록 설정합니다.
  * (주의: 매크로 정의 끝에 세미콜론(;)를 붙이면, 호출 시 중복된 세미콜론으로 오류가 발생할 수 있으므로
  *   일반적으로 매크로 정의에서는 세미콜론을 생략합니다.)
  */
#define stack_size list_size

#endif

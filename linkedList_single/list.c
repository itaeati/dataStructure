#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * list_init - 연결 리스트 초기화
 * ----------------------------
 * 리스트 구조체의 각 필드를 초기 값으로 지정한다.
 *
 * 매개변수:
 *   list     - 초기화할 리스트 구조체에 대한 포인터
 *   destroy  - 노드가 제거될 때 데이터 해제를 위한 사용자 정의 함수 포인터 (없으면 NULL)
 *
 * 동작:
 *   리스트의 크기를 0으로 설정하고, destroy 함수 포인터를 등록하며,
 *   head와 tail 포인터를 NULL로 초기화한다.
 */
void list_init(List* list, void (*destroy)(void* data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
    return;
}

/*
 * list_destroy - 리스트의 모든 노드 제거 및 구조체 초기화
 * -------------------------------------------------------
 * 리스트에 있는 모든 노드를 삭제하고, 등록된 destroy 함수를 사용하여
 * 각 노드의 데이터를 해제한 후, 리스트 구조체를 0으로 초기화한다.
 *
 * 매개변수:
 *   list - 삭제할 노드들이 포함된 리스트 구조체 포인터
 *
 * 동작:
 *   list_rem_next() 함수를 반복 호출하여 노드를 순차적으로 제거하며,
 *   등록된 destroy 함수가 있으면 호출하여 노드 데이터도 해제한다.
 */
void list_destroy(List* list)
{
    void* data;

    // 리스트에 노드가 남아있는 동안 반복
    while (list_size(list) > 0)
    {
        // element가 NULL이면 head 노드 제거. 성공 시 data에 제거된 데이터 저장.
        if (list_rem_next(list, NULL, (void**)&data) == 0 && list->destroy != NULL)
        {
            // 사용자가 등록한 destroy 함수로 데이터를 해제
            list->destroy(data);
        }
    }

    // 리스트 구조체 자체의 내용을 0으로 초기화
    memset(list, 0, sizeof(List));
    return;
}

/*
 * list_ins_next - 연결 리스트에 새로운 노드 삽입
 * ------------------------------------------------
 * 지정한 노드(element)의 다음 위치에 새로운 노드를 추가한다.
 * 만약 element가 NULL이면 리스트의 맨 앞(head)에 삽입된다.
 *
 * 매개변수:
 *   list    - 노드를 삽입할 대상 리스트
 *   element - 기준이 되는 기존 노드 (NULL이면 head에 삽입)
 *   data    - 새 노드에 저장할 데이터 (void* 형식)
 *
 * 반환값:
 *   삽입 성공 시 0, 메모리 할당 실패 등 오류 시 -1 반환
 *
 * 동작:
 *   1. 새 노드에 대한 메모리 할당을 시도한다.
 *   2. element가 NULL일 경우, 새 노드를 리스트의 시작 부분에 삽입한다.
 *   3. 그렇지 않으면 element의 다음 위치에 노드를 연결한다.
 *   4. 리스트의 size를 증가시키고, tail 포인터의 갱신 여부를 체크한다.
 */
int list_ins_next(List* list, ListElmt* element, const void* data)
{
    ListElmt* new_element;

    // 새 노드를 위한 메모리 할당 실패 시 -1 반환
    if ((new_element = (ListElmt*)malloc(sizeof(ListElmt))) == NULL)
        return -1;

    new_element->data = (void*)data;

    if (element == NULL) // 기준 노드가 없는 경우: 리스트 맨 앞에 삽입
    {
        if (list_size(list) == 0)
            list->tail = new_element;  // 빈 리스트인 경우 새 노드가 tail이 된다.
        new_element->next = list->head;
        list->head = new_element;
    }
    else // 기준 노드가 있는 경우: element 뒤에 삽입
    {
        if (element->next == NULL)
            list->tail = new_element;  // element가 현재 tail이면, 새 노드가 새로운 tail이 됨.
        new_element->next = element->next;
        element->next = new_element;
    }

    list->size++;  // 리스트에 추가된 노드 하나 증가
    return 0;
}

/*
 * list_rem_next - 연결 리스트에서 지정 노드의 다음 노드 제거
 * ------------------------------------------------------------
 * 지정된 노드(element)의 다음 노드를 제거하고, 해당 노드의 데이터를
 * data 포인터를 통해 반환한다. element가 NULL이면 리스트의 head 노드를 제거한다.
 *
 * 매개변수:
 *   list    - 노드를 제거할 대상 리스트
 *   element - 기준 노드 (NULL이면 head 노드를 제거)
 *   data    - 제거된 노드의 데이터를 저장할 이중 포인터
 *
 * 반환값:
 *   제거 성공 시 0, 오류 시 -1 반환
 *
 * 동작:
 *   1. 리스트가 비어있으면 직접 -1 반환.
 *   2. element가 NULL인 경우, head 노드를 제거하고,
 *      리스트의 head 포인터를 다음 노드로 이동시킨다.
 *   3. element가 존재하면, element의 다음 노드가 존재하는지 확인 후 제거 처리.
 *   4. tail 업데이트 및 메모리 해제를 진행하고, 리스트의 size를 감소시킨다.
 */
int list_rem_next(List* list, ListElmt* element, void** data)
{
    ListElmt* old_element;

    if (list_size(list) == 0)
        return -1;

    if (element == NULL) // head 노드 제거
    {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1)
            list->tail = NULL;
    }
    else // element가 지정된 경우
    {
        if (element->next == NULL)
            return -1;

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == NULL)
            list->tail = element;
    }
    free(old_element);
    list->size--;
    return 0;
}

/*
 * add_node - 리스트에 새로운 노드 삽입 (사용자 입력 기반 기준 노드 활용)
 * ----------------------------------------------------------------------------
 * 리스트에 새 데이터를 삽입하기 위해, 사용자가 기준으로 삼을 노드(혹은 중복 발생 시
 * 여러 후보)를 찾은 뒤 적절한 위치에 삽입하는 함수이다.
 *
 * 매개변수:
 *   list  - 데이터를 삽입할 리스트
 *   data  - 새 노드에 저장할 데이터
 *   input - 삽입 기준 노드의 데이터를 결정하기 위한 사용자 입력 값
 *
 * 동작:
 *   1. 리스트의 크기가 2 이상이면, 리스트 전체를 순회하며 input과 일치하는 노드들을 찾는다.
 *      일치하는 노드들을 임시 배열(address)에 저장한다.
 *   2. 후보 노드가 여러 개이면, choice() 함수를 호출하여 몇 번째 후보를 기준으로 할지 결정한다.
 *      선택된 노드 뒤에 새 노드를 삽입한다.
 *   3. 후보가 하나이거나 input이 NULL이 아닌 경우 순회하여 해당 노드 뒤에 새 노드를 삽입한다.
 *   4. 리스트의 크기가 1 이하이면, 단순히 현재 head 뒤에 새 노드를 삽입한다.
 *   5. 동적으로 할당한 임시 배열(address)은 사용 후 해제한다.
 *
 * 반환값:
 *   성공 시 0, 실패 시 -1을 반환한다.
 */
int add_node(List* list, const void* data, const void* input)
{
    ListElmt* current = list->head;
    ListElmt* check = list->head;

    // 입력값과 일치하는 노드들을 저장할 임시 배열 (후에 후보 선택에 사용)
    ListElmt** address = malloc(list->size * sizeof(ListElmt*));
    if (address == NULL)
        return -1;

    int i = 0;
    if (list->size >= 2)
    {
        // 리스트를 순회하며 입력값(input)과 일치하는 노드를 찾는다.
        while (check != NULL)
        {
            if (list->match(check->data, input))
            {
                address[i] = check;
                i++;
            }
            check = check->next;
        }
    }

    if (list->size >= 2)
    {
        if (i > 1)
        {
            int k = 0;
            // 중복 후보가 여러 개라면, 사용자가 몇 번째를 기준으로 할지 선택
            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }
            // 선택된 후보 노드(address[k]) 뒤에 새 데이터를 삽입
            if (list_ins_next(list, address[k], data) != 0)
            {
                free(address);
                return -1;
            }
        }
        else
        {
            if (input != NULL)
            {
                // 후보가 하나이면, 해당 노드까지 순회하며 찾아서 그 뒤에 삽입
                while (current != NULL && list->match(current->data, input) == 0)
                    current = current->next;
                if (list_ins_next(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
            else
            {
                // input이 NULL인 경우에는 리스트의 head 뒤에 삽입
                if (list_ins_next(list, NULL, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
        }
    }
    else
    {
        // 리스트의 크기가 0 또는 1이면, 현재 head 뒤에 삽입
        if (list_ins_next(list, current, data) != 0)
        {
            free(address);
            return -1;
        }
    }
    free(address);
    return 0;
}

/*
 * rem_node - 리스트에서 일치하는 노드를 제거
 * -----------------------------------------------------------
 * 리스트에서 data와 일치하는 노드를 제거할 때, 삭제 후보(중복될 수 있음)의
 * 이전 노드들을 임시 배열에 저장하고, 중복이 있으면 사용자가 제거할 대상을 선택한다.
 *
 * 매개변수:
 *   list - 노드를 제거할 리스트
 *   data - 제거 대상의 데이터 (삭제 기준)
 *
 * 동작:
 *   1. 리스트 전체를 순회하며, data와 일치하는 노드를 찾는다. 이 때, 각 노드의 이전 노드 포인터를
 *      임시 배열(address)에 저장한다.
 *   2. 리스트의 크기가 2보다 크면:
 *      - 삭제 후보가 여러 개인 경우, choice() 함수를 이용해 제거할 후보를 선택한 후,
 *        선택된 후보의 다음 노드를 제거한다.
 *      - 삭제 후보가 하나이면, 순회를 통해 해당 노드를 찾고 그 이전 노드 뒤의 노드를 제거한다.
 *   3. 리스트의 크기가 2 이하이면, 단순하게 head 뒤의 노드를 제거한다.
 *   4. 사용 후 임시 배열(address)을 해제한다.
 *
 * 반환값:
 *   성공 시 0, 실패 시 -1을 반환한다.
 */
int rem_node(List* list, const void* data)
{
    // 리스트가 비어있으면 삭제할 수 없음
    if (list->size == 0)
        return -1;

    ListElmt* current = list->head;
    ListElmt* check = list->head;
    ListElmt* prev = NULL;

    // 일치하는(삭제 후보) 노드의 "이전 노드" 포인터들을 저장할 임시 배열 할당
    ListElmt** address = malloc(list->size * sizeof(ListElmt*));
    if (address == NULL)
        return -1;

    int i = 0;
    // 리스트 전체를 순회하며, data와 일치하는 노드를 찾고, 그 노드의 이전 노드를 address 배열에 저장
    while (check != NULL)
    {
        if (list->match(check->data, data))
        {
            address[i] = prev;  // 후보가 head인 경우 prev는 NULL이 됨
            i++;
        }
        prev = check;
        check = check->next;
    }

    int ret = 0;
    void* temp;  // list_rem_next 함수에서 제거한 노드의 데이터를 받는 변수

    if (list->size > 2)
    {
        if (i > 1)
        {
            // 여러 후보가 있는 경우, 사용자가 몇 번째 후보를 기준으로 할지를 선택
            int k = 0;
            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }
            ret = list_rem_next(list, address[k], &temp);
            if (ret != 0)
            {
                free(address);
                return ret;
            }
        }
        else
        {
            // 후보가 하나인 경우 : 재초기화하여 삭제 대상 탐색
            // 이렇게 함으로써 만약 삭제 대상이 head이면 prev는 NULL이 되어, head 삭제가 올바르게 처리됨.
            current = list->head;
            prev = NULL;
            while (current != NULL && !list->match(current->data, data))
            {
                prev = current;
                current = current->next;
            }
            ret = list_rem_next(list, prev, &temp);
            if (ret != 0)
            {
                free(address);
                return ret;
            }
        }
    }
    else
    {
        // 리스트의 크기가 2 이하인 경우:
        // 만약 head 노드 자체가 삭제 대상이면 element에 NULL을 전달하여 head 노드를 제거한다.
        if (list->match(list->head->data, data))
            ret = list_rem_next(list, NULL, &temp);
        else
            ret = list_rem_next(list, list->head, &temp);

        if (ret != 0)
        {
            free(address);
            return ret;
        }
    }
    free(address);
    return 0;
}


/*
 * choice - 중복 노드에 대한 기준 선택 함수
 * -----------------------------------------------------------
 * 중복된 노드가 여러 개 있을 경우, 사용자가 몇 번째 노드를
 * 기준으로 할지 입력받는다.
 *
 * 매개변수:
 *   sequence - 사용자 입력을 저장할 정수형 포인터
 *
 * 반환값:
 *   scanf의 반환값 (정상 입력 시 1, 입력 실패 시 오류 코드)
 */
int choice(int* sequence)
{
    printf("몇 번째 중복 노드를 기준으로 하시겠습니까?\n");
    return scanf("%d", sequence);
}

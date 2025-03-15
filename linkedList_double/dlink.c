#include "dlist.h"

/*
 * dlist_init - 이중 연결 리스트 초기화 함수
 * ----------------------------------------------------------------------------
 * 이 함수는 이중 연결 리스트를 초기 상태로 설정한다.
 * 리스트의 크기를 0으로 초기화하고, 데이터 소멸 함수 포인터(destroy) 및
 * 리스트의 head와 tail을 NULL로 설정한다.
 *
 * 매개변수:
 *   list    - 초기화할 이중 연결 리스트의 포인터
 *   destroy - 리스트 내 데이터 소멸 시 호출할 함수 포인터 (없으면 NULL)
 *
 * 반환값:
 *   없음.
 */
void dlist_init(DList* list, void (*destroy)(void* data))
{
    list->size = 0;         // 리스트 내 노드 개수를 0으로 초기화
    list->destroy = destroy;  // 사용자 정의 데이터 소멸 함수 지정
    list->head = NULL;      // 리스트의 시작 노드를 NULL로 설정
    list->tail = NULL;      // 리스트의 마지막 노드를 NULL로 설정

    return;
}


/*
 * dlist_destroy - 이중 연결 리스트의 모든 노드를 제거하는 함수
 * ----------------------------------------------------------------------------
 * 이 함수는 리스트가 비어질 때까지 tail 노드를 순차적으로 제거한다.
 * 제거 과정 중, 제거된 노드의 데이터가 존재하고 소멸 함수가 등록되어 있다면,
 * 해당 데이터를 소멸 처리한다.
 * 마지막으로 리스트 구조체의 메모리를 0으로 초기화한다.
 *
 * 매개변수:
 *   list - 제거할 노드를 포함하는 이중 연결 리스트의 포인터
 *
 * 반환값:
 *   없음.
 */
void dlist_destroy(DList* list)
{
    void* data;

    // 리스트에 노드가 있는 동안 tail 노드를 제거한다.
    while (dlist_size(list) > 0)
    {
        // tail 노드를 제거하고, 제거된 데이터가 있다면 소멸 함수로 처리
        if (dlist_remove(list, dlist_tail(list), (void**)&data) == 0 && list->destroy != NULL)
        {
            list->destroy(data);
        }
    }

    // 리스트 구조체의 모든 멤버를 0으로 초기화시켜 잔여 데이터를 제거
    memset(list, 0, sizeof(DList));

    return;
}


/*
 * dlist_ins_next - 이중 연결 리스트의 지정된 노드 뒤에 새 노드를 삽입하는 함수
 * ----------------------------------------------------------------------------
 * 이 함수는 지정된 기준 노드(element)의 뒤쪽에 새로운 노드를 삽입한다.
 * 만약 리스트가 비어있다면, 새 노드를 head와 tail로 설정한다.
 *
 * 매개변수:
 *   list    - 데이터를 삽입할 이중 연결 리스트의 포인터
 *   element - 새 노드를 삽입할 기준 노드 (리스트가 비어있지 않은 경우 NULL이면 오류)
 *   data    - 새 노드에 저장할 데이터
 *
 * 반환값:
 *   성공하면 0, 실패하면 -1을 반환한다.
 */
int dlist_ins_next(DList* list, DListElmt* element, const void* data)
{
    DListElmt* new_element;

    // 리스트가 비어있지 않은데 element가 NULL이면 삽입할 수 없음
    if (element == NULL && dlist_size(list) != 0)
        return -1;

    // 새 노드 메모리 할당 실패 시 에러 반환
    if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
        return -1;

    new_element->data = (void*)data;  // 새 노드에 저장할 데이터 설정

    if (dlist_size(list) == 0)  // 리스트가 비어있는 경우
    {
        // 새 노드를 리스트의 유일한 노드로 설정
        list->head = new_element;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_element;
    }
    else  // 리스트에 이미 노드가 존재하는 경우
    {
        // 기준 노드(element)의 뒤에 새 노드를 삽입
        new_element->next = element->next;  // 새 노드의 next는 기준 노드의 다음 노드
        new_element->prev = element;          // 새 노드의 prev는 기준 노드를 가리킴

        if (element->next == NULL)
            list->tail = new_element;  // 기준 노드가 tail이면, 새 노드가 새로운 tail이 됨
        else
            element->next->prev = new_element;  // 기존 다음 노드의 prev를 새 노드로 갱신

        element->next = new_element;  // 기준 노드의 next를 새 노드로 연결
    }

    list->size++;  // 리스트 크기 증가

    return 0;  // 삽입 성공
}


/*
 * dlist_ins_prev - 이중 연결 리스트의 지정된 노드 앞에 새 노드를 삽입하는 함수
 * ----------------------------------------------------------------------------
 * 이 함수는 지정된 기준 노드(element)의 앞쪽에 새로운 노드를 삽입한다.
 * 만약 리스트가 비어있으면, 새 노드를 head와 tail로 설정한다.
 *
 * 매개변수:
 *   list    - 데이터를 삽입할 이중 연결 리스트의 포인터
 *   element - 새 노드를 삽입할 기준 노드 (리스트가 비어있지 않은 경우 NULL이면 오류)
 *   data    - 새 노드에 저장할 데이터
 *
 * 반환값:
 *   성공하면 0, 실패하면 -1을 반환한다.
 */
int dlist_ins_prev(DList* list, DListElmt* element, const void* data)
{
    DListElmt* new_element;

    // 리스트가 비어있지 않은데 기준 노드(element)가 NULL이면 삽입할 수 없음
    if (element == NULL && list->size != 0)
        return -1;

    // 새 노드 생성 실패 시 에러 반환
    if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
        return -1;

    new_element->data = (void*)data;  // 새 노드에 저장할 데이터 설정

    if (dlist_size(list) == 0)  // 리스트가 비어있는 경우
    {
        // 새 노드를 유일한 노드로 설정
        list->head = new_element;
        list->tail = new_element;
        list->head->prev = NULL;
        list->tail->next = NULL;
    }
    else  // 리스트에 노드가 존재하는 경우
    {
        // 기준 노드(element)의 앞쪽에 새 노드를 삽입
        new_element->next = element;           // 새 노드의 next는 기준 노드를 가리킴
        new_element->prev = element->prev;       // 새 노드의 prev는 기준 노드의 이전 노드를 가리킴

        if (element->prev == NULL)
            list->head = new_element;  // 기준 노드가 head이면, 새 노드가 새로운 head가 됨
        else
            element->prev->next = new_element;  // 기존 이전 노드의 next를 새 노드로 갱신

        element->prev = new_element;  // 기준 노드의 prev를 새 노드로 연결
    }
    list->size++;  // 리스트 크기 증가

    return 0;  // 삽입 성공
}


/*
 * dlist_remove - 이중 연결 리스트에서 지정된 노드를 제거하는 함수
 * ----------------------------------------------------------------------------
 * 이 함수는 지정된 노드(element)를 리스트에서 제거하고,
 * 제거된 노드의 데이터를 data 포인터를 통해 호출자에게 반환한다.
 * 또한, 제거 후 리스트 내 연결 관계를 재정비하고, 할당된 메모리를 해제한다.
 *
 * 매개변수:
 *   list    - 노드를 제거할 이중 연결 리스트의 포인터
 *   element - 제거할 노드를 가리키는 포인터
 *   data    - 제거된 노드의 데이터를 저장할 포인터의 주소
 *
 * 반환값:
 *   성공하면 0, 실패하면 -1을 반환한다.
 */
int dlist_remove(DList* list, DListElmt* element, void** data)
{
    // 제거할 노드가 NULL이거나 리스트에 노드가 없는 경우 에러 반환
    if (element == NULL || dlist_size(list) == 0)
        return -1;

    *data = element->data;  // 제거하는 노드의 데이터를 호출자에게 반환

    if (element == list->head)
    {
        // 제거할 노드가 head인 경우, head를 다음 노드로 변경
        list->head = element->next;

        if (list->head == NULL)
            // 리스트에 남은 노드가 없으면 tail도 NULL로 설정
            list->tail = NULL;
        else
            // 새 head의 prev 포인터를 NULL로 초기화
            element->next->prev = NULL;
    }
    else
    {
        // 제거할 노드가 중간 또는 tail인 경우, 이전 노드와 다음 노드를 연결
        element->prev->next = element->next;

        if (element->next == NULL)
            // 제거할 노드가 tail이면 tail을 이전 노드로 갱신
            list->tail = element->prev;
        else
            element->next->prev = element->prev;  // 중간 노드라면 다음 노드의 prev 갱신
    }

    free(element);   // 제거된 노드의 메모리 해제
    list->size--;    // 리스트 내 노드 개수를 1 감소

    return 0;  // 제거 성공
}


/*
 * add_node - 이중 연결 리스트에 새 노드를 기준 노드의 앞 또는 뒤에 삽입하는 함수
 * ----------------------------------------------------------------------------
 * 설명:
 *   이 함수는 이중 연결 리스트(list)에 대해, 사용자가 제공한 입력(input)을 기준으로
 *   삽입할 위치를 결정하여, 해당 위치 바로 앞이나 뒤 (mode에 따라)에 새 노드를 추가합니다.
 *   만약 기준 데이터(input)와 일치하는 노드가 여러 개 존재한다면, 사용자가 선택(choice())
 *   를 통해 원하는 후보 노드를 결정할 수 있습니다.
 *
 * 매개 변수:
 *   list  - 새 노드가 삽입될 대상 이중 연결 리스트
 *   data  - 새 노드에 저장할 데이터 (불변)
 *   input - 삽입 기준 노드를 선택하기 위한 사용자 입력 데이터
 *   mode  - 삽입 모드 (1: 기준 노드의 뒤에 삽입, 2: 기준 노드의 앞에 삽입)
 *
 * 반환값:
 *   삽입에 성공하면 0, 메모리 할당 실패나 삽입 오류 등 문제가 있으면 -1을 반환한다.
 */
int add_node(DList* list, const void* data, const void* input, int mode)
{
    /*
     * current:
     *    리스트 순회 시 기준이 되는 노드 포인터
     * check:
     *    input 값과 매칭되는 노드를 찾기 위해 리스트를 순회할 때 사용하는 포인터
     */
    DListElmt* current = list->head;
    DListElmt* check = list->head;

    /*
     * address:
     *    input과 일치하는 후보 노드들을 저장하기 위한 임시 배열.
     *    리스트의 전체 크기만큼 공간을 할당하며, 최대 후보 수는 list->size와 같다.
     */
    DListElmt** address = malloc(list->size * sizeof(DListElmt*));
    if (address == NULL)
        return -1;  // 메모리 할당 실패 시

    int i = 0;  // input과 일치하는 후보 노드의 개수를 센다.

    /*
     * 리스트의 크기가 2 이상이면, 전체 노드를 순회하면서 input 값과 일치하는 노드를 찾는다.
     * 찾은 경우 해당 노드의 주소를 address 배열에 저장하고 후보 개수(i)를 증가시킨다.
     */

    if (list->size >= 2)
    {
        while (check != NULL)
        {
            // list->match() 함수로 현재 노드의 데이터와 input을 비교한다.
            if (list->match(check->data, input) == 1)
            {
                address[i] = check;  // 일치하면 후보 배열에 저장
                i++;
            }
            check = check->next;
        }
    }

    /*
     * 모드 1: 기준 노드의 "뒤"에 새 노드를 삽입하는 경우
     */

    if (mode == 1)
    {
        if (i > 1)
        {
            /*
             * 후보 노드가 여러 개일 경우, choice() 함수를 통해 사용자가 원하는 후보
             * (즉 몇 번째 후보를 기준으로 할지)를 결정한다.
             */

            int k = 0;  // 선택된 후보 인덱스

            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }

            // 선택한 후보(address[k])의 뒤에 새 데이터를 저장하는 노드를 삽입
            if (dlist_ins_next(list, address[k], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else if (i == 1)
        {
            // 후보가 1개이면, address[0]가 유일한 후보이므로 바로 사용
            if (dlist_ins_next(list, address[0], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else
        {
            /*
             * 후보가 하나이거나(또는 전혀 없거나) input 값이 존재하면,
             * 리스트를 순회하여 input과 일치하는 첫 번째 노드를 찾아 그 뒤에 삽입한다.
             */

            if (input != NULL)
            {
                while (current != NULL && list->match(current->data, input) == 0)
                    current = current->next;
                if (dlist_ins_next(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
            else
            {
                /* input이 NULL이면, 기본적으로 리스트의 head(또는 current) 뒤에 삽입 */
                if (dlist_ins_next(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
        }
        free(address);
        return 0;
    }

    /*
     * 모드 2: 기준 노드의 "앞"에 새 노드를 삽입하는 경우
     * (실제 구현에서는 dlist_ins_next()를 사용하므로 삽입 위치를 어떻게 처리할지
     *  약간의 추가 논리가 필요할 수 있음)
     */
    if (mode == 2)
    {
        if (i > 1)
        {
            /*
             * 후보 노드가 여러 개인 경우에는, choice() 함수를 통해 사용자가 선택한 후보를 사용한다.
             */
            int k = 0;
            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }
            // 선택된 후보 노드(address[k])의 앞(또는 적절한 위치)에 새 노드를 삽입
            if (dlist_ins_prev(list, address[k], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else if (i == 1)
        {
            // 후보가 1개이면, address[0]가 유일한 후보이므로 바로 사용
            if (dlist_ins_prev(list, address[0], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else
        {
            /*
             * 후보가 하나이거나 input이 존재하는 경우, 리스트 순회를 통해 기준 노드를 찾아
             * 해당 노드 앞(또는 적절한 위치)에 삽입한다.
             */
            if (input != NULL)
            {
                while (current != NULL && list->match(current->data, input) == 0)
                    current = current->next;
                if (dlist_ins_prev(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
            else
            {
                // input이 없는 경우 기본적으로 head(또는 current)를 대상으로 삽입
                if (dlist_ins_prev(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
        }
        
    }

    free(address);
    return 0;
}


/*
 * rem_node - 특정 데이터와 일치하는 노드를 삭제하는 함수
 * ----------------------------------------------------------------------------
 * 이 함수는 연결 리스트 내에서 입력받은 data와 일치하는 노드를 전부 검색하여 후보 배열(address)에
 * 저장한 후, 리스트의 전체 크기와 후보 개수에 따라 다음과 같이 삭제 대상을 결정한다.
 *
 *  1. 리스트의 크기가 3개 이상인 경우:
 *     - 후보가 여러 개(i > 1)일 경우, choice() 함수를 호출하여 사용자가 삭제할 후보의 인덱스를 선택하고
 *       해당 노드를 삭제한다.
 *     - 후보가 단 하나라면, while 루프를 통해 일치하는 첫 번째 노드를 찾아서 삭제한다.
 *
 *  2. 리스트의 크기가 2개 이하인 경우:
 *     - 리스트의 크기가 1이면 단순히 head 노드를 삭제한다.
 *     - 리스트의 크기가 2이면 while 루프를 통해 입력 data와 일치하는 노드를 검색하여 삭제한다.
 *
 * 매개변수:
 *   list - 노드를 삭제할 이중 연결 리스트의 포인터
 *   data - 삭제할 노드의 데이터와 비교할 값
 *
 * 반환값:
 *   삭제에 성공하면 0을, 실패하면 -1을 반환한다.
 */
int rem_node(DList* list, const void* data)
{
    // current: 삭제할 후보를 찾기 위한 순회 포인터
    // check: 전체 리스트를 순회하며 data와 일치하는 노드를 검사할 포인터
    DListElmt* current = list->head;
    DListElmt* check = list->head;

    // 후보 노드들을 저장할 배열 (리스트 크기만큼 할당)
    DListElmt** address = malloc(list->size * sizeof(DListElmt*));
    void* temp;
    int i = 0;  // 매칭된 후보의 개수를 저장할 변수

    // 메모리 할당 실패 시 에러 반환
    if (address == NULL)
    {
        // address가 NULL일 때 free() 호출은 불필요하지만, 안전하게 처리
        free(address);
        return -1;
    }

    // 전체 리스트를 순회하며, 입력 data와 일치하는 노드를 candidate 배열에 저장
    while (check != NULL)
    {
        // list->match() 함수가 일치하면 0을 반환한다고 가정
        if (list->match(check->data, data))
        {
            address[i] = check;
            i++;
        }
        check = check->next;
    }

    /* 분기: 리스트 전체 크기에 따라 삭제 대상 결정 */

    // 리스트의 크기가 2개 이상인 경우 처리
    if (list->size >= 2)
    {
        // 후보 노드가 여러 개일 경우: 사용자 입력(choice)을 통해 삭제할 인덱스 선택
        if (i >= 1)
        {
            int k = 0;

            if (choice(&k) != 1)  // 사용자가 올바른 선택을 하지 못하면 에러 처리
            {
                free(address);
                return -1;
            }

            // 선택된 후보(address[k])를 삭제
            if (dlist_remove(list, address[k], &temp) != 0)
            {
                free(address);
                return -1;
            }
        }
        // 후보가 한 개만 있을 경우: while 루프를 통해 첫 번째 일치하는 노드 탐색 후 삭제
        else
        {
            // !list->match() 조건은 match()가 0이면 false가 되므로,
            // 일치하는 노드를 찾기 위해 "match(...) != 0" 대신 "not(match(...))"를 사용해도 됨.
            // 여기서는 "while (current != NULL && !list->match(current->data, data))"로,
            // 일치하지 않는 노드를 건너뛰며, 일치하는 노드가 나오면 루프를 종료한다.
            while (current != NULL && !list->match(current->data, data))
                current = current->next;

            if (dlist_remove(list, current, &temp) != 0)
            {
                free(address);
                return -1;
            }
        }
    }
    // 리스트의 크기가 2개 이하인 경우 처리
    else
    {
        // 리스트 내 노드가 한 개이면 head를 단순히 삭제
        if (list->size == 1 && dlist_remove(list, list->head, &temp) != 0)
        {
            free(address);
            return -1;
        }
        else
        {
            // 리스트의 크기가 2인 경우: head와 tail 중 입력 data와 일치하는 노드를 찾음
            while (current != NULL && !list->match(current->data, data))
                current = current->next;

            if (dlist_remove(list, current, &temp) != 0)
            {
                free(address);
                return -1;
            }
        }
    }

    // 동적 할당한 후보 배열 메모리 해제 후 정상 종료
    free(address);
    return 0;
}

int choice(int* sequence) // 중복 데이터 선택 함수
{
    printf("몇 번째 중복 노드를 기준으로 하시겠습니까?\n");
    return scanf("%d", sequence);
} // 입력받은 값을 sequence에 저장하고, 반환값으로 입력 성공 여부를 반환합니다.
#include "set.h"

/*
 * set_init - 집합(Set) 초기화 함수
 *
 * 이 함수는 집합을 초기화 합니다.
 * 내부적으로 집합은 연결 리스트(List)를 기반으로 구성되어 있으므로,
 * list_init() 함수를 호출하여 리스트를 초기화하고,
 * 나아가 요소 비교에 사용할 사용자 정의 함수(match)를 설정합니다.
 *
 * Parameters:
 *   set     - 초기화할 집합 포인터
 *   match   - 두 데이터의 비교를 위한 함수 포인터 (동일하면 0 반환)
 *   destroy - 집합 소멸 시 데이터 해제에 사용할 함수 포인터 (사용하지 않을 경우 NULL)
 */
void set_init(Set* set,
    int (*match)(const void* key1, const void* key2),
    void (*destroy)(void* data))
{
    list_init(set, destroy);   // 연결 리스트 기반의 집합 초기화
    set->match = match;        // 데이터 비교에 사용할 함수를 설정
    return;
}

/*
 * set_insert - 집합에 데이터를 삽입하는 함수
 *
 * 집합에 중복 없이 데이터를 삽입합니다.
 * 만약 동일한 데이터가 이미 존재하면 삽입을 거부하고 1을 반환합니다.
 *
 * Parameters:
 *   set  - 데이터를 삽입할 집합 포인터
 *   data - 삽입할 데이터 포인터
 *
 * Returns:
 *   중복 데이터이면 1, 삽입 성공 시 0, 삽입 실패 시 -1을 반환
 */
int set_insert(Set* set, const void* data)
{
    // 데이터가 이미 존재하면 중복으로 간주하여 삽입하지 않음.
    if (set_is_member(set, data))
        return 1;

    // 연결 리스트의 꼬리(tail)에 데이터를 삽입함.
    return list_ins_next(set, list_tail(set), data);
}

/*
 * set_remove - 집합에서 특정 데이터를 삭제하는 함수
 *
 * 집합 내부의 연결 리스트를 순차 탐색하여,
 * 입력 받은 키와 일치하는 요소를 찾아 제거합니다.
 *
 * Parameters:
 *   set  - 데이터를 제거할 집합 포인터
 *   data - 삭제할 데이터의 키를 가리키는 포인터. 제거 후 데이터를 반환받을 수 있음.
 *
 * Returns:
 *   삭제 성공 시 0, 찾지 못한 경우 -1을 반환
 */
int set_remove(Set* set, void** data)
{
    ListElmt* member;
    ListElmt* prev = NULL;

    // 집합(연결 리스트)의 각 노드를 순차적으로 탐색
    for (member = list_head(set); member != NULL; member = member->next) {
        if (set->match(data, member->data))
            break;
        prev = member;
    }

    // 일치하는 요소가 없으면 에러 반환
    if (member == NULL)
        return -1;

    // 이전 노드(prev) 다음 요소를 제거하고 data에 저장
    return list_rem_next(set, prev, data);
}

/*
 * set_union - 두 집합의 합집합을 구성하는 함수
 *
 * set1과 set2의 모든 요소를 포함하는 합집합을 새 집합 setu에 생성합니다.
 * set1의 모든 요소는 무조건 합집합에 포함되며,
 * set2의 요소 중 set1에 없는 요소만 추가합니다.
 *
 * Parameters:
 *   setu - 합집합 결과를 저장할 집합 포인터
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   성공 시 0, 데이터 삽입 실패 시 setu를 해제 후 -1 반환
 */
int set_union(Set* setu, const Set* set1, const Set* set2)
{
    ListElmt* member;
    void* data;

    // 합집합 초기화 (set1의 match 함수 사용)
    set_init(setu, set1->match, NULL);

    // set1의 모든 요소를 합집합에 삽입
    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        data = list_data(member);
        if (list_ins_next(setu, list_tail(setu), data) != 0) {
            set_destroy(setu);
            return -1;
        }
    }

    // set2의 요소 중 set1에 없는 것만 합집합에 삽입
    for (member = list_head(set2); member != NULL; member = list_next(member)) {
        if (set_is_member(set1, list_data(member)))
            continue;
        else {
            data = list_data(member);
            if (list_ins_next(setu, list_tail(setu), data) != 0) {
                set_destroy(setu);
                return -1;
            }
        }
    }
    return 0;
}

/*
 * set_intersection - 두 집합의 교집합을 구성하는 함수
 *
 * set1과 set2 모두에 존재하는 요소들만을 모아 새 집합 seti에 저장합니다.
 *
 * Parameters:
 *   seti - 교집합 결과를 저장할 집합 포인터
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   성공 시 0, 데이터 삽입 실패 시 seti를 해제 후 -1 반환
 */
int set_intersection(Set* seti, const Set* set1, const Set* set2)
{
    ListElmt* member;
    void* data;

    // 교집합 집합 초기화 (set1의 match 함수 사용)
    set_init(seti, set1->match, NULL);

    // set1의 각 요소가 set2에도 존재하는지 확인
    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        if (set_is_member(set2, member->data) != 0) {
            data = member->data;
            if (list_ins_next(seti, list_tail(seti), data) != 0) {
                set_destroy(seti);
                return -1;
            }
        }
    }
    return 0;
}

/*
 * set_difference - 집합 차집합 (set1 - set2)을 구성하는 함수
 *
 * set1에 존재하지만 set2에는 없는 요소들을 찾아 새 집합 setd에 저장합니다.
 *
 * Parameters:
 *   setd - 차집합 결과를 저장할 집합 포인터
 *   set1 - 기준 집합 포인터
 *   set2 - 비교 집합 포인터
 *
 * Returns:
 *   성공 시 0, 삽입 실패 시 setd를 해제 후 -1 반환
 */
int set_difference(Set* setd, const Set* set1, const Set* set2)
{
    ListElmt* member;
    void* data;

    // 차집합 초기화 (set1의 match 함수 사용)
    set_init(setd, set1->match, NULL);

    // set1의 각 요소가 set2에 없는 경우에만 차집합에 삽입
    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        if (!set_is_member(set2, member->data)) {
            data = member->data;
            if (list_ins_next(setd, list_tail(setd), data) != 0) {
                set_destroy(setd);
                return -1;
            }
        }
    }
    return 0;
}

/*
 * set_is_member - 데이터가 집합 내에 존재하는지 확인하는 함수
 *
 * 집합의 모든 요소를 순회하며, 입력 데이터와 동일한 요소가 있는지 비교합니다.
 * 비교는 집합의 match 함수로 수행합니다.
 *
 * Parameters:
 *   set  - 검색할 집합 포인터
 *   data - 검색할 데이터 포인터
 *
 * Returns:
 *   데이터가 존재하면 1, 존재하지 않으면 0을 반환
 */
int set_is_member(const Set* set, const void* data)
{
    ListElmt* member;

    for (member = list_head(set); member != NULL; member = member->next) {
        if (set->match(data, member->data) == 0)
            return 1;
    }
    return 0;
}

/*
 * set_is_subset - 한 집합이 다른 집합의 부분집합인지 검사하는 함수
 *
 * set1의 모든 요소가 set2에도 존재하면 set1은 set2의 부분집합입니다.
 * 단, set1의 크기가 set2보다 클 경우 바로 부분집합이 아님을 반환합니다.
 *
 * Parameters:
 *   set1 - 부분집합 여부를 검사할 집합
 *   set2 - 기준 집합
 *
 * Returns:
 *   set1이 set2의 부분집합이면 1, 아니면 0
 */
int set_is_subset(const Set* set1, const Set* set2)
{
    ListElmt* member;

    // set1이 set2보다 크면 set1은 절대 부분집합이 될 수 없음.
    if (list_size(set1) > list_size(set2))
        return 0;

    // set1의 각 요소가 set2에도 존재하는지 확인
    for (member = list_head(set1); member != NULL; member = member->next) {
        if (!set_is_member(set2, member->data))
            return 0;
    }
    return 1;
}

/*
 * set_is_equal - 두 집합이 동일한지 검사하는 함수
 *
 * 두 집합은 크기가 같고, 서로의 부분집합 관계일 때 동일하다고 판단합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합
 *   set2 - 두 번째 집합
 *
 * Returns:
 *   두 집합이 동일하면 1, 아니면 0
 */
int set_is_equal(const Set* set1, const Set* set2)
{
    // 크기가 다르면 동일할 수 없음.
    if (set1->size != set2->size)
        return 0;

    // set1이 set2의 부분집합이면 두 집합은 동일함.
    return set_is_subset(set1, set2);
}

/*
 * set_ins - 사용자로부터 입력 받아 두 집합 중 하나에 데이터를 삽입하는 함수
 *
 * 사용자가 선택한 집합에 대해 문자열 데이터를 입력받아 삽입합니다.
 * 삽입하려는 데이터가 이미 존재할 경우 에러를 출력하고 데이터 메모리를 해제합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   성공 시 0, 오류 발생 시 -1
 */
int set_ins(Set* set1, Set* set2)
{
    int choice = 0;
    char* string = malloc(CHAR_MAX);   // 입력 받을 문자열 메모리 할당

    if (string == NULL)
        return -1;

    printf("집합을 선택하세요\n1. 첫 번째 집합\t2. 두 번째 집합: ");
    scanf("%d", &choice);

    if (choice == 1) {  // 첫 번째 집합 선택 시
        printf("삽입할 데이터를 입력하세요: ");
        scanf("%s", string);
        if (set_insert(set1, string) != 0) {
            printf("중복된 데이터가 있습니다.\n");
            free(string);
            return -1;
        }
    }
    else if (choice == 2) {  // 두 번째 집합 선택 시
        printf("삽입할 데이터를 입력하세요: ");
        scanf("%s", string);
        if (set_insert(set2, string) != 0) {
            printf("중복된 데이터가 있습니다.\n");
            free(string);
            return -1;
        }
    }
    else {
        // 잘못된 선택인 경우
        printf("잘못된 선택입니다.\n");
        free(string);
        return -1;
    }

    return 0;
}

/*
 * set_rem - 사용자 입력을 받아 집합에서 특정 데이터를 삭제하는 함수
 *
 * 사용자가 선택한 집합에서 삭제할 데이터를 입력받아 해당 데이터를 제거합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   삭제 성공 시 0, 오류 발생 시 -1
 */
int set_rem(Set* set1, Set* set2)
{
    int choice = 0;
    char* string = malloc(CHAR_MAX);

    if (string == NULL)
        return -1;

    printf("집합을 선택하세요\n1. 첫 번째 집합\t2. 두 번째 집합: ");
    scanf("%d", &choice);

    printf("삭제할 데이터를 입력하세요: ");
    scanf("%s", string);

    if (choice == 1) {
        if (set_remove(set1, string) != 0) {
            free(string);
            return -1;
        }
    }
    else if (choice == 2) {
        if (set_remove(set2, string) != 0) {
            free(string);
            return -1;
        }
    }
    else {
        printf("잘못된 선택입니다.\n");
        free(string);
        return -1;
    }

    free(string);
    return 0;
}

/*
 * printSet - 선택한 집합의 모든 요소를 출력하는 함수
 *
 * 사용자에게 출력할 집합(첫 번째 또는 두 번째)을 선택하도록 요청한 후,
 * 선택된 집합의 각 노드를 순회하며 데이터와 노드 주소를 출력합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 */
void printSet(Set* set1, Set* set2)
{
    ListElmt* member;
    int choice = 0;

    // 올바른 집합 번호(1 또는 2)가 입력될 때까지 반복
    do {
        printf("집합을 선택하세요\n1. 첫 번째 집합\t2. 두 번째 집합: ");
        scanf("%d", &choice);
        if (choice != 1 && choice != 2)
            printf("잘못된 선택입니다. 다시 입력하세요.\n");
    } while (choice != 1 && choice != 2);

    if (choice == 1) {  // 첫 번째 집합 선택
        member = set1->head;
        if (list_head(set1) == NULL)
            printf("첫 번째 집합이 비어있습니다.\n");
        else {
            while (member != NULL) {
                printf("노드 데이터 : %s, 노드 주소 : %p\n", list_data(member), member);
                member = member->next;
            }
        }
    }
    else if (choice == 2) {  // 두 번째 집합 선택
        member = set2->head;
        if (list_head(set2) == NULL)
            printf("두 번째 집합이 비어있습니다.\n");
        else {
            while (member != NULL) {
                printf("노드 데이터 : %s, 노드 주소 : %p\n", list_data(member), member);
                member = member->next;
            }
        }
    }
}

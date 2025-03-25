#include "chtbl.h"

/*
 * 해시 테이블 초기화 함수
 * - htbl: 해시 테이블 구조체에 대한 포인터
 * - buckets: 해시 테이블에서 사용할 버킷(리스트)의 수
 * - h: 해시 함수 (key를 받아 정수 값을 반환)
 * - match: 두 key의 일치를 비교하는 함수 (일치하면 0 반환)
 * - destroy: 데이터 삭제 시 호출할 함수 (NULL이면 삭제 안함)
 *
 * 반환값:
 *   0  - 초기화 성공
 *  -1  - 메모리 할당 실패 등 초기화 실패
 */
int chtbl_init(CHTbl* htbl, int buckets, int (*h)(const void* key),
    int (*match)(const void* key1, const void* key2),
    void (*destroy)(void* data))
{
    // 해시 테이블의 버킷 배열(List 배열)을 할당
    if ((htbl->table = (List*)malloc(buckets * sizeof(List))) == NULL)
        return -1;

    // 각 버킷(연결 리스트)를 초기화 (빈 리스트로 시작)
    for (int i = 0; i < buckets; i++)
    {
        list_init(&htbl->table[i], destroy);
    }

    // 해시 테이블 메타데이터 설정
    htbl->buckets = buckets;  // 총 버킷 수 기록
    htbl->h = h;              // 해시 함수 지정
    htbl->match = match;      // 데이터 비교 함수 지정
    htbl->destroy = destroy;  // 데이터 삭제 시 호출할 함수 지정
    htbl->size = 0;           // 현재 데이터 개수는 0으로 초기화

    return 0;
}

/*
 * 해시 테이블 전체 삭제 함수
 * - htbl: 삭제할 해시 테이블 구조체에 대한 포인터
 *
 * 각 버킷에 있는 모든 노드를 제거하고, 할당한 버킷 배열과
 * 해시 테이블 구조체의 내용을 초기화합니다.
 */
void chtbl_destroy(CHTbl* htbl)
{
    // 모든 버킷(리스트)을 순회하며 각 리스트의 노드를 제거
    for (int i = 0; i < htbl->buckets; i++)
    {
        list_destroy(&htbl->table[i]);
    }
    // 할당된 버킷 배열 메모리 해제
    free(htbl->table);

    // 해시 테이블 구조체 자체를 0으로 초기화 (안전하게 만듦)
    memset(htbl, 0, sizeof(CHTbl));
    return;
}

/*
 * 해시 테이블에 데이터 삽입 함수
 * - htbl: 데이터를 삽입할 해시 테이블 구조체 포인터
 * - data: 삽입할 데이터(void*로 되어 있으므로 임의의 데이터 가능)
 *
 * 삽입 전에 중복 데이터 여부를 체크하고, 해당하는 버킷에 연결 리스트 삽입 함수인
 * list_ins_next를 호출하여 데이터를 추가합니다.
 *
 * 반환값:
 *   0  - 삽입 성공
 *   1  - 중복 데이터 발견 (삽입하지 않음)
 *  -1  - 삽입에 실패
 */
int chtbl_insert(CHTbl* htbl, const void* data)
{
    int bucket;
    int retval;

    // 임시 변수 temp를 사용하여 중복 체크를 위한 포인터 전달
    void* temp = (void*)data;

    // 데이터 중복 확인: 이미 존재하면 삽입하지 않음
    if (chtbl_lookup(htbl, &temp) == 0)
        return 1;

    // 데이터가 들어갈 버킷 계산 (해시 함수 결과를 버킷 총 수로 나눈 나머지)
    bucket = htbl->h(data) % htbl->buckets;

    // 해당 버킷의 리스트에 데이터를 삽입 (NULL을 인자로 전달하면 리스트의 맨 앞에 삽입)
    if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
        htbl->size++;   // 삽입 성공 시 해시 테이블의 데이터 개수 증가

    return retval;
}

/*
 * 해시 테이블에서 데이터 제거 함수
 * - htbl: 데이터를 제거할 해시 테이블 구조체 포인터
 * - data: 제거할 데이터의 포인터에 대한 주소를 전달 (제거된 데이터의 주소로 변경될 수 있음)
 *
 * 지정한 데이터가 저장된 버킷에서 연결 리스트를 순회하며 데이터 일치 여부를 체크하여,
 * 일치하는 노드를 list_rem_next 함수를 통해 제거합니다.
 *
 * 반환값:
 *   0  - 제거 성공
 *  -1  - 해당 데이터를 찾을 수 없음이나 제거 실패
 */
int chtbl_remove(CHTbl* htbl, void** data)
{
    ListElmt* element;
    ListElmt* prev = NULL;
    int bucket;

    // 제거할 데이터가 저장되어 있을 버킷 계산
    bucket = htbl->h(*data) % htbl->buckets;

    // 해당 버킷의 리스트를 순회하며 제거 대상 노드를 찾음
    for (element = list_head(&htbl->table[bucket]); element != NULL; element = element->next)
    {
        // match 함수가 0이면 두 데이터가 동일함
        if (htbl->match(*data, element->data) == 0)
        {
            // list_rem_next를 호출하여 해당 노드를 제거
            if (list_rem_next(&htbl->table[bucket], prev, data) == 0)
            {
                htbl->size--;  // 노드 제거 성공 시 전체 크기를 감소
                return 0;
            }
            else
                return -1; // list_rem_next 실패 시
        }
        prev = element;
    }
    return -1; // 지정한 데이터를 찾지 못한 경우
}

/*
 * 해시 테이블에서 데이터 조회 함수
 * - htbl: 조회할 해시 테이블 구조체 포인터
 * - data: 조회하려는 데이터의 포인터에 대한 주소. 일치하는 데이터가 있는 경우,
 *         해당 노드 데이터의 주소로 변경됨.
 *
 * 반환값:
 *   0  - 조회 성공 (데이터가 존재)
 *  -1  - 데이터 미발견
 */
int chtbl_lookup(const CHTbl* htbl, void** data)
{
    ListElmt* element;
    int bucket;

    // 조회할 데이터가 저장되어 있을 버킷 계산
    bucket = htbl->h(*data) % htbl->buckets;

    // 해당 버킷의 리스트를 순회하며 데이터 일치 여부 검증
    for (element = list_head(&htbl->table[bucket]); element != NULL; element = element->next)
    {
        // 데이터 일치 (match 함수가 0 반환 시)
        if (htbl->match(*data, element->data) == 0)
        {
            *data = element->data;  // 일치하는 데이터를 data 포인터에 저장
            return 0;
        }
    }
    return -1;  // 데이터가 존재하지 않으면 -1 반환
}

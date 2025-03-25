#include "chtbl.h"                           // 해시 테이블 관련 함수와 구조체 선언 헤더
#define CHAR_MAX 100 * (sizeof(char))          // 문자열 입력을 위한 최대 크기 (100바이트)

// 두 문자열(key1, key2)이 동일한지 비교하는 함수 
// strcmp는 두 문자열이 같으면 0을 반환하므로 이를 그대로 사용
int match(const char* key1, const char* key2)
{
    return strcmp(key1, key2);
}

// 문자열 값을 균등하게 분산시키기 위한 해시 함수
// 입력받은 key를 기반으로 해시 값을 계산하여 반환
int hash_Func(const void* key) // 문자열 균등 해시 함수
{
    const char* ptr;      // key에 대한 char 포인터
    int val;              // 해시 값

    val = 0;
    ptr = key;            // key를 문자열로 처리

    // 문자열의 끝 ('\0')까지 순회하면서 해시 값을 계산
    while (*ptr != '\0')
    {
        int tmp;

        // 기존 해시 값에 현재 문자의 값을 더하면서 왼쪽으로 4비트 시프트
        val = (val << 4) + (*ptr);

        // 상위 4비트가 채워진 경우 이를 처리하여 값의 분포를 고르게 함
        if (tmp = (val & 0xf0000000))
        {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }
        ptr++;  // 다음 문자로 이동
    }
    return val;
}

int main(void)
{
    // 해시 테이블 구조체를 위한 메모리 할당
    CHTbl* htbl = malloc(sizeof(CHTbl));
    int bucket = 0; // 사용자가 입력할 버킷 수

    // 메모리 할당 실패 시 프로그램 종료
    if (htbl == NULL)
        return -1;

    // 해시 테이블의 버킷 수를 사용자로부터 입력 받음
    printf("해시 테이블의 버킷의 수를 입력하세요: ");
    scanf("%d", &bucket);

    // 해시 테이블 초기화: 할당된 버킷 수, 해시 함수, 비교 함수, 삭제 함수를 전달
    if (chtbl_init(htbl, bucket, hash_Func, match, NULL) != 0)
        return -1;

    // 메인 루프로 계속해서 사용자의 입력을 처리
    while (1)
    {
        int mode = 0;

        // 사용자 메뉴 출력
        printf("1.데이터 삽입\t2.데이터 출력\t3.데이터 제거\t4.데이터 전체 출력\t5.종료\n");
        scanf("%d", &mode);

        // 모드 1: 데이터 삽입
        if (mode == 1)
        {
            // 데이터 삽입을 위한 메모리 할당
            char* string = malloc(CHAR_MAX);

            printf("데이터를 입력하세요 : ");
            scanf("%s", string);

            // 데이터 삽입 시 중복 체크 후 삽입, 실패 시 메모리 해제 후 종료
            if (chtbl_insert(htbl, string) != 0)
            {
                free(string);
                return -1;
            }
        }
        // 모드 2: 개별 데이터 조회 (출력)
        else if (mode == 2)
        {
            // 검색할 문자열을 위한 메모리 할당
            char* find = malloc(CHAR_MAX);
            void* data;  // lookup 결과를 저장할 포인터

            printf("찾고자 하는 데이터를 입력하세요 : ");
            scanf("%s", find);

            // 검색 시작: data에 찾고자 하는 문자열의 주소 할당 후 lookup 함수 호출
            data = find;
            if (chtbl_lookup(htbl, &data) == 0)
                // lookup 성공 시, 해당 노드의 주소와 데이터를 출력
                printf("노드 주소: %p, 노드 데이터: %s\n", data, (char*)data);
            else
                // lookup 실패 시 해당 메시지 출력
                printf("해당 데이터 노드를 찾을 수 없습니다.\n");

            free(find); // 검색을 위한 임시 메모리 해제
        }
        // 모드 3: 데이터 제거
        else if (mode == 3)
        {
            // 삭제할 데이터를 입력받기 위해 임시 메모리 할당
            char* remove = malloc(CHAR_MAX);
            void* data;  // 삭제할 데이터의 주소를 저장할 포인터

            printf("삭제하고자 하는 데이터를 입력하세요 : ");
            scanf("%s", remove);
            data = remove;

            // 삭제 함수 호출, 실패 시 메시지 출력
            if (chtbl_remove(htbl, &data) != 0)
                printf("해당 데이터 노드를 찾을 수 없습니다.\n");

            free(remove); // 삭제를 위한 임시 메모리 해제
        }
        // 모드 4: 전체 데이터 출력 (해시 테이블 전체 출력)
        else if (mode == 4)
        {
            int i;
            ListElmt* member;

            // 모든 버킷을 순회하며 각 버킷의 연결 리스트에 저장된 노드 출력
            for (i = 0; i < htbl->buckets; i++)
            {
                printf("버킷 %d:\n", i);
                for (member = list_head(&htbl->table[i]); member != NULL; member = member->next)
                {
                    // 각 노드의 주소, 데이터, 다음 노드의 주소 출력
                    printf("\t노드 주소: %p, 노드 데이터: %s, 다음 노드 주소: %p\n", member, member->data, member->next);
                }
            }
        }
        // 모드 5: 종료
        else if (mode == 5)
            break;

        // 그 외의 입력은 다시 반복문으로 보내어 무시
        else
            continue;
    }

    // 해시 테이블 구조체에 할당된 메모리 해제 
    free(htbl);
    return 0;
}

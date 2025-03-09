#include "list.h"

#define CHAR_MAX (100 * sizeof(char))

/*
 * match: 두 문자열 비교 함수
 * 두 문자열이 같으면 1, 다르면 0을 반환한다.
 */
int match(const void* key1, const void* key2)
{
    if (strcmp((const char*)key1, (const char*)key2) == 0)
        return 1;
    else
        return 0;
}

int main(void) // 프로그램: 연결 리스트를 사용하여 문자열 데이터를 조작하고 테스트한다.
{
    int option = 0;

    /* 동적 할당 및 초기화 */
    List* link_list = malloc(sizeof(List)); // 연결 리스트 생성
    list_init(link_list, NULL);              // 연결 리스트 초기화
    link_list->match = &match;               // 비교 함수를 연결 리스트에 등록

    while (1) // 메뉴 기반 무한 루프
    {
        /* 연결 리스트 테스트를 위한 메뉴
         * 1. 연결리스트 노드 삽입
         * 2. 연결리스트 노드 삭제
         * 3. 연결리스트 전체 출력
         * 4. 프로그램 종료
         */
        printf("1.연결리스트 노드 삽입\t2.연결리스트 노드 삭제\t3.연결리스트 전체 출력\t4.프로그램 종료\n");
        scanf("%d", &option);

        if (option == 1)  // [옵션 1] 연결 리스트에 새 노드 삽입
        {
            char* string = malloc(CHAR_MAX);    // 삽입할 데이터 버퍼
            char* reference = malloc(CHAR_MAX);   // 기준 노드 데이터 버퍼

            printf("삽입하고자 하는 노드 데이터를 입력하세요: ");
            scanf("%s", string);

            if (link_list->size == 1)
            {
                /* 노드가 1개인 경우에는 기준 노드가 필요없으므로 미리 할당한 reference 해제 */
                free(reference);
                reference = NULL;
            }
            else if (link_list->size >= 2)
            {
                printf("기준 노드의 데이터를 입력하세요: ");
                scanf("%s", reference);
            }

            if (add_node(link_list, string, reference) != 0)
            {
                list_destroy(link_list);
                free(link_list);
                return -1;
            }
            /* 참고: 만약 add_node()가 입력 받은 string의 소유권을 연결 리스트에 넘기도록 설계되었다면,
             * 여기서 별도로 string을 free하면 안된다. (메모리 해제 정책에 주의)
             */
        }
        else if (option == 2)  // [옵션 2] 연결 리스트에서 노드 삭제
        {
            char* string = malloc(CHAR_MAX);

            printf("삭제하고자 하는 노드의 데이터를 입력하세요: ");
            scanf("%s", string);

            if (rem_node(link_list, string) != 0)
            {
                free(string);
                list_destroy(link_list);
                free(link_list);
                return -1;
            }
            free(string);  // 사용 후 동적 할당 버퍼 해제
        }
        else if (option == 3)  // [옵션 3] 연결 리스트 전체 출력
        {
            ListElmt* member = link_list->head;
            if (member == NULL)
            {
                printf("연결 리스트가 비어있습니다.\n");
            }
            else
            {
                while (member != NULL)
                {
                    printf("노드 주소: %p\n노드 데이터: %s\n다음 노드 주소: %p\n",
                        member, (char*)member->data, member->next);
                    member = member->next;
                }
            }
        }
        else if (option == 4)  // [옵션 4] 프로그램 종료
        {
            break;
        }
    }

    /* 프로그램 종료 전, 연결 리스트의 모든 자원 해제 */
    list_destroy(link_list);
    free(link_list);

    return 0;
}

#include "dlist.h"

#define CHAR_MAX (100 * sizeof(char))

// 두 키 값을 비교하는 함수
int match(const void* key1, const void* key2)
{
    if (strcmp((const char*)key1, (const char*)key2) == 0)
        return 1; // 일치하면 1 반환
    else
        return 0; // 일치하지 않으면 0 반환
}

int main(void)
{
    int mode = 0;
    // 이중 연결 리스트를 위한 메모리 할당
    DList* list = malloc(sizeof(DList));

    // 메모리 할당 실패 여부 확인
    if (list == NULL)
    {
        printf("리스트를 위한 메모리 할당에 실패했습니다.\n");
        return -1;
    }

    dlist_init(list, NULL); // 리스트 초기화
    list->match = &match;   // match 함수 지정

    while (1)
    {
        printf("이중 연결리스트\n1.리스트 노드 추가  2.리스트 노드 제거  3.리스트 전체 출력  4.노드 전체 삭제\t5.종료\n");
        scanf("%d", &mode);

        if (mode == 1)
        {
            char* string = malloc(CHAR_MAX);
            char* reference = malloc(CHAR_MAX);
            int option = 0;

            // 메모리 할당 실패 확인
            if (string == NULL || reference == NULL)
            {
                printf("문자열을 위한 메모리 할당에 실패했습니다.\n");
                free(string);
                free(reference);
                continue;
            }

            printf("추가할 노드의 데이터를 입력하세요 :");
            scanf("%s", string);

            if (list->size == 1)
            {
                /* 노드가 하나라면 기준 노드 메모리를 해제 */
                free(reference);
                reference = NULL;
            }
            else if (list->size >= 2)
            {
                printf("기준 노드의 데이터를 입력하세요 :");
                scanf("%s", reference);
            }

            if (list->size >= 1)
            {
                printf("1 : 기준 노드 뒤에 노드 생성\t2.기준 노드 앞에 노드 생성\n");
                scanf("%d", &option);
            }
            else
            {
                option = 1;
            }

            // add_node 함수 실행 결과 확인
            if (add_node(list, string, reference, option) != 0)
            {
                printf("노드 추가에 실패했습니다.\n");
                free(string);
                free(reference);
                free(list);
                return -1;
            }
        }

        if (mode == 2)
        {
            char* string = malloc(CHAR_MAX);

            if (string == NULL) // 메모리 할당 실패 확인
            {
                printf("문자열을 위한 메모리 할당에 실패했습니다.\n");
                continue;
            }

            printf("삭제 하고자 하는 노드의 데이터를 입력하세요 : ");
            scanf("%s", string);

            if (rem_node(list, string) != 0)
            {
                printf("노드 제거에 실패했습니다.\n");
                free(string);
                free(list);
                return -1;
            }

            free(string); // 메모리 해제
        }

        if (mode == 3)
        {
            DListElmt* member = list->head;
            if (list->size == 0)
                printf("이중 연결리스트가 비어있습니다.\n");
            else
            {
                while (member != NULL)
                {
                    printf("이전 노드 주소 : %p\n노드 주소 : %p\n노드 데이터 : %s\n다음 노드 주소 : %p\n\n",
                        member->prev, member, (char*)member->data, member->next);
                    member = member->next;
                }
            }
        }

        if (mode == 4)
        {
            dlist_destroy(list); // 모든 노드 삭제
            continue;
        }

        if (mode == 5)
            break;
    }

    dlist_destroy(list); // 리스트 정리
    free(list); // 리스트 메모리 해제
    return 0;
}

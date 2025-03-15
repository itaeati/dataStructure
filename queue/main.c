#include "queue.h"   // 큐 자료구조 정의; 내부적으로 연결 리스트(List)를 기반으로 구현됨.

#define CHAR_MAX (100 * sizeof(char))  // 100개의 char 공간(바이트 단위) 확보

/*
 * match: 두 문자열 비교 함수
 * --------------------------------------------------------
 * 설명:
 *   두 문자열(key1과 key2)이 완전히 동일한지 검사합니다.
 *   문자열이 동일하면 1, 그렇지 않으면 0을 반환합니다.
 *
 * 매개변수:
 *   key1, key2: 비교할 두 문자열을 가리키는 포인터(void*).
 *              실제로는 char* 타입의 문자열이 전달됩니다.
 *
 * 반환값:
 *   문자열이 같으면 1, 아니면 0.
 */
int match(const void* key1, const void* key2)
{
    return (strcmp((const char*)key1, (const char*)key2) == 0) ? 1 : 0;
}

int main(void)
{
    int option = 0;  // 메뉴에서 사용자가 선택한 옵션을 저장할 변수

    /* 큐 생성 및 초기화 */
    // 동적 메모리를 할당하여 큐 구조체(내부적으로 연결 리스트)를 생성합니다.
    Queue* queue = malloc(sizeof(Queue));
    if (queue == NULL) 
    {
        perror("메모리 할당 실패");
        return -1;
    }
    // queue_init은 내부적으로 list_init을 호출하여 큐를 초기화합니다.
    queue_init(queue, NULL);
    // 필요하다면, 큐의 비교함수를 설정할 수 있습니다.
    queue->match = &match;

    while (1)  // 메뉴 기반 무한 루프
    {
        // 메뉴 출력
        printf("1. 큐 삽입\t2. 큐 꺼내기\t3. 큐 전체 출력\t4. 프로그램 종료\n");
        scanf("%d", &option);

        if (option == 1)  /* [옵션 1] 큐 삽입 (enqueue) */
        {
            // 사용자 입력을 받을 데이터를 위한 버퍼를 동적 할당합니다.
            char* data = malloc(CHAR_MAX);
            if (data == NULL) 
            {
                perror("메모리 할당 실패");
                continue;
            }
            printf("큐에 삽입할 데이터를 입력하세요: ");
            scanf("%s", data);

            // queue_enqueue는 큐의 후단(즉, tail)에 데이터를 삽입합니다.
            if (queue_enqueue(queue, data) != 0) 
            {
                // 삽입 실패 시, 할당한 메모리를 해제합니다.
                free(data);
                printf("큐 삽입 실패\n");
            }
            else 
            {
                printf("데이터가 큐에 삽입되었습니다.\n");
            }
            // 삽입에 성공하면, 데이터의 소유권은 큐로 이전되므로 여기서 free하지 않습니다.
        }
        else if (option == 2)  /* [옵션 2] 큐 꺼내기 (dequeue) */
        {
            char* data = NULL;
            // queue_dequeue 함수는 큐의 전단(head)에서 데이터를 제거하고, 제거한 데이터를 반환합니다.
            if (queue_dequeue(queue, (void**)&data) != 0) 
            {
                printf("큐가 비어있습니다.\n");
            }
            else 
            {
                printf("큐에서 꺼낸 데이터: %s\n", data);
                free(data);  // 제거한 데이터는 더 이상 사용하지 않으므로 해제합니다.
            }
        }
        else if (option == 3)  /* [옵션 3] 큐 전체 출력 */
        {
            // 큐는 내부적으로 연결 리스트(List)로 구현되어 있으므로, head부터 순회합니다.
            ListElmt* member = queue->head;

            if (member == NULL) 
            {
                printf("큐는 비어있습니다.\n");
            }

            else 
            {
                printf("\n-- 큐 전체 출력 --\n");
                while (member != NULL) {
                    printf("노드 데이터: %s\n노드 주소: %p\n다음 노드 주소: %p\n", (char*)member->data, member, member->next);
                    member = member->next;
                }
            }

        }

        else if (option == 4)  /* [옵션 4] 프로그램 종료 */
        {
            break;
        }

        else 
        {
            // 유효하지 않은 옵션 입력 시 메시지 출력
            printf("잘못된 옵션입니다.\n");
        }
    }

    /* 프로그램 종료 전, 큐에 할당된 모든 자원을 해제 */
    queue_destroy(queue);
    free(queue);

    return 0;
}

#include "stack.h"


#define CHAR_MAX (100 * sizeof(char))

/*
 * match: 두 문자열 비교 함수
 * --------------------------------------------------------
 * 설명:
 *   두 문자열(key1과 key2)이 완전히 동일한지 검사합니다.
 *   문자열 비교에는 strcmp() 함수를 사용하며,
 *   두 문자열이 동일하면 1, 그렇지 않으면 0을 반환합니다.
 *
 * 매개변수:
 *   key1, key2: 비교할 두 문자열을 가리키는 const void* 포인터.
 *              실제로는 char* 타입의 문자열이 전달됩니다.
 *
 * 반환값:
 *   문자열이 동일하면 1, 아니면 0.
 */
int match(const void* key1, const void* key2)
{
    return (strcmp((const char*)key1, (const char*)key2) == 0) ? 1 : 0;
}

int main(void)
{
    int option = 0;  // 사용자로부터 입력받은 메뉴 옵션을 저장할 변수

    /*
     * 스택 생성 및 초기화
     * -------------------------------
     * 스택은 연결리스트 기반으로 구현되어 있습니다.
     * 먼저 Stack 구조체 크기만큼 동적 메모리 할당을 받고,
     * list_init 함수를 통해 초기화합니다.
     */
    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL)
    {
        perror("메모리 할당 실패");
        return -1;
    }
    // list_init 함수는 Stack을 내부적으로 List처럼 초기화합니다.
    // 두 번째 인자는 데이터 복사 혹은 소멸 함수 포인터로 사용될 수 있지만, 여기서는 NULL을 전달.
    list_init(stack, NULL);

    /*
     * 메뉴 기반 무한 루프 시작
     * 사용자가 옵션을 선택하면 해당 기능(푸쉬, 팝, 전체 출력, 종료)을 실행합니다.
     */
    while (1)
    {
        // 메뉴를 출력하여 사용자가 선택할 수 있도록 함
        printf("1. 스택 푸쉬\t2. 스택 팝\t3. 스택 전체 출력\t4. 프로그램 종료\n");
        printf("옵션을 선택하세요: ");
        scanf("%d", &option);  // 사용자 입력을 받아 option 변수에 저장

        if (option == 1)  /* [옵션 1] 스택 푸쉬: 사용자로부터 데이터를 입력받아 스택에 저장 */
        {
            // 사용자가 입력할 데이터를 위한 버퍼를 동적 할당.
            char* data = malloc(CHAR_MAX);
            if (data == NULL)
            {
                perror("메모리 할당 실패");
                continue; // 할당 실패 시, 반복문의 처음으로 돌아감
            }

            printf("스택에 푸쉬할 데이터를 입력하세요: ");
            scanf("%s", data);  // 사용자로부터 문자열 데이터를 입력 받음

            // stack_push() 함수는 스택(연결리스트)에 데이터를 추가하는 함수입니다.
            // 만약 반환값이 0이 아니라면, 푸쉬에 실패한 것으로 간주합니다.
            if (stack_push(stack, data) != 0)
            {
                free(data);  // 푸쉬에 실패한 경우, 데이터 버퍼를 해제하여 메모리 누수를 방지
                printf("스택 푸쉬 실패.\n");
            }
            else
            {
                printf("데이터가 스택에 푸쉬되었습니다.\n");
            }
            // 성공적으로 푸쉬되면, 데이터의 소유권은 스택으로 이전되었으므로 여기서 따로 free 하지 않음.
        }
        else if (option == 2)  /* [옵션 2] 스택 팝: 스택에서 데이터를 꺼내 출력 */
        {
            char* data = NULL;  // 팝된 데이터를 저장할 포인터 초기화

            // stack_pop() 함수는 스택에서 데이터를 꺼내 (팝) 가져오는 함수입니다.
            // 스택이 비어있으면 오류 코드(nonzero)를 반환합니다.
            if (stack_pop(stack, (void**)&data) != 0)
            {
                printf("스택이 비어있습니다.\n");
            }
            else
            {
                // 데이터를 정상적으로 팝한 경우,
                printf("스택에서 팝된 데이터: %s\n", data);
                free(data);  // 팝된 데이터는 사용 후 해제합니다.
            }
        }
        else if (option == 3)  /* [옵션 3] 스택 전체 출력: 스택의 내부 연결리스트를 순회하며 모든 데이터를 출력 */
        {
            // 스택 내부의 노드들은 연결리스트(List)로 관리되므로, 스택 헤드 포인터를 가져옴.
            ListElmt* member = stack->head;
            if (member == NULL)
            {
                printf("스택은 비어있습니다.\n");
            }
            else
            {
                // 연결리스트의 각 노드를 순회하면서 데이터와 노드 주소들을 출력합니다.
                while (member != NULL)
                {
                    printf("노드 데이터: %s\n노드 주소 : %p\n다음 노드 주소 : %p\n",
                        (char*)member->data, member, member->next);
                    member = member->next;
                }
            }
        }
        else if (option == 4)  /* [옵션 4] 프로그램 종료 */
        {
            break;  // while문을 빠져나와 프로그램 종료
        }
        else
        {
            // 옵션이 1, 2, 3, 4 외의 값일 경우 안내 메시지 출력
            printf("잘못된 옵션입니다.\n");
        }
    }  // 무한 루프 종료

    /*
     * 프로그램 종료 전, 스택 자원 해제
     * list_destroy() 함수는 스택(연결리스트)의 모든 노드를 순회하며 메모리 해제를 수행합니다.
     * 그 후, 스택 자체를 free()를 통해 해제합니다.
     */

    list_destroy(stack);
    free(stack);

    return 0;
}
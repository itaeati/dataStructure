#include "set.h"

#define CHAR_MAX (100 * sizeof(char))  // 문자 데이터를 위한 최대 할당 크기 정의

/*
 * match 함수:
 * 두 개의 문자열을 비교합니다.
 * 구현상, 두 문자열이 동일하면 strcmp()는 0을 반환하게 되며,
 * 이 함수는 같을 경우 0, 다를 경우 1을 반환합니다.
 * (원래 주석과는 반대로 동작하므로, 집합 라이브러리에서의
 * 매칭 기준에 맞게 구현되었을 가능성이 있습니다.)
 */
int match(const char* key1, const char* key2)
{
    // strcmp()가 0이 아니면 두 문자열이 다르므로 1을 반환하고, 
    // 0이면 동일하므로 0을 반환함.
    return strcmp(key1, key2) ? 1 : 0;
}

int main(void)
{
    // 두 개의 집합(Set)을 위한 메모리 동적 할당
    Set* fir_Set = malloc(sizeof(Set));
    Set* sec_Set = malloc(sizeof(Set));

    // 사용자 메뉴에서 선택한 옵션을 저장하는 변수 선언
    int option = 0, mode = 0;

    // 각각의 집합을 초기화.
    // 두 번째 인자는 데이터 비교에 사용할 match 함수, 세 번째 인자는 데이터 파괴자(여기서는 NULL)
    set_init(fir_Set, match, NULL);
    set_init(sec_Set, match, NULL);

    // 메인 반복 루프: 사용자의 입력에 따라 다양한 집합 연산을 실행함
    while (1)
    {
        // 메인 메뉴 출력
        printf("1.집합 내의 데이터 삽입\t2.집합 내의 데이터 삭제\t3.집합 내의 모든 데이터 출력\t4.집합 선택\t5.종료\n");
        scanf("%d", &option);

        if (option == 1)
        {
            /*
             * 옵션 1: 데이터 삽입.
             * set_ins 함수는 fir_Set과 sec_Set 두 집합 중 적절한 곳에
             * 데이터를 삽입하도록 내부 로직에서 처리합니다.
             */
            if (set_ins(fir_Set, sec_Set) != 0)
            {
                // 삽입 도중 오류 발생 시 메모리 해제 후 에러 코드 반환
                set_destroy(fir_Set);
                set_destroy(sec_Set);
                return -1;
            }
        }
        else if (option == 2)
        {
            /*
             * 옵션 2: 데이터 삭제.
             * set_rem 함수가 두 집합에서 데이터를 삭제하는 역할을 합니다.
             */
            if (set_rem(fir_Set, sec_Set) != 0)
            {
                // 삭제 도중 오류 발생 시 메모리 해제 후 에러 코드 반환
                set_destroy(fir_Set);
                set_destroy(sec_Set);
                return -1;
            }
        }
        else if (option == 3)
        {
            /*
             * 옵션 3: 집합 내의 모든 데이터 출력.
             * printSet 함수는 두 집합의 데이터를 화면에 출력합니다.
             */
            printSet(fir_Set, sec_Set);
            continue;  // 반복문의 끝으로 돌아감
        }
        else if (option == 4)
        {
            /*
             * 옵션 4: 집합 선택.
             * 사용자가 서브 메뉴를 통해 집합 연산(합집합, 차집합, 교집합, 부분집합 확인)을 선택할 수 있도록 함.
             */
            while (1)
            {
                // 서브 메뉴 출력
                printf("1.합집합\t2.차집합\t3.교집합\t4.부분 집합 확인\t5.종료\n");
                scanf("%d", &mode);

                if (mode == 1)
                {
                    // [합집합] 연산 처리
                    ListElmt* member;
                    // 결과를 저장할 새로운 집합(setu)을 동적 할당
                    Set* setu = malloc(sizeof(Set));

                    // fir_Set과 sec_Set의 합집합을 계산하여 setu에 저장
                    if (set_union(setu, fir_Set, sec_Set) != 0)
                    {
                        // 합집합 계산 도중 오류 발생 시 모든 집합 해제 후 종료
                        set_destroy(setu);
                        set_destroy(fir_Set);
                        set_destroy(sec_Set);
                        return -1;
                    }

                    // 결과 집합(setu)의 각 노드 순회하며 데이터 출력
                    member = setu->head;
                    while (member != NULL)
                    {
                        printf("노드 데이터 : %s, 노드 주소 : %p\n", list_data(member), member);
                        member = member->next;
                    }
                }
                else if (mode == 2)
                {
                    // [차집합] 연산 처리
                    ListElmt* member;
                    // 결과를 저장할 새로운 집합(setd)을 동적 할당
                    Set* setd = malloc(sizeof(Set));

                    // fir_Set과 sec_Set의 차집합을 계산하여 setd에 저장
                    if (set_difference(setd, fir_Set, sec_Set) != 0)
                    {
                        // 차집합 계산 오류 시 메모리 해제 후 프로그램 종료
                        set_destroy(setd);
                        set_destroy(fir_Set);
                        set_destroy(sec_Set);
                        return -1;
                    }

                    // 결과 집합(setd)의 각 노드 순회하며 데이터 출력
                    member = setd->head;
                    while (member != NULL)
                    {
                        printf("노드 데이터 : %s, 노드 주소 : %p\n", list_data(member), member);
                        member = member->next;
                    }
                }
                else if (mode == 3)
                {
                    // [교집합] 연산 처리
                    ListElmt* member;
                    // 결과를 저장할 새로운 집합(seti)을 동적 할당
                    Set* seti = malloc(sizeof(Set));

                    // fir_Set과 sec_Set의 교집합을 계산하여 seti에 저장
                    if (set_intersection(seti, fir_Set, sec_Set) != 0)
                    {
                        // 교집합 계산 오류 시 메모리 해제 후 프로그램 종료
                        set_destroy(seti);
                        set_destroy(fir_Set);
                        set_destroy(sec_Set);
                        return -1;
                    }

                    // 결과 집합(seti)의 크기가 0이면 교집합이 없음을 출력
                    member = seti->head;
                    if (list_size(seti) == 0)
                        printf("교집합은 없습니다.\n");

                    // 결과 집합의 각 노드 순회하며 데이터 출력
                    while (member != NULL)
                    {
                        printf("노드 데이터 : %s, 노드 주소 : %p\n", list_data(member), member);
                        member = member->next;
                    }
                }
                else if (mode == 4)
                {
                    // [부분 집합 확인] 연산 처리
                    // fir_Set이 sec_Set의 부분집합인지 확인
                    if (set_is_subset(fir_Set, sec_Set) == 0)
                        printf("첫 번째 집합은 두 번째 집합의 부분집합이 아닙니다.\n");
                    else
                        printf("첫 번째 집합은 두 번째 집합의 부분집합입니다.\n");
                }
                else
                {
                    // 모드 값이 1~4가 아닌 경우 내부 서브 루프 종료
                    break;
                }
            }
        }
        else if (option == 5)
        {
            // 옵션 5: 종료 선택. 메인 루프 종료 후 프로그램 종료.
            break;
        }
        else
        {
            // 잘못된 옵션 입력 시 안내 메시지 출력하고 반복문의 처음으로 돌아감.
            printf("다시 입력하세요");
            continue;
        }
    }

    // 동적 할당한 집합 메모리 해제
    free(fir_Set);
    free(sec_Set);
    return 0;
}

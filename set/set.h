#pragma comment(lib, ".\\Linked_List_Lib1.lib") 
// 이 라이브러리를 링크에 추가하여 연결 리스트 라이브러리(Linked_List_Lib1.lib)를 사용합니다.

#ifndef SET_H
#define SET_H

#include "list.h"  // 집합(Set)은 내부적으로 연결 리스트(List)를 기반으로 구현됨

/*
 * Set 타입 정의
 * Set은 List 구조체를 기반으로 하며, 별도의 타입으로 추상화하여 사용합니다.
 */
typedef List Set;


/*
 * 매크로 정의:
 *
 * set_destroy: set_destroy()는 list_destroy()로 대체되어, 집합을 해제할 때 연결 리스트 해제 함수를 사용합니다.
 * set_size: 집합의 요소 개수를 반환하는 매크로.
 */
#define set_destroy list_destroy
#define set_size(set) ((set)->size)

 /*
  * 함수 원형 선언
  */

  /*
   * set_init - 집합(Set)을 초기화하는 함수
   * 집합을 사용하는 데이터 비교 함수(match)와 데이터 소멸자 함수(destroy)를 설정합니다.
   *
   * Parameters:
   *   set     - 초기화할 집합 포인터
   *   match   - 두 개의 데이터가 동일한지 비교하는 함수 포인터 (일치하면 0을 반환하는 함수)
   *   destroy - 집합 소멸 시 데이터를 해제하는 함수 포인터 (사용하지 않을 경우 NULL)
   */
void set_init(Set* set, int (*match)(const void* key1, const void* key2), void (*destroy)(void* data));

/*
 * set_insert - 집합에 데이터를 삽입하는 함수
 * 이미 동일한 데이터가 존재하면 삽입하지 않습니다.
 *
 * Parameters:
 *   set  - 데이터를 삽입할 집합 포인터
 *   data - 삽입할 데이터의 포인터
 *
 * Returns:
 *   중복된 데이터가 존재하거나 삽입 실패 시 에러 코드(-1, 또는 1)를 반환하고,
 *   정상적으로 삽입되면 0을 반환합니다.
 */
int set_insert(Set* set, const void* data);

/*
 * set_remove - 집합에서 데이터를 제거하는 함수
 * 집합을 순회하며 일치하는 데이터를 찾아 제거합니다.
 *
 * Parameters:
 *   set  - 데이터 제거 대상 집합 포인터
 *   data - 제거할 데이터의 포인터(제거 후 해당 데이터를 반환받기 위한 더블 포인터)
 *
 * Returns:
 *   제거 성공 시 0, 일치하는 데이터가 없는 경우 -1을 반환합니다.
 */
int set_remove(Set* set, void** data);

/*
 * set_union - 두 집합(set1, set2)의 합집합을 구성하는 함수
 * 두 집합의 모든 원소를 포함하는 새 집합(setu)을 생성합니다.
 *
 * Parameters:
 *   setu - 합집합 결과를 저장할 집합 포인터
 *   set1 - 첫 번째 집합
 *   set2 - 두 번째 집합
 *
 * Returns:
 *   합집합 생성 성공 시 0, 실패 시 -1을 반환합니다.
 */
int set_union(Set* setu, const Set* set1, const Set* set2);

/*
 * set_intersection - 두 집합(set1, set2)의 교집합을 구성하는 함수
 * 두 집합에 동시에 존재하는 원소들만을 새로운 집합(seti)에 저장합니다.
 *
 * Parameters:
 *   seti - 교집합 결과를 저장할 집합 포인터
 *   set1 - 첫 번째 집합
 *   set2 - 두 번째 집합
 *
 * Returns:
 *   교집합 생성 성공 시 0, 실패 시 -1을 반환합니다.
 */
int set_intersection(Set* seti, const Set* set1, const Set* set2);

/*
 * set_difference - 두 집합(set1, set2)의 차집합을 구성하는 함수
 * set1에 존재하지만 set2에는 없는 원소들을 모아 새 집합(setd)에 저장합니다.
 *
 * Parameters:
 *   setd - 차집합 결과를 저장할 집합 포인터
 *   set1 - 기준 집합 (원소들이 포함될 집합)
 *   set2 - 비교할 집합 (제외할 원소들이 있는 집합)
 *
 * Returns:
 *   차집합 생성 성공 시 0, 실패 시 -1을 반환합니다.
 */
int set_difference(Set* setd, const Set* set1, const Set* set2);

/*
 * set_is_member - 집합 내에 특정 데이터가 존재하는지 검사하는 함수
 * 사용자가 정의한 match 함수를 이용해 동일한 데이터가 있는지 확인합니다.
 *
 * Parameters:
 *   set  - 검색할 집합 포인터
 *   data - 확인할 데이터의 포인터
 *
 * Returns:
 *   데이터가 존재하면 1, 없으면 0을 반환합니다.
 */
int set_is_member(const Set* set, const void* data);

/*
 * set_is_subset - 한 집합이 다른 집합의 부분집합인지 검사하는 함수
 *
 * Parameters:
 *   set1 - 부분집합 여부를 검증할 집합 포인터
 *   set2 - 기준 집합 포인터
 *
 * Returns:
 *   set1이 set2의 부분집합이면 1, 아니면 0을 반환합니다.
 */
int set_is_subset(const Set* set1, const Set* set2);

/*
 * set_is_equal - 두 집합이 동일한지 검사하는 함수
 * 두 집합의 크기가 같고, 상대 간에 부분집합 관계이면 동일한 것으로 판단합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   두 집합이 동일하면 1, 아니면 0을 반환합니다.
 */
int set_is_equal(const Set* set1, const Set* set2);

/*
 * set_ins - 사용자 인터페이스용 집합 삽입 함수
 * 두 집합 중 선택한 집합에 데이터를 삽입하도록 인터페이스를 제공합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   삽입 성공 시 0, 에러 발생 시 -1을 반환합니다.
 */
int set_ins(Set* set1, Set* set2);

/*
 * set_rem - 사용자 인터페이스용 집합 삭제 함수
 * 두 집합 중 선택한 집합에서 데이터를 삭제하도록 인터페이스를 제공합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 *
 * Returns:
 *   삭제 성공 시 0, 에러 발생 시 -1을 반환합니다.
 */
int set_rem(Set* set1, Set* set2);

/*
 * printSet - 선택한 집합의 모든 데이터를 출력하는 함수
 * 사용자에게 출력할 집합을 선택하게 한 후, 해당 집합의 각 요소를 화면에 출력합니다.
 *
 * Parameters:
 *   set1 - 첫 번째 집합 포인터
 *   set2 - 두 번째 집합 포인터
 */
void printSet(Set* set1, Set* set2);

#endif  // SET_H

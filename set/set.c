#include "set.h"

/*
 * set_init - ����(Set) �ʱ�ȭ �Լ�
 *
 * �� �Լ��� ������ �ʱ�ȭ �մϴ�.
 * ���������� ������ ���� ����Ʈ(List)�� ������� �����Ǿ� �����Ƿ�,
 * list_init() �Լ��� ȣ���Ͽ� ����Ʈ�� �ʱ�ȭ�ϰ�,
 * ���ư� ��� �񱳿� ����� ����� ���� �Լ�(match)�� �����մϴ�.
 *
 * Parameters:
 *   set     - �ʱ�ȭ�� ���� ������
 *   match   - �� �������� �񱳸� ���� �Լ� ������ (�����ϸ� 0 ��ȯ)
 *   destroy - ���� �Ҹ� �� ������ ������ ����� �Լ� ������ (������� ���� ��� NULL)
 */
void set_init(Set* set,
    int (*match)(const void* key1, const void* key2),
    void (*destroy)(void* data))
{
    list_init(set, destroy);   // ���� ����Ʈ ����� ���� �ʱ�ȭ
    set->match = match;        // ������ �񱳿� ����� �Լ��� ����
    return;
}

/*
 * set_insert - ���տ� �����͸� �����ϴ� �Լ�
 *
 * ���տ� �ߺ� ���� �����͸� �����մϴ�.
 * ���� ������ �����Ͱ� �̹� �����ϸ� ������ �ź��ϰ� 1�� ��ȯ�մϴ�.
 *
 * Parameters:
 *   set  - �����͸� ������ ���� ������
 *   data - ������ ������ ������
 *
 * Returns:
 *   �ߺ� �������̸� 1, ���� ���� �� 0, ���� ���� �� -1�� ��ȯ
 */
int set_insert(Set* set, const void* data)
{
    // �����Ͱ� �̹� �����ϸ� �ߺ����� �����Ͽ� �������� ����.
    if (set_is_member(set, data))
        return 1;

    // ���� ����Ʈ�� ����(tail)�� �����͸� ������.
    return list_ins_next(set, list_tail(set), data);
}

/*
 * set_remove - ���տ��� Ư�� �����͸� �����ϴ� �Լ�
 *
 * ���� ������ ���� ����Ʈ�� ���� Ž���Ͽ�,
 * �Է� ���� Ű�� ��ġ�ϴ� ��Ҹ� ã�� �����մϴ�.
 *
 * Parameters:
 *   set  - �����͸� ������ ���� ������
 *   data - ������ �������� Ű�� ����Ű�� ������. ���� �� �����͸� ��ȯ���� �� ����.
 *
 * Returns:
 *   ���� ���� �� 0, ã�� ���� ��� -1�� ��ȯ
 */
int set_remove(Set* set, void** data)
{
    ListElmt* member;
    ListElmt* prev = NULL;

    // ����(���� ����Ʈ)�� �� ��带 ���������� Ž��
    for (member = list_head(set); member != NULL; member = member->next) {
        if (set->match(data, member->data))
            break;
        prev = member;
    }

    // ��ġ�ϴ� ��Ұ� ������ ���� ��ȯ
    if (member == NULL)
        return -1;

    // ���� ���(prev) ���� ��Ҹ� �����ϰ� data�� ����
    return list_rem_next(set, prev, data);
}

/*
 * set_union - �� ������ �������� �����ϴ� �Լ�
 *
 * set1�� set2�� ��� ��Ҹ� �����ϴ� �������� �� ���� setu�� �����մϴ�.
 * set1�� ��� ��Ҵ� ������ �����տ� ���ԵǸ�,
 * set2�� ��� �� set1�� ���� ��Ҹ� �߰��մϴ�.
 *
 * Parameters:
 *   setu - ������ ����� ������ ���� ������
 *   set1 - ù ��° ���� ������
 *   set2 - �� ��° ���� ������
 *
 * Returns:
 *   ���� �� 0, ������ ���� ���� �� setu�� ���� �� -1 ��ȯ
 */
int set_union(Set* setu, const Set* set1, const Set* set2)
{
    ListElmt* member;
    void* data;

    // ������ �ʱ�ȭ (set1�� match �Լ� ���)
    set_init(setu, set1->match, NULL);

    // set1�� ��� ��Ҹ� �����տ� ����
    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        data = list_data(member);
        if (list_ins_next(setu, list_tail(setu), data) != 0) {
            set_destroy(setu);
            return -1;
        }
    }

    // set2�� ��� �� set1�� ���� �͸� �����տ� ����
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
 * set_intersection - �� ������ �������� �����ϴ� �Լ�
 *
 * set1�� set2 ��ο� �����ϴ� ��ҵ鸸�� ��� �� ���� seti�� �����մϴ�.
 *
 * Parameters:
 *   seti - ������ ����� ������ ���� ������
 *   set1 - ù ��° ���� ������
 *   set2 - �� ��° ���� ������
 *
 * Returns:
 *   ���� �� 0, ������ ���� ���� �� seti�� ���� �� -1 ��ȯ
 */
int set_intersection(Set* seti, const Set* set1, const Set* set2)
{
    ListElmt* member;
    void* data;

    // ������ ���� �ʱ�ȭ (set1�� match �Լ� ���)
    set_init(seti, set1->match, NULL);

    // set1�� �� ��Ұ� set2���� �����ϴ��� Ȯ��
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
 * set_difference - ���� ������ (set1 - set2)�� �����ϴ� �Լ�
 *
 * set1�� ���������� set2���� ���� ��ҵ��� ã�� �� ���� setd�� �����մϴ�.
 *
 * Parameters:
 *   setd - ������ ����� ������ ���� ������
 *   set1 - ���� ���� ������
 *   set2 - �� ���� ������
 *
 * Returns:
 *   ���� �� 0, ���� ���� �� setd�� ���� �� -1 ��ȯ
 */
int set_difference(Set* setd, const Set* set1, const Set* set2)
{
    ListElmt* member;
    void* data;

    // ������ �ʱ�ȭ (set1�� match �Լ� ���)
    set_init(setd, set1->match, NULL);

    // set1�� �� ��Ұ� set2�� ���� ��쿡�� �����տ� ����
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
 * set_is_member - �����Ͱ� ���� ���� �����ϴ��� Ȯ���ϴ� �Լ�
 *
 * ������ ��� ��Ҹ� ��ȸ�ϸ�, �Է� �����Ϳ� ������ ��Ұ� �ִ��� ���մϴ�.
 * �񱳴� ������ match �Լ��� �����մϴ�.
 *
 * Parameters:
 *   set  - �˻��� ���� ������
 *   data - �˻��� ������ ������
 *
 * Returns:
 *   �����Ͱ� �����ϸ� 1, �������� ������ 0�� ��ȯ
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
 * set_is_subset - �� ������ �ٸ� ������ �κ��������� �˻��ϴ� �Լ�
 *
 * set1�� ��� ��Ұ� set2���� �����ϸ� set1�� set2�� �κ������Դϴ�.
 * ��, set1�� ũ�Ⱑ set2���� Ŭ ��� �ٷ� �κ������� �ƴ��� ��ȯ�մϴ�.
 *
 * Parameters:
 *   set1 - �κ����� ���θ� �˻��� ����
 *   set2 - ���� ����
 *
 * Returns:
 *   set1�� set2�� �κ������̸� 1, �ƴϸ� 0
 */
int set_is_subset(const Set* set1, const Set* set2)
{
    ListElmt* member;

    // set1�� set2���� ũ�� set1�� ���� �κ������� �� �� ����.
    if (list_size(set1) > list_size(set2))
        return 0;

    // set1�� �� ��Ұ� set2���� �����ϴ��� Ȯ��
    for (member = list_head(set1); member != NULL; member = member->next) {
        if (!set_is_member(set2, member->data))
            return 0;
    }
    return 1;
}

/*
 * set_is_equal - �� ������ �������� �˻��ϴ� �Լ�
 *
 * �� ������ ũ�Ⱑ ����, ������ �κ����� ������ �� �����ϴٰ� �Ǵ��մϴ�.
 *
 * Parameters:
 *   set1 - ù ��° ����
 *   set2 - �� ��° ����
 *
 * Returns:
 *   �� ������ �����ϸ� 1, �ƴϸ� 0
 */
int set_is_equal(const Set* set1, const Set* set2)
{
    // ũ�Ⱑ �ٸ��� ������ �� ����.
    if (set1->size != set2->size)
        return 0;

    // set1�� set2�� �κ������̸� �� ������ ������.
    return set_is_subset(set1, set2);
}

/*
 * set_ins - ����ڷκ��� �Է� �޾� �� ���� �� �ϳ��� �����͸� �����ϴ� �Լ�
 *
 * ����ڰ� ������ ���տ� ���� ���ڿ� �����͸� �Է¹޾� �����մϴ�.
 * �����Ϸ��� �����Ͱ� �̹� ������ ��� ������ ����ϰ� ������ �޸𸮸� �����մϴ�.
 *
 * Parameters:
 *   set1 - ù ��° ���� ������
 *   set2 - �� ��° ���� ������
 *
 * Returns:
 *   ���� �� 0, ���� �߻� �� -1
 */
int set_ins(Set* set1, Set* set2)
{
    int choice = 0;
    char* string = malloc(CHAR_MAX);   // �Է� ���� ���ڿ� �޸� �Ҵ�

    if (string == NULL)
        return -1;

    printf("������ �����ϼ���\n1. ù ��° ����\t2. �� ��° ����: ");
    scanf("%d", &choice);

    if (choice == 1) {  // ù ��° ���� ���� ��
        printf("������ �����͸� �Է��ϼ���: ");
        scanf("%s", string);
        if (set_insert(set1, string) != 0) {
            printf("�ߺ��� �����Ͱ� �ֽ��ϴ�.\n");
            free(string);
            return -1;
        }
    }
    else if (choice == 2) {  // �� ��° ���� ���� ��
        printf("������ �����͸� �Է��ϼ���: ");
        scanf("%s", string);
        if (set_insert(set2, string) != 0) {
            printf("�ߺ��� �����Ͱ� �ֽ��ϴ�.\n");
            free(string);
            return -1;
        }
    }
    else {
        // �߸��� ������ ���
        printf("�߸��� �����Դϴ�.\n");
        free(string);
        return -1;
    }

    return 0;
}

/*
 * set_rem - ����� �Է��� �޾� ���տ��� Ư�� �����͸� �����ϴ� �Լ�
 *
 * ����ڰ� ������ ���տ��� ������ �����͸� �Է¹޾� �ش� �����͸� �����մϴ�.
 *
 * Parameters:
 *   set1 - ù ��° ���� ������
 *   set2 - �� ��° ���� ������
 *
 * Returns:
 *   ���� ���� �� 0, ���� �߻� �� -1
 */
int set_rem(Set* set1, Set* set2)
{
    int choice = 0;
    char* string = malloc(CHAR_MAX);

    if (string == NULL)
        return -1;

    printf("������ �����ϼ���\n1. ù ��° ����\t2. �� ��° ����: ");
    scanf("%d", &choice);

    printf("������ �����͸� �Է��ϼ���: ");
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
        printf("�߸��� �����Դϴ�.\n");
        free(string);
        return -1;
    }

    free(string);
    return 0;
}

/*
 * printSet - ������ ������ ��� ��Ҹ� ����ϴ� �Լ�
 *
 * ����ڿ��� ����� ����(ù ��° �Ǵ� �� ��°)�� �����ϵ��� ��û�� ��,
 * ���õ� ������ �� ��带 ��ȸ�ϸ� �����Ϳ� ��� �ּҸ� ����մϴ�.
 *
 * Parameters:
 *   set1 - ù ��° ���� ������
 *   set2 - �� ��° ���� ������
 */
void printSet(Set* set1, Set* set2)
{
    ListElmt* member;
    int choice = 0;

    // �ùٸ� ���� ��ȣ(1 �Ǵ� 2)�� �Էµ� ������ �ݺ�
    do {
        printf("������ �����ϼ���\n1. ù ��° ����\t2. �� ��° ����: ");
        scanf("%d", &choice);
        if (choice != 1 && choice != 2)
            printf("�߸��� �����Դϴ�. �ٽ� �Է��ϼ���.\n");
    } while (choice != 1 && choice != 2);

    if (choice == 1) {  // ù ��° ���� ����
        member = set1->head;
        if (list_head(set1) == NULL)
            printf("ù ��° ������ ����ֽ��ϴ�.\n");
        else {
            while (member != NULL) {
                printf("��� ������ : %s, ��� �ּ� : %p\n", list_data(member), member);
                member = member->next;
            }
        }
    }
    else if (choice == 2) {  // �� ��° ���� ����
        member = set2->head;
        if (list_head(set2) == NULL)
            printf("�� ��° ������ ����ֽ��ϴ�.\n");
        else {
            while (member != NULL) {
                printf("��� ������ : %s, ��� �ּ� : %p\n", list_data(member), member);
                member = member->next;
            }
        }
    }
}

#include "set.h"

#define CHAR_MAX (100 * sizeof(char))  // ���� �����͸� ���� �ִ� �Ҵ� ũ�� ����

/*
 * match �Լ�:
 * �� ���� ���ڿ��� ���մϴ�.
 * ������, �� ���ڿ��� �����ϸ� strcmp()�� 0�� ��ȯ�ϰ� �Ǹ�,
 * �� �Լ��� ���� ��� 0, �ٸ� ��� 1�� ��ȯ�մϴ�.
 * (���� �ּ����� �ݴ�� �����ϹǷ�, ���� ���̺귯��������
 * ��Ī ���ؿ� �°� �����Ǿ��� ���ɼ��� �ֽ��ϴ�.)
 */
int match(const char* key1, const char* key2)
{
    // strcmp()�� 0�� �ƴϸ� �� ���ڿ��� �ٸ��Ƿ� 1�� ��ȯ�ϰ�, 
    // 0�̸� �����ϹǷ� 0�� ��ȯ��.
    return strcmp(key1, key2) ? 1 : 0;
}

int main(void)
{
    // �� ���� ����(Set)�� ���� �޸� ���� �Ҵ�
    Set* fir_Set = malloc(sizeof(Set));
    Set* sec_Set = malloc(sizeof(Set));

    // ����� �޴����� ������ �ɼ��� �����ϴ� ���� ����
    int option = 0, mode = 0;

    // ������ ������ �ʱ�ȭ.
    // �� ��° ���ڴ� ������ �񱳿� ����� match �Լ�, �� ��° ���ڴ� ������ �ı���(���⼭�� NULL)
    set_init(fir_Set, match, NULL);
    set_init(sec_Set, match, NULL);

    // ���� �ݺ� ����: ������� �Է¿� ���� �پ��� ���� ������ ������
    while (1)
    {
        // ���� �޴� ���
        printf("1.���� ���� ������ ����\t2.���� ���� ������ ����\t3.���� ���� ��� ������ ���\t4.���� ����\t5.����\n");
        scanf("%d", &option);

        if (option == 1)
        {
            /*
             * �ɼ� 1: ������ ����.
             * set_ins �Լ��� fir_Set�� sec_Set �� ���� �� ������ ����
             * �����͸� �����ϵ��� ���� �������� ó���մϴ�.
             */
            if (set_ins(fir_Set, sec_Set) != 0)
            {
                // ���� ���� ���� �߻� �� �޸� ���� �� ���� �ڵ� ��ȯ
                set_destroy(fir_Set);
                set_destroy(sec_Set);
                return -1;
            }
        }
        else if (option == 2)
        {
            /*
             * �ɼ� 2: ������ ����.
             * set_rem �Լ��� �� ���տ��� �����͸� �����ϴ� ������ �մϴ�.
             */
            if (set_rem(fir_Set, sec_Set) != 0)
            {
                // ���� ���� ���� �߻� �� �޸� ���� �� ���� �ڵ� ��ȯ
                set_destroy(fir_Set);
                set_destroy(sec_Set);
                return -1;
            }
        }
        else if (option == 3)
        {
            /*
             * �ɼ� 3: ���� ���� ��� ������ ���.
             * printSet �Լ��� �� ������ �����͸� ȭ�鿡 ����մϴ�.
             */
            printSet(fir_Set, sec_Set);
            continue;  // �ݺ����� ������ ���ư�
        }
        else if (option == 4)
        {
            /*
             * �ɼ� 4: ���� ����.
             * ����ڰ� ���� �޴��� ���� ���� ����(������, ������, ������, �κ����� Ȯ��)�� ������ �� �ֵ��� ��.
             */
            while (1)
            {
                // ���� �޴� ���
                printf("1.������\t2.������\t3.������\t4.�κ� ���� Ȯ��\t5.����\n");
                scanf("%d", &mode);

                if (mode == 1)
                {
                    // [������] ���� ó��
                    ListElmt* member;
                    // ����� ������ ���ο� ����(setu)�� ���� �Ҵ�
                    Set* setu = malloc(sizeof(Set));

                    // fir_Set�� sec_Set�� �������� ����Ͽ� setu�� ����
                    if (set_union(setu, fir_Set, sec_Set) != 0)
                    {
                        // ������ ��� ���� ���� �߻� �� ��� ���� ���� �� ����
                        set_destroy(setu);
                        set_destroy(fir_Set);
                        set_destroy(sec_Set);
                        return -1;
                    }

                    // ��� ����(setu)�� �� ��� ��ȸ�ϸ� ������ ���
                    member = setu->head;
                    while (member != NULL)
                    {
                        printf("��� ������ : %s, ��� �ּ� : %p\n", list_data(member), member);
                        member = member->next;
                    }
                }
                else if (mode == 2)
                {
                    // [������] ���� ó��
                    ListElmt* member;
                    // ����� ������ ���ο� ����(setd)�� ���� �Ҵ�
                    Set* setd = malloc(sizeof(Set));

                    // fir_Set�� sec_Set�� �������� ����Ͽ� setd�� ����
                    if (set_difference(setd, fir_Set, sec_Set) != 0)
                    {
                        // ������ ��� ���� �� �޸� ���� �� ���α׷� ����
                        set_destroy(setd);
                        set_destroy(fir_Set);
                        set_destroy(sec_Set);
                        return -1;
                    }

                    // ��� ����(setd)�� �� ��� ��ȸ�ϸ� ������ ���
                    member = setd->head;
                    while (member != NULL)
                    {
                        printf("��� ������ : %s, ��� �ּ� : %p\n", list_data(member), member);
                        member = member->next;
                    }
                }
                else if (mode == 3)
                {
                    // [������] ���� ó��
                    ListElmt* member;
                    // ����� ������ ���ο� ����(seti)�� ���� �Ҵ�
                    Set* seti = malloc(sizeof(Set));

                    // fir_Set�� sec_Set�� �������� ����Ͽ� seti�� ����
                    if (set_intersection(seti, fir_Set, sec_Set) != 0)
                    {
                        // ������ ��� ���� �� �޸� ���� �� ���α׷� ����
                        set_destroy(seti);
                        set_destroy(fir_Set);
                        set_destroy(sec_Set);
                        return -1;
                    }

                    // ��� ����(seti)�� ũ�Ⱑ 0�̸� �������� ������ ���
                    member = seti->head;
                    if (list_size(seti) == 0)
                        printf("�������� �����ϴ�.\n");

                    // ��� ������ �� ��� ��ȸ�ϸ� ������ ���
                    while (member != NULL)
                    {
                        printf("��� ������ : %s, ��� �ּ� : %p\n", list_data(member), member);
                        member = member->next;
                    }
                }
                else if (mode == 4)
                {
                    // [�κ� ���� Ȯ��] ���� ó��
                    // fir_Set�� sec_Set�� �κ��������� Ȯ��
                    if (set_is_subset(fir_Set, sec_Set) == 0)
                        printf("ù ��° ������ �� ��° ������ �κ������� �ƴմϴ�.\n");
                    else
                        printf("ù ��° ������ �� ��° ������ �κ������Դϴ�.\n");
                }
                else
                {
                    // ��� ���� 1~4�� �ƴ� ��� ���� ���� ���� ����
                    break;
                }
            }
        }
        else if (option == 5)
        {
            // �ɼ� 5: ���� ����. ���� ���� ���� �� ���α׷� ����.
            break;
        }
        else
        {
            // �߸��� �ɼ� �Է� �� �ȳ� �޽��� ����ϰ� �ݺ����� ó������ ���ư�.
            printf("�ٽ� �Է��ϼ���");
            continue;
        }
    }

    // ���� �Ҵ��� ���� �޸� ����
    free(fir_Set);
    free(sec_Set);
    return 0;
}

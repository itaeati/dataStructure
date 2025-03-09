#include "list.h"

#define CHAR_MAX (100 * sizeof(char))

/*
 * match: �� ���ڿ� �� �Լ�
 * �� ���ڿ��� ������ 1, �ٸ��� 0�� ��ȯ�Ѵ�.
 */
int match(const void* key1, const void* key2)
{
    if (strcmp((const char*)key1, (const char*)key2) == 0)
        return 1;
    else
        return 0;
}

int main(void) // ���α׷�: ���� ����Ʈ�� ����Ͽ� ���ڿ� �����͸� �����ϰ� �׽�Ʈ�Ѵ�.
{
    int option = 0;

    /* ���� �Ҵ� �� �ʱ�ȭ */
    List* link_list = malloc(sizeof(List)); // ���� ����Ʈ ����
    list_init(link_list, NULL);              // ���� ����Ʈ �ʱ�ȭ
    link_list->match = &match;               // �� �Լ��� ���� ����Ʈ�� ���

    while (1) // �޴� ��� ���� ����
    {
        /* ���� ����Ʈ �׽�Ʈ�� ���� �޴�
         * 1. ���Ḯ��Ʈ ��� ����
         * 2. ���Ḯ��Ʈ ��� ����
         * 3. ���Ḯ��Ʈ ��ü ���
         * 4. ���α׷� ����
         */
        printf("1.���Ḯ��Ʈ ��� ����\t2.���Ḯ��Ʈ ��� ����\t3.���Ḯ��Ʈ ��ü ���\t4.���α׷� ����\n");
        scanf("%d", &option);

        if (option == 1)  // [�ɼ� 1] ���� ����Ʈ�� �� ��� ����
        {
            char* string = malloc(CHAR_MAX);    // ������ ������ ����
            char* reference = malloc(CHAR_MAX);   // ���� ��� ������ ����

            printf("�����ϰ��� �ϴ� ��� �����͸� �Է��ϼ���: ");
            scanf("%s", string);

            if (link_list->size == 1)
            {
                /* ��尡 1���� ��쿡�� ���� ��尡 �ʿ�����Ƿ� �̸� �Ҵ��� reference ���� */
                free(reference);
                reference = NULL;
            }
            else if (link_list->size >= 2)
            {
                printf("���� ����� �����͸� �Է��ϼ���: ");
                scanf("%s", reference);
            }

            if (add_node(link_list, string, reference) != 0)
            {
                list_destroy(link_list);
                free(link_list);
                return -1;
            }
            /* ����: ���� add_node()�� �Է� ���� string�� �������� ���� ����Ʈ�� �ѱ⵵�� ����Ǿ��ٸ�,
             * ���⼭ ������ string�� free�ϸ� �ȵȴ�. (�޸� ���� ��å�� ����)
             */
        }
        else if (option == 2)  // [�ɼ� 2] ���� ����Ʈ���� ��� ����
        {
            char* string = malloc(CHAR_MAX);

            printf("�����ϰ��� �ϴ� ����� �����͸� �Է��ϼ���: ");
            scanf("%s", string);

            if (rem_node(link_list, string) != 0)
            {
                free(string);
                list_destroy(link_list);
                free(link_list);
                return -1;
            }
            free(string);  // ��� �� ���� �Ҵ� ���� ����
        }
        else if (option == 3)  // [�ɼ� 3] ���� ����Ʈ ��ü ���
        {
            ListElmt* member = link_list->head;
            if (member == NULL)
            {
                printf("���� ����Ʈ�� ����ֽ��ϴ�.\n");
            }
            else
            {
                while (member != NULL)
                {
                    printf("��� �ּ�: %p\n��� ������: %s\n���� ��� �ּ�: %p\n",
                        member, (char*)member->data, member->next);
                    member = member->next;
                }
            }
        }
        else if (option == 4)  // [�ɼ� 4] ���α׷� ����
        {
            break;
        }
    }

    /* ���α׷� ���� ��, ���� ����Ʈ�� ��� �ڿ� ���� */
    list_destroy(link_list);
    free(link_list);

    return 0;
}

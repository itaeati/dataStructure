#include "dlist.h"

#define CHAR_MAX (100 * sizeof(char))

// �� Ű ���� ���ϴ� �Լ�
int match(const void* key1, const void* key2)
{
    if (strcmp((const char*)key1, (const char*)key2) == 0)
        return 1; // ��ġ�ϸ� 1 ��ȯ
    else
        return 0; // ��ġ���� ������ 0 ��ȯ
}

int main(void)
{
    int mode = 0;
    // ���� ���� ����Ʈ�� ���� �޸� �Ҵ�
    DList* list = malloc(sizeof(DList));

    // �޸� �Ҵ� ���� ���� Ȯ��
    if (list == NULL)
    {
        printf("����Ʈ�� ���� �޸� �Ҵ翡 �����߽��ϴ�.\n");
        return -1;
    }

    dlist_init(list, NULL); // ����Ʈ �ʱ�ȭ
    list->match = &match;   // match �Լ� ����

    while (1)
    {
        printf("���� ���Ḯ��Ʈ\n1.����Ʈ ��� �߰�  2.����Ʈ ��� ����  3.����Ʈ ��ü ���  4.��� ��ü ����\t5.����\n");
        scanf("%d", &mode);

        if (mode == 1)
        {
            char* string = malloc(CHAR_MAX);
            char* reference = malloc(CHAR_MAX);
            int option = 0;

            // �޸� �Ҵ� ���� Ȯ��
            if (string == NULL || reference == NULL)
            {
                printf("���ڿ��� ���� �޸� �Ҵ翡 �����߽��ϴ�.\n");
                free(string);
                free(reference);
                continue;
            }

            printf("�߰��� ����� �����͸� �Է��ϼ��� :");
            scanf("%s", string);

            if (list->size == 1)
            {
                /* ��尡 �ϳ���� ���� ��� �޸𸮸� ���� */
                free(reference);
                reference = NULL;
            }
            else if (list->size >= 2)
            {
                printf("���� ����� �����͸� �Է��ϼ��� :");
                scanf("%s", reference);
            }

            if (list->size >= 1)
            {
                printf("1 : ���� ��� �ڿ� ��� ����\t2.���� ��� �տ� ��� ����\n");
                scanf("%d", &option);
            }
            else
            {
                option = 1;
            }

            // add_node �Լ� ���� ��� Ȯ��
            if (add_node(list, string, reference, option) != 0)
            {
                printf("��� �߰��� �����߽��ϴ�.\n");
                free(string);
                free(reference);
                free(list);
                return -1;
            }
        }

        if (mode == 2)
        {
            char* string = malloc(CHAR_MAX);

            if (string == NULL) // �޸� �Ҵ� ���� Ȯ��
            {
                printf("���ڿ��� ���� �޸� �Ҵ翡 �����߽��ϴ�.\n");
                continue;
            }

            printf("���� �ϰ��� �ϴ� ����� �����͸� �Է��ϼ��� : ");
            scanf("%s", string);

            if (rem_node(list, string) != 0)
            {
                printf("��� ���ſ� �����߽��ϴ�.\n");
                free(string);
                free(list);
                return -1;
            }

            free(string); // �޸� ����
        }

        if (mode == 3)
        {
            DListElmt* member = list->head;
            if (list->size == 0)
                printf("���� ���Ḯ��Ʈ�� ����ֽ��ϴ�.\n");
            else
            {
                while (member != NULL)
                {
                    printf("���� ��� �ּ� : %p\n��� �ּ� : %p\n��� ������ : %s\n���� ��� �ּ� : %p\n\n",
                        member->prev, member, (char*)member->data, member->next);
                    member = member->next;
                }
            }
        }

        if (mode == 4)
        {
            dlist_destroy(list); // ��� ��� ����
            continue;
        }

        if (mode == 5)
            break;
    }

    dlist_destroy(list); // ����Ʈ ����
    free(list); // ����Ʈ �޸� ����
    return 0;
}

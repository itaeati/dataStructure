#include "queue.h"   // ť �ڷᱸ�� ����; ���������� ���� ����Ʈ(List)�� ������� ������.

#define CHAR_MAX (100 * sizeof(char))  // 100���� char ����(����Ʈ ����) Ȯ��

/*
 * match: �� ���ڿ� �� �Լ�
 * --------------------------------------------------------
 * ����:
 *   �� ���ڿ�(key1�� key2)�� ������ �������� �˻��մϴ�.
 *   ���ڿ��� �����ϸ� 1, �׷��� ������ 0�� ��ȯ�մϴ�.
 *
 * �Ű�����:
 *   key1, key2: ���� �� ���ڿ��� ����Ű�� ������(void*).
 *              �����δ� char* Ÿ���� ���ڿ��� ���޵˴ϴ�.
 *
 * ��ȯ��:
 *   ���ڿ��� ������ 1, �ƴϸ� 0.
 */
int match(const void* key1, const void* key2)
{
    return (strcmp((const char*)key1, (const char*)key2) == 0) ? 1 : 0;
}

int main(void)
{
    int option = 0;  // �޴����� ����ڰ� ������ �ɼ��� ������ ����

    /* ť ���� �� �ʱ�ȭ */
    // ���� �޸𸮸� �Ҵ��Ͽ� ť ����ü(���������� ���� ����Ʈ)�� �����մϴ�.
    Queue* queue = malloc(sizeof(Queue));
    if (queue == NULL) 
    {
        perror("�޸� �Ҵ� ����");
        return -1;
    }
    // queue_init�� ���������� list_init�� ȣ���Ͽ� ť�� �ʱ�ȭ�մϴ�.
    queue_init(queue, NULL);
    // �ʿ��ϴٸ�, ť�� ���Լ��� ������ �� �ֽ��ϴ�.
    queue->match = &match;

    while (1)  // �޴� ��� ���� ����
    {
        // �޴� ���
        printf("1. ť ����\t2. ť ������\t3. ť ��ü ���\t4. ���α׷� ����\n");
        scanf("%d", &option);

        if (option == 1)  /* [�ɼ� 1] ť ���� (enqueue) */
        {
            // ����� �Է��� ���� �����͸� ���� ���۸� ���� �Ҵ��մϴ�.
            char* data = malloc(CHAR_MAX);
            if (data == NULL) 
            {
                perror("�޸� �Ҵ� ����");
                continue;
            }
            printf("ť�� ������ �����͸� �Է��ϼ���: ");
            scanf("%s", data);

            // queue_enqueue�� ť�� �Ĵ�(��, tail)�� �����͸� �����մϴ�.
            if (queue_enqueue(queue, data) != 0) 
            {
                // ���� ���� ��, �Ҵ��� �޸𸮸� �����մϴ�.
                free(data);
                printf("ť ���� ����\n");
            }
            else 
            {
                printf("�����Ͱ� ť�� ���ԵǾ����ϴ�.\n");
            }
            // ���Կ� �����ϸ�, �������� �������� ť�� �����ǹǷ� ���⼭ free���� �ʽ��ϴ�.
        }
        else if (option == 2)  /* [�ɼ� 2] ť ������ (dequeue) */
        {
            char* data = NULL;
            // queue_dequeue �Լ��� ť�� ����(head)���� �����͸� �����ϰ�, ������ �����͸� ��ȯ�մϴ�.
            if (queue_dequeue(queue, (void**)&data) != 0) 
            {
                printf("ť�� ����ֽ��ϴ�.\n");
            }
            else 
            {
                printf("ť���� ���� ������: %s\n", data);
                free(data);  // ������ �����ʹ� �� �̻� ������� �����Ƿ� �����մϴ�.
            }
        }
        else if (option == 3)  /* [�ɼ� 3] ť ��ü ��� */
        {
            // ť�� ���������� ���� ����Ʈ(List)�� �����Ǿ� �����Ƿ�, head���� ��ȸ�մϴ�.
            ListElmt* member = queue->head;

            if (member == NULL) 
            {
                printf("ť�� ����ֽ��ϴ�.\n");
            }

            else 
            {
                printf("\n-- ť ��ü ��� --\n");
                while (member != NULL) {
                    printf("��� ������: %s\n��� �ּ�: %p\n���� ��� �ּ�: %p\n", (char*)member->data, member, member->next);
                    member = member->next;
                }
            }

        }

        else if (option == 4)  /* [�ɼ� 4] ���α׷� ���� */
        {
            break;
        }

        else 
        {
            // ��ȿ���� ���� �ɼ� �Է� �� �޽��� ���
            printf("�߸��� �ɼ��Դϴ�.\n");
        }
    }

    /* ���α׷� ���� ��, ť�� �Ҵ�� ��� �ڿ��� ���� */
    queue_destroy(queue);
    free(queue);

    return 0;
}

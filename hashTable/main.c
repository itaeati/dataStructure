#include "chtbl.h"                           // �ؽ� ���̺� ���� �Լ��� ����ü ���� ���
#define CHAR_MAX 100 * (sizeof(char))          // ���ڿ� �Է��� ���� �ִ� ũ�� (100����Ʈ)

// �� ���ڿ�(key1, key2)�� �������� ���ϴ� �Լ� 
// strcmp�� �� ���ڿ��� ������ 0�� ��ȯ�ϹǷ� �̸� �״�� ���
int match(const char* key1, const char* key2)
{
    return strcmp(key1, key2);
}

// ���ڿ� ���� �յ��ϰ� �л��Ű�� ���� �ؽ� �Լ�
// �Է¹��� key�� ������� �ؽ� ���� ����Ͽ� ��ȯ
int hash_Func(const void* key) // ���ڿ� �յ� �ؽ� �Լ�
{
    const char* ptr;      // key�� ���� char ������
    int val;              // �ؽ� ��

    val = 0;
    ptr = key;            // key�� ���ڿ��� ó��

    // ���ڿ��� �� ('\0')���� ��ȸ�ϸ鼭 �ؽ� ���� ���
    while (*ptr != '\0')
    {
        int tmp;

        // ���� �ؽ� ���� ���� ������ ���� ���ϸ鼭 �������� 4��Ʈ ����Ʈ
        val = (val << 4) + (*ptr);

        // ���� 4��Ʈ�� ä���� ��� �̸� ó���Ͽ� ���� ������ ���� ��
        if (tmp = (val & 0xf0000000))
        {
            val = val ^ (tmp >> 24);
            val = val ^ tmp;
        }
        ptr++;  // ���� ���ڷ� �̵�
    }
    return val;
}

int main(void)
{
    // �ؽ� ���̺� ����ü�� ���� �޸� �Ҵ�
    CHTbl* htbl = malloc(sizeof(CHTbl));
    int bucket = 0; // ����ڰ� �Է��� ��Ŷ ��

    // �޸� �Ҵ� ���� �� ���α׷� ����
    if (htbl == NULL)
        return -1;

    // �ؽ� ���̺��� ��Ŷ ���� ����ڷκ��� �Է� ����
    printf("�ؽ� ���̺��� ��Ŷ�� ���� �Է��ϼ���: ");
    scanf("%d", &bucket);

    // �ؽ� ���̺� �ʱ�ȭ: �Ҵ�� ��Ŷ ��, �ؽ� �Լ�, �� �Լ�, ���� �Լ��� ����
    if (chtbl_init(htbl, bucket, hash_Func, match, NULL) != 0)
        return -1;

    // ���� ������ ����ؼ� ������� �Է��� ó��
    while (1)
    {
        int mode = 0;

        // ����� �޴� ���
        printf("1.������ ����\t2.������ ���\t3.������ ����\t4.������ ��ü ���\t5.����\n");
        scanf("%d", &mode);

        // ��� 1: ������ ����
        if (mode == 1)
        {
            // ������ ������ ���� �޸� �Ҵ�
            char* string = malloc(CHAR_MAX);

            printf("�����͸� �Է��ϼ��� : ");
            scanf("%s", string);

            // ������ ���� �� �ߺ� üũ �� ����, ���� �� �޸� ���� �� ����
            if (chtbl_insert(htbl, string) != 0)
            {
                free(string);
                return -1;
            }
        }
        // ��� 2: ���� ������ ��ȸ (���)
        else if (mode == 2)
        {
            // �˻��� ���ڿ��� ���� �޸� �Ҵ�
            char* find = malloc(CHAR_MAX);
            void* data;  // lookup ����� ������ ������

            printf("ã���� �ϴ� �����͸� �Է��ϼ��� : ");
            scanf("%s", find);

            // �˻� ����: data�� ã���� �ϴ� ���ڿ��� �ּ� �Ҵ� �� lookup �Լ� ȣ��
            data = find;
            if (chtbl_lookup(htbl, &data) == 0)
                // lookup ���� ��, �ش� ����� �ּҿ� �����͸� ���
                printf("��� �ּ�: %p, ��� ������: %s\n", data, (char*)data);
            else
                // lookup ���� �� �ش� �޽��� ���
                printf("�ش� ������ ��带 ã�� �� �����ϴ�.\n");

            free(find); // �˻��� ���� �ӽ� �޸� ����
        }
        // ��� 3: ������ ����
        else if (mode == 3)
        {
            // ������ �����͸� �Է¹ޱ� ���� �ӽ� �޸� �Ҵ�
            char* remove = malloc(CHAR_MAX);
            void* data;  // ������ �������� �ּҸ� ������ ������

            printf("�����ϰ��� �ϴ� �����͸� �Է��ϼ��� : ");
            scanf("%s", remove);
            data = remove;

            // ���� �Լ� ȣ��, ���� �� �޽��� ���
            if (chtbl_remove(htbl, &data) != 0)
                printf("�ش� ������ ��带 ã�� �� �����ϴ�.\n");

            free(remove); // ������ ���� �ӽ� �޸� ����
        }
        // ��� 4: ��ü ������ ��� (�ؽ� ���̺� ��ü ���)
        else if (mode == 4)
        {
            int i;
            ListElmt* member;

            // ��� ��Ŷ�� ��ȸ�ϸ� �� ��Ŷ�� ���� ����Ʈ�� ����� ��� ���
            for (i = 0; i < htbl->buckets; i++)
            {
                printf("��Ŷ %d:\n", i);
                for (member = list_head(&htbl->table[i]); member != NULL; member = member->next)
                {
                    // �� ����� �ּ�, ������, ���� ����� �ּ� ���
                    printf("\t��� �ּ�: %p, ��� ������: %s, ���� ��� �ּ�: %p\n", member, member->data, member->next);
                }
            }
        }
        // ��� 5: ����
        else if (mode == 5)
            break;

        // �� ���� �Է��� �ٽ� �ݺ������� ������ ����
        else
            continue;
    }

    // �ؽ� ���̺� ����ü�� �Ҵ�� �޸� ���� 
    free(htbl);
    return 0;
}

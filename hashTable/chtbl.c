#include "chtbl.h"

/*
 * �ؽ� ���̺� �ʱ�ȭ �Լ�
 * - htbl: �ؽ� ���̺� ����ü�� ���� ������
 * - buckets: �ؽ� ���̺��� ����� ��Ŷ(����Ʈ)�� ��
 * - h: �ؽ� �Լ� (key�� �޾� ���� ���� ��ȯ)
 * - match: �� key�� ��ġ�� ���ϴ� �Լ� (��ġ�ϸ� 0 ��ȯ)
 * - destroy: ������ ���� �� ȣ���� �Լ� (NULL�̸� ���� ����)
 *
 * ��ȯ��:
 *   0  - �ʱ�ȭ ����
 *  -1  - �޸� �Ҵ� ���� �� �ʱ�ȭ ����
 */
int chtbl_init(CHTbl* htbl, int buckets, int (*h)(const void* key),
    int (*match)(const void* key1, const void* key2),
    void (*destroy)(void* data))
{
    // �ؽ� ���̺��� ��Ŷ �迭(List �迭)�� �Ҵ�
    if ((htbl->table = (List*)malloc(buckets * sizeof(List))) == NULL)
        return -1;

    // �� ��Ŷ(���� ����Ʈ)�� �ʱ�ȭ (�� ����Ʈ�� ����)
    for (int i = 0; i < buckets; i++)
    {
        list_init(&htbl->table[i], destroy);
    }

    // �ؽ� ���̺� ��Ÿ������ ����
    htbl->buckets = buckets;  // �� ��Ŷ �� ���
    htbl->h = h;              // �ؽ� �Լ� ����
    htbl->match = match;      // ������ �� �Լ� ����
    htbl->destroy = destroy;  // ������ ���� �� ȣ���� �Լ� ����
    htbl->size = 0;           // ���� ������ ������ 0���� �ʱ�ȭ

    return 0;
}

/*
 * �ؽ� ���̺� ��ü ���� �Լ�
 * - htbl: ������ �ؽ� ���̺� ����ü�� ���� ������
 *
 * �� ��Ŷ�� �ִ� ��� ��带 �����ϰ�, �Ҵ��� ��Ŷ �迭��
 * �ؽ� ���̺� ����ü�� ������ �ʱ�ȭ�մϴ�.
 */
void chtbl_destroy(CHTbl* htbl)
{
    // ��� ��Ŷ(����Ʈ)�� ��ȸ�ϸ� �� ����Ʈ�� ��带 ����
    for (int i = 0; i < htbl->buckets; i++)
    {
        list_destroy(&htbl->table[i]);
    }
    // �Ҵ�� ��Ŷ �迭 �޸� ����
    free(htbl->table);

    // �ؽ� ���̺� ����ü ��ü�� 0���� �ʱ�ȭ (�����ϰ� ����)
    memset(htbl, 0, sizeof(CHTbl));
    return;
}

/*
 * �ؽ� ���̺� ������ ���� �Լ�
 * - htbl: �����͸� ������ �ؽ� ���̺� ����ü ������
 * - data: ������ ������(void*�� �Ǿ� �����Ƿ� ������ ������ ����)
 *
 * ���� ���� �ߺ� ������ ���θ� üũ�ϰ�, �ش��ϴ� ��Ŷ�� ���� ����Ʈ ���� �Լ���
 * list_ins_next�� ȣ���Ͽ� �����͸� �߰��մϴ�.
 *
 * ��ȯ��:
 *   0  - ���� ����
 *   1  - �ߺ� ������ �߰� (�������� ����)
 *  -1  - ���Կ� ����
 */
int chtbl_insert(CHTbl* htbl, const void* data)
{
    int bucket;
    int retval;

    // �ӽ� ���� temp�� ����Ͽ� �ߺ� üũ�� ���� ������ ����
    void* temp = (void*)data;

    // ������ �ߺ� Ȯ��: �̹� �����ϸ� �������� ����
    if (chtbl_lookup(htbl, &temp) == 0)
        return 1;

    // �����Ͱ� �� ��Ŷ ��� (�ؽ� �Լ� ����� ��Ŷ �� ���� ���� ������)
    bucket = htbl->h(data) % htbl->buckets;

    // �ش� ��Ŷ�� ����Ʈ�� �����͸� ���� (NULL�� ���ڷ� �����ϸ� ����Ʈ�� �� �տ� ����)
    if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0)
        htbl->size++;   // ���� ���� �� �ؽ� ���̺��� ������ ���� ����

    return retval;
}

/*
 * �ؽ� ���̺��� ������ ���� �Լ�
 * - htbl: �����͸� ������ �ؽ� ���̺� ����ü ������
 * - data: ������ �������� �����Ϳ� ���� �ּҸ� ���� (���ŵ� �������� �ּҷ� ����� �� ����)
 *
 * ������ �����Ͱ� ����� ��Ŷ���� ���� ����Ʈ�� ��ȸ�ϸ� ������ ��ġ ���θ� üũ�Ͽ�,
 * ��ġ�ϴ� ��带 list_rem_next �Լ��� ���� �����մϴ�.
 *
 * ��ȯ��:
 *   0  - ���� ����
 *  -1  - �ش� �����͸� ã�� �� �����̳� ���� ����
 */
int chtbl_remove(CHTbl* htbl, void** data)
{
    ListElmt* element;
    ListElmt* prev = NULL;
    int bucket;

    // ������ �����Ͱ� ����Ǿ� ���� ��Ŷ ���
    bucket = htbl->h(*data) % htbl->buckets;

    // �ش� ��Ŷ�� ����Ʈ�� ��ȸ�ϸ� ���� ��� ��带 ã��
    for (element = list_head(&htbl->table[bucket]); element != NULL; element = element->next)
    {
        // match �Լ��� 0�̸� �� �����Ͱ� ������
        if (htbl->match(*data, element->data) == 0)
        {
            // list_rem_next�� ȣ���Ͽ� �ش� ��带 ����
            if (list_rem_next(&htbl->table[bucket], prev, data) == 0)
            {
                htbl->size--;  // ��� ���� ���� �� ��ü ũ�⸦ ����
                return 0;
            }
            else
                return -1; // list_rem_next ���� ��
        }
        prev = element;
    }
    return -1; // ������ �����͸� ã�� ���� ���
}

/*
 * �ؽ� ���̺��� ������ ��ȸ �Լ�
 * - htbl: ��ȸ�� �ؽ� ���̺� ����ü ������
 * - data: ��ȸ�Ϸ��� �������� �����Ϳ� ���� �ּ�. ��ġ�ϴ� �����Ͱ� �ִ� ���,
 *         �ش� ��� �������� �ּҷ� �����.
 *
 * ��ȯ��:
 *   0  - ��ȸ ���� (�����Ͱ� ����)
 *  -1  - ������ �̹߰�
 */
int chtbl_lookup(const CHTbl* htbl, void** data)
{
    ListElmt* element;
    int bucket;

    // ��ȸ�� �����Ͱ� ����Ǿ� ���� ��Ŷ ���
    bucket = htbl->h(*data) % htbl->buckets;

    // �ش� ��Ŷ�� ����Ʈ�� ��ȸ�ϸ� ������ ��ġ ���� ����
    for (element = list_head(&htbl->table[bucket]); element != NULL; element = element->next)
    {
        // ������ ��ġ (match �Լ��� 0 ��ȯ ��)
        if (htbl->match(*data, element->data) == 0)
        {
            *data = element->data;  // ��ġ�ϴ� �����͸� data �����Ϳ� ����
            return 0;
        }
    }
    return -1;  // �����Ͱ� �������� ������ -1 ��ȯ
}

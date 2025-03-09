#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * list_init - ���� ����Ʈ �ʱ�ȭ
 * ----------------------------
 * ����Ʈ ����ü�� �� �ʵ带 �ʱ� ������ �����Ѵ�.
 *
 * �Ű�����:
 *   list     - �ʱ�ȭ�� ����Ʈ ����ü�� ���� ������
 *   destroy  - ��尡 ���ŵ� �� ������ ������ ���� ����� ���� �Լ� ������ (������ NULL)
 *
 * ����:
 *   ����Ʈ�� ũ�⸦ 0���� �����ϰ�, destroy �Լ� �����͸� ����ϸ�,
 *   head�� tail �����͸� NULL�� �ʱ�ȭ�Ѵ�.
 */
void list_init(List* list, void (*destroy)(void* data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
    return;
}

/*
 * list_destroy - ����Ʈ�� ��� ��� ���� �� ����ü �ʱ�ȭ
 * -------------------------------------------------------
 * ����Ʈ�� �ִ� ��� ��带 �����ϰ�, ��ϵ� destroy �Լ��� ����Ͽ�
 * �� ����� �����͸� ������ ��, ����Ʈ ����ü�� 0���� �ʱ�ȭ�Ѵ�.
 *
 * �Ű�����:
 *   list - ������ ������ ���Ե� ����Ʈ ����ü ������
 *
 * ����:
 *   list_rem_next() �Լ��� �ݺ� ȣ���Ͽ� ��带 ���������� �����ϸ�,
 *   ��ϵ� destroy �Լ��� ������ ȣ���Ͽ� ��� �����͵� �����Ѵ�.
 */
void list_destroy(List* list)
{
    void* data;

    // ����Ʈ�� ��尡 �����ִ� ���� �ݺ�
    while (list_size(list) > 0)
    {
        // element�� NULL�̸� head ��� ����. ���� �� data�� ���ŵ� ������ ����.
        if (list_rem_next(list, NULL, (void**)&data) == 0 && list->destroy != NULL)
        {
            // ����ڰ� ����� destroy �Լ��� �����͸� ����
            list->destroy(data);
        }
    }

    // ����Ʈ ����ü ��ü�� ������ 0���� �ʱ�ȭ
    memset(list, 0, sizeof(List));
    return;
}

/*
 * list_ins_next - ���� ����Ʈ�� ���ο� ��� ����
 * ------------------------------------------------
 * ������ ���(element)�� ���� ��ġ�� ���ο� ��带 �߰��Ѵ�.
 * ���� element�� NULL�̸� ����Ʈ�� �� ��(head)�� ���Եȴ�.
 *
 * �Ű�����:
 *   list    - ��带 ������ ��� ����Ʈ
 *   element - ������ �Ǵ� ���� ��� (NULL�̸� head�� ����)
 *   data    - �� ��忡 ������ ������ (void* ����)
 *
 * ��ȯ��:
 *   ���� ���� �� 0, �޸� �Ҵ� ���� �� ���� �� -1 ��ȯ
 *
 * ����:
 *   1. �� ��忡 ���� �޸� �Ҵ��� �õ��Ѵ�.
 *   2. element�� NULL�� ���, �� ��带 ����Ʈ�� ���� �κп� �����Ѵ�.
 *   3. �׷��� ������ element�� ���� ��ġ�� ��带 �����Ѵ�.
 *   4. ����Ʈ�� size�� ������Ű��, tail �������� ���� ���θ� üũ�Ѵ�.
 */
int list_ins_next(List* list, ListElmt* element, const void* data)
{
    ListElmt* new_element;

    // �� ��带 ���� �޸� �Ҵ� ���� �� -1 ��ȯ
    if ((new_element = (ListElmt*)malloc(sizeof(ListElmt))) == NULL)
        return -1;

    new_element->data = (void*)data;

    if (element == NULL) // ���� ��尡 ���� ���: ����Ʈ �� �տ� ����
    {
        if (list_size(list) == 0)
            list->tail = new_element;  // �� ����Ʈ�� ��� �� ��尡 tail�� �ȴ�.
        new_element->next = list->head;
        list->head = new_element;
    }
    else // ���� ��尡 �ִ� ���: element �ڿ� ����
    {
        if (element->next == NULL)
            list->tail = new_element;  // element�� ���� tail�̸�, �� ��尡 ���ο� tail�� ��.
        new_element->next = element->next;
        element->next = new_element;
    }

    list->size++;  // ����Ʈ�� �߰��� ��� �ϳ� ����
    return 0;
}

/*
 * list_rem_next - ���� ����Ʈ���� ���� ����� ���� ��� ����
 * ------------------------------------------------------------
 * ������ ���(element)�� ���� ��带 �����ϰ�, �ش� ����� �����͸�
 * data �����͸� ���� ��ȯ�Ѵ�. element�� NULL�̸� ����Ʈ�� head ��带 �����Ѵ�.
 *
 * �Ű�����:
 *   list    - ��带 ������ ��� ����Ʈ
 *   element - ���� ��� (NULL�̸� head ��带 ����)
 *   data    - ���ŵ� ����� �����͸� ������ ���� ������
 *
 * ��ȯ��:
 *   ���� ���� �� 0, ���� �� -1 ��ȯ
 *
 * ����:
 *   1. ����Ʈ�� ��������� ���� -1 ��ȯ.
 *   2. element�� NULL�� ���, head ��带 �����ϰ�,
 *      ����Ʈ�� head �����͸� ���� ���� �̵���Ų��.
 *   3. element�� �����ϸ�, element�� ���� ��尡 �����ϴ��� Ȯ�� �� ���� ó��.
 *   4. tail ������Ʈ �� �޸� ������ �����ϰ�, ����Ʈ�� size�� ���ҽ�Ų��.
 */
int list_rem_next(List* list, ListElmt* element, void** data)
{
    ListElmt* old_element;

    if (list_size(list) == 0)
        return -1;

    if (element == NULL) // head ��� ����
    {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1)
            list->tail = NULL;
    }
    else // element�� ������ ���
    {
        if (element->next == NULL)
            return -1;

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == NULL)
            list->tail = element;
    }
    free(old_element);
    list->size--;
    return 0;
}

/*
 * add_node - ����Ʈ�� ���ο� ��� ���� (����� �Է� ��� ���� ��� Ȱ��)
 * ----------------------------------------------------------------------------
 * ����Ʈ�� �� �����͸� �����ϱ� ����, ����ڰ� �������� ���� ���(Ȥ�� �ߺ� �߻� ��
 * ���� �ĺ�)�� ã�� �� ������ ��ġ�� �����ϴ� �Լ��̴�.
 *
 * �Ű�����:
 *   list  - �����͸� ������ ����Ʈ
 *   data  - �� ��忡 ������ ������
 *   input - ���� ���� ����� �����͸� �����ϱ� ���� ����� �Է� ��
 *
 * ����:
 *   1. ����Ʈ�� ũ�Ⱑ 2 �̻��̸�, ����Ʈ ��ü�� ��ȸ�ϸ� input�� ��ġ�ϴ� ������ ã�´�.
 *      ��ġ�ϴ� ������ �ӽ� �迭(address)�� �����Ѵ�.
 *   2. �ĺ� ��尡 ���� ���̸�, choice() �Լ��� ȣ���Ͽ� �� ��° �ĺ��� �������� ���� �����Ѵ�.
 *      ���õ� ��� �ڿ� �� ��带 �����Ѵ�.
 *   3. �ĺ��� �ϳ��̰ų� input�� NULL�� �ƴ� ��� ��ȸ�Ͽ� �ش� ��� �ڿ� �� ��带 �����Ѵ�.
 *   4. ����Ʈ�� ũ�Ⱑ 1 �����̸�, �ܼ��� ���� head �ڿ� �� ��带 �����Ѵ�.
 *   5. �������� �Ҵ��� �ӽ� �迭(address)�� ��� �� �����Ѵ�.
 *
 * ��ȯ��:
 *   ���� �� 0, ���� �� -1�� ��ȯ�Ѵ�.
 */
int add_node(List* list, const void* data, const void* input)
{
    ListElmt* current = list->head;
    ListElmt* check = list->head;

    // �Է°��� ��ġ�ϴ� ������ ������ �ӽ� �迭 (�Ŀ� �ĺ� ���ÿ� ���)
    ListElmt** address = malloc(list->size * sizeof(ListElmt*));
    if (address == NULL)
        return -1;

    int i = 0;
    if (list->size >= 2)
    {
        // ����Ʈ�� ��ȸ�ϸ� �Է°�(input)�� ��ġ�ϴ� ��带 ã�´�.
        while (check != NULL)
        {
            if (list->match(check->data, input))
            {
                address[i] = check;
                i++;
            }
            check = check->next;
        }
    }

    if (list->size >= 2)
    {
        if (i > 1)
        {
            int k = 0;
            // �ߺ� �ĺ��� ���� �����, ����ڰ� �� ��°�� �������� ���� ����
            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }
            // ���õ� �ĺ� ���(address[k]) �ڿ� �� �����͸� ����
            if (list_ins_next(list, address[k], data) != 0)
            {
                free(address);
                return -1;
            }
        }
        else
        {
            if (input != NULL)
            {
                // �ĺ��� �ϳ��̸�, �ش� ������ ��ȸ�ϸ� ã�Ƽ� �� �ڿ� ����
                while (current != NULL && list->match(current->data, input) == 0)
                    current = current->next;
                if (list_ins_next(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
            else
            {
                // input�� NULL�� ��쿡�� ����Ʈ�� head �ڿ� ����
                if (list_ins_next(list, NULL, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
        }
    }
    else
    {
        // ����Ʈ�� ũ�Ⱑ 0 �Ǵ� 1�̸�, ���� head �ڿ� ����
        if (list_ins_next(list, current, data) != 0)
        {
            free(address);
            return -1;
        }
    }
    free(address);
    return 0;
}

/*
 * rem_node - ����Ʈ���� ��ġ�ϴ� ��带 ����
 * -----------------------------------------------------------
 * ����Ʈ���� data�� ��ġ�ϴ� ��带 ������ ��, ���� �ĺ�(�ߺ��� �� ����)��
 * ���� ������ �ӽ� �迭�� �����ϰ�, �ߺ��� ������ ����ڰ� ������ ����� �����Ѵ�.
 *
 * �Ű�����:
 *   list - ��带 ������ ����Ʈ
 *   data - ���� ����� ������ (���� ����)
 *
 * ����:
 *   1. ����Ʈ ��ü�� ��ȸ�ϸ�, data�� ��ġ�ϴ� ��带 ã�´�. �� ��, �� ����� ���� ��� �����͸�
 *      �ӽ� �迭(address)�� �����Ѵ�.
 *   2. ����Ʈ�� ũ�Ⱑ 2���� ũ��:
 *      - ���� �ĺ��� ���� ���� ���, choice() �Լ��� �̿��� ������ �ĺ��� ������ ��,
 *        ���õ� �ĺ��� ���� ��带 �����Ѵ�.
 *      - ���� �ĺ��� �ϳ��̸�, ��ȸ�� ���� �ش� ��带 ã�� �� ���� ��� ���� ��带 �����Ѵ�.
 *   3. ����Ʈ�� ũ�Ⱑ 2 �����̸�, �ܼ��ϰ� head ���� ��带 �����Ѵ�.
 *   4. ��� �� �ӽ� �迭(address)�� �����Ѵ�.
 *
 * ��ȯ��:
 *   ���� �� 0, ���� �� -1�� ��ȯ�Ѵ�.
 */
int rem_node(List* list, const void* data)
{
    // ����Ʈ�� ��������� ������ �� ����
    if (list->size == 0)
        return -1;

    ListElmt* current = list->head;
    ListElmt* check = list->head;
    ListElmt* prev = NULL;

    // ��ġ�ϴ�(���� �ĺ�) ����� "���� ���" �����͵��� ������ �ӽ� �迭 �Ҵ�
    ListElmt** address = malloc(list->size * sizeof(ListElmt*));
    if (address == NULL)
        return -1;

    int i = 0;
    // ����Ʈ ��ü�� ��ȸ�ϸ�, data�� ��ġ�ϴ� ��带 ã��, �� ����� ���� ��带 address �迭�� ����
    while (check != NULL)
    {
        if (list->match(check->data, data))
        {
            address[i] = prev;  // �ĺ��� head�� ��� prev�� NULL�� ��
            i++;
        }
        prev = check;
        check = check->next;
    }

    int ret = 0;
    void* temp;  // list_rem_next �Լ����� ������ ����� �����͸� �޴� ����

    if (list->size > 2)
    {
        if (i > 1)
        {
            // ���� �ĺ��� �ִ� ���, ����ڰ� �� ��° �ĺ��� �������� ������ ����
            int k = 0;
            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }
            ret = list_rem_next(list, address[k], &temp);
            if (ret != 0)
            {
                free(address);
                return ret;
            }
        }
        else
        {
            // �ĺ��� �ϳ��� ��� : ���ʱ�ȭ�Ͽ� ���� ��� Ž��
            // �̷��� �����ν� ���� ���� ����� head�̸� prev�� NULL�� �Ǿ�, head ������ �ùٸ��� ó����.
            current = list->head;
            prev = NULL;
            while (current != NULL && !list->match(current->data, data))
            {
                prev = current;
                current = current->next;
            }
            ret = list_rem_next(list, prev, &temp);
            if (ret != 0)
            {
                free(address);
                return ret;
            }
        }
    }
    else
    {
        // ����Ʈ�� ũ�Ⱑ 2 ������ ���:
        // ���� head ��� ��ü�� ���� ����̸� element�� NULL�� �����Ͽ� head ��带 �����Ѵ�.
        if (list->match(list->head->data, data))
            ret = list_rem_next(list, NULL, &temp);
        else
            ret = list_rem_next(list, list->head, &temp);

        if (ret != 0)
        {
            free(address);
            return ret;
        }
    }
    free(address);
    return 0;
}


/*
 * choice - �ߺ� ��忡 ���� ���� ���� �Լ�
 * -----------------------------------------------------------
 * �ߺ��� ��尡 ���� �� ���� ���, ����ڰ� �� ��° ��带
 * �������� ���� �Է¹޴´�.
 *
 * �Ű�����:
 *   sequence - ����� �Է��� ������ ������ ������
 *
 * ��ȯ��:
 *   scanf�� ��ȯ�� (���� �Է� �� 1, �Է� ���� �� ���� �ڵ�)
 */
int choice(int* sequence)
{
    printf("�� ��° �ߺ� ��带 �������� �Ͻðڽ��ϱ�?\n");
    return scanf("%d", sequence);
}

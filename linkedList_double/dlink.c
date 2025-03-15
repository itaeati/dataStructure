#include "dlist.h"

/*
 * dlist_init - ���� ���� ����Ʈ �ʱ�ȭ �Լ�
 * ----------------------------------------------------------------------------
 * �� �Լ��� ���� ���� ����Ʈ�� �ʱ� ���·� �����Ѵ�.
 * ����Ʈ�� ũ�⸦ 0���� �ʱ�ȭ�ϰ�, ������ �Ҹ� �Լ� ������(destroy) ��
 * ����Ʈ�� head�� tail�� NULL�� �����Ѵ�.
 *
 * �Ű�����:
 *   list    - �ʱ�ȭ�� ���� ���� ����Ʈ�� ������
 *   destroy - ����Ʈ �� ������ �Ҹ� �� ȣ���� �Լ� ������ (������ NULL)
 *
 * ��ȯ��:
 *   ����.
 */
void dlist_init(DList* list, void (*destroy)(void* data))
{
    list->size = 0;         // ����Ʈ �� ��� ������ 0���� �ʱ�ȭ
    list->destroy = destroy;  // ����� ���� ������ �Ҹ� �Լ� ����
    list->head = NULL;      // ����Ʈ�� ���� ��带 NULL�� ����
    list->tail = NULL;      // ����Ʈ�� ������ ��带 NULL�� ����

    return;
}


/*
 * dlist_destroy - ���� ���� ����Ʈ�� ��� ��带 �����ϴ� �Լ�
 * ----------------------------------------------------------------------------
 * �� �Լ��� ����Ʈ�� ����� ������ tail ��带 ���������� �����Ѵ�.
 * ���� ���� ��, ���ŵ� ����� �����Ͱ� �����ϰ� �Ҹ� �Լ��� ��ϵǾ� �ִٸ�,
 * �ش� �����͸� �Ҹ� ó���Ѵ�.
 * ���������� ����Ʈ ����ü�� �޸𸮸� 0���� �ʱ�ȭ�Ѵ�.
 *
 * �Ű�����:
 *   list - ������ ��带 �����ϴ� ���� ���� ����Ʈ�� ������
 *
 * ��ȯ��:
 *   ����.
 */
void dlist_destroy(DList* list)
{
    void* data;

    // ����Ʈ�� ��尡 �ִ� ���� tail ��带 �����Ѵ�.
    while (dlist_size(list) > 0)
    {
        // tail ��带 �����ϰ�, ���ŵ� �����Ͱ� �ִٸ� �Ҹ� �Լ��� ó��
        if (dlist_remove(list, dlist_tail(list), (void**)&data) == 0 && list->destroy != NULL)
        {
            list->destroy(data);
        }
    }

    // ����Ʈ ����ü�� ��� ����� 0���� �ʱ�ȭ���� �ܿ� �����͸� ����
    memset(list, 0, sizeof(DList));

    return;
}


/*
 * dlist_ins_next - ���� ���� ����Ʈ�� ������ ��� �ڿ� �� ��带 �����ϴ� �Լ�
 * ----------------------------------------------------------------------------
 * �� �Լ��� ������ ���� ���(element)�� ���ʿ� ���ο� ��带 �����Ѵ�.
 * ���� ����Ʈ�� ����ִٸ�, �� ��带 head�� tail�� �����Ѵ�.
 *
 * �Ű�����:
 *   list    - �����͸� ������ ���� ���� ����Ʈ�� ������
 *   element - �� ��带 ������ ���� ��� (����Ʈ�� ������� ���� ��� NULL�̸� ����)
 *   data    - �� ��忡 ������ ������
 *
 * ��ȯ��:
 *   �����ϸ� 0, �����ϸ� -1�� ��ȯ�Ѵ�.
 */
int dlist_ins_next(DList* list, DListElmt* element, const void* data)
{
    DListElmt* new_element;

    // ����Ʈ�� ������� ������ element�� NULL�̸� ������ �� ����
    if (element == NULL && dlist_size(list) != 0)
        return -1;

    // �� ��� �޸� �Ҵ� ���� �� ���� ��ȯ
    if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
        return -1;

    new_element->data = (void*)data;  // �� ��忡 ������ ������ ����

    if (dlist_size(list) == 0)  // ����Ʈ�� ����ִ� ���
    {
        // �� ��带 ����Ʈ�� ������ ���� ����
        list->head = new_element;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_element;
    }
    else  // ����Ʈ�� �̹� ��尡 �����ϴ� ���
    {
        // ���� ���(element)�� �ڿ� �� ��带 ����
        new_element->next = element->next;  // �� ����� next�� ���� ����� ���� ���
        new_element->prev = element;          // �� ����� prev�� ���� ��带 ����Ŵ

        if (element->next == NULL)
            list->tail = new_element;  // ���� ��尡 tail�̸�, �� ��尡 ���ο� tail�� ��
        else
            element->next->prev = new_element;  // ���� ���� ����� prev�� �� ���� ����

        element->next = new_element;  // ���� ����� next�� �� ���� ����
    }

    list->size++;  // ����Ʈ ũ�� ����

    return 0;  // ���� ����
}


/*
 * dlist_ins_prev - ���� ���� ����Ʈ�� ������ ��� �տ� �� ��带 �����ϴ� �Լ�
 * ----------------------------------------------------------------------------
 * �� �Լ��� ������ ���� ���(element)�� ���ʿ� ���ο� ��带 �����Ѵ�.
 * ���� ����Ʈ�� ���������, �� ��带 head�� tail�� �����Ѵ�.
 *
 * �Ű�����:
 *   list    - �����͸� ������ ���� ���� ����Ʈ�� ������
 *   element - �� ��带 ������ ���� ��� (����Ʈ�� ������� ���� ��� NULL�̸� ����)
 *   data    - �� ��忡 ������ ������
 *
 * ��ȯ��:
 *   �����ϸ� 0, �����ϸ� -1�� ��ȯ�Ѵ�.
 */
int dlist_ins_prev(DList* list, DListElmt* element, const void* data)
{
    DListElmt* new_element;

    // ����Ʈ�� ������� ������ ���� ���(element)�� NULL�̸� ������ �� ����
    if (element == NULL && list->size != 0)
        return -1;

    // �� ��� ���� ���� �� ���� ��ȯ
    if ((new_element = (DListElmt*)malloc(sizeof(DListElmt))) == NULL)
        return -1;

    new_element->data = (void*)data;  // �� ��忡 ������ ������ ����

    if (dlist_size(list) == 0)  // ����Ʈ�� ����ִ� ���
    {
        // �� ��带 ������ ���� ����
        list->head = new_element;
        list->tail = new_element;
        list->head->prev = NULL;
        list->tail->next = NULL;
    }
    else  // ����Ʈ�� ��尡 �����ϴ� ���
    {
        // ���� ���(element)�� ���ʿ� �� ��带 ����
        new_element->next = element;           // �� ����� next�� ���� ��带 ����Ŵ
        new_element->prev = element->prev;       // �� ����� prev�� ���� ����� ���� ��带 ����Ŵ

        if (element->prev == NULL)
            list->head = new_element;  // ���� ��尡 head�̸�, �� ��尡 ���ο� head�� ��
        else
            element->prev->next = new_element;  // ���� ���� ����� next�� �� ���� ����

        element->prev = new_element;  // ���� ����� prev�� �� ���� ����
    }
    list->size++;  // ����Ʈ ũ�� ����

    return 0;  // ���� ����
}


/*
 * dlist_remove - ���� ���� ����Ʈ���� ������ ��带 �����ϴ� �Լ�
 * ----------------------------------------------------------------------------
 * �� �Լ��� ������ ���(element)�� ����Ʈ���� �����ϰ�,
 * ���ŵ� ����� �����͸� data �����͸� ���� ȣ���ڿ��� ��ȯ�Ѵ�.
 * ����, ���� �� ����Ʈ �� ���� ���踦 �������ϰ�, �Ҵ�� �޸𸮸� �����Ѵ�.
 *
 * �Ű�����:
 *   list    - ��带 ������ ���� ���� ����Ʈ�� ������
 *   element - ������ ��带 ����Ű�� ������
 *   data    - ���ŵ� ����� �����͸� ������ �������� �ּ�
 *
 * ��ȯ��:
 *   �����ϸ� 0, �����ϸ� -1�� ��ȯ�Ѵ�.
 */
int dlist_remove(DList* list, DListElmt* element, void** data)
{
    // ������ ��尡 NULL�̰ų� ����Ʈ�� ��尡 ���� ��� ���� ��ȯ
    if (element == NULL || dlist_size(list) == 0)
        return -1;

    *data = element->data;  // �����ϴ� ����� �����͸� ȣ���ڿ��� ��ȯ

    if (element == list->head)
    {
        // ������ ��尡 head�� ���, head�� ���� ���� ����
        list->head = element->next;

        if (list->head == NULL)
            // ����Ʈ�� ���� ��尡 ������ tail�� NULL�� ����
            list->tail = NULL;
        else
            // �� head�� prev �����͸� NULL�� �ʱ�ȭ
            element->next->prev = NULL;
    }
    else
    {
        // ������ ��尡 �߰� �Ǵ� tail�� ���, ���� ���� ���� ��带 ����
        element->prev->next = element->next;

        if (element->next == NULL)
            // ������ ��尡 tail�̸� tail�� ���� ���� ����
            list->tail = element->prev;
        else
            element->next->prev = element->prev;  // �߰� ����� ���� ����� prev ����
    }

    free(element);   // ���ŵ� ����� �޸� ����
    list->size--;    // ����Ʈ �� ��� ������ 1 ����

    return 0;  // ���� ����
}


/*
 * add_node - ���� ���� ����Ʈ�� �� ��带 ���� ����� �� �Ǵ� �ڿ� �����ϴ� �Լ�
 * ----------------------------------------------------------------------------
 * ����:
 *   �� �Լ��� ���� ���� ����Ʈ(list)�� ����, ����ڰ� ������ �Է�(input)�� ��������
 *   ������ ��ġ�� �����Ͽ�, �ش� ��ġ �ٷ� ���̳� �� (mode�� ����)�� �� ��带 �߰��մϴ�.
 *   ���� ���� ������(input)�� ��ġ�ϴ� ��尡 ���� �� �����Ѵٸ�, ����ڰ� ����(choice())
 *   �� ���� ���ϴ� �ĺ� ��带 ������ �� �ֽ��ϴ�.
 *
 * �Ű� ����:
 *   list  - �� ��尡 ���Ե� ��� ���� ���� ����Ʈ
 *   data  - �� ��忡 ������ ������ (�Һ�)
 *   input - ���� ���� ��带 �����ϱ� ���� ����� �Է� ������
 *   mode  - ���� ��� (1: ���� ����� �ڿ� ����, 2: ���� ����� �տ� ����)
 *
 * ��ȯ��:
 *   ���Կ� �����ϸ� 0, �޸� �Ҵ� ���г� ���� ���� �� ������ ������ -1�� ��ȯ�Ѵ�.
 */
int add_node(DList* list, const void* data, const void* input, int mode)
{
    /*
     * current:
     *    ����Ʈ ��ȸ �� ������ �Ǵ� ��� ������
     * check:
     *    input ���� ��Ī�Ǵ� ��带 ã�� ���� ����Ʈ�� ��ȸ�� �� ����ϴ� ������
     */
    DListElmt* current = list->head;
    DListElmt* check = list->head;

    /*
     * address:
     *    input�� ��ġ�ϴ� �ĺ� ������ �����ϱ� ���� �ӽ� �迭.
     *    ����Ʈ�� ��ü ũ�⸸ŭ ������ �Ҵ��ϸ�, �ִ� �ĺ� ���� list->size�� ����.
     */
    DListElmt** address = malloc(list->size * sizeof(DListElmt*));
    if (address == NULL)
        return -1;  // �޸� �Ҵ� ���� ��

    int i = 0;  // input�� ��ġ�ϴ� �ĺ� ����� ������ ����.

    /*
     * ����Ʈ�� ũ�Ⱑ 2 �̻��̸�, ��ü ��带 ��ȸ�ϸ鼭 input ���� ��ġ�ϴ� ��带 ã�´�.
     * ã�� ��� �ش� ����� �ּҸ� address �迭�� �����ϰ� �ĺ� ����(i)�� ������Ų��.
     */

    if (list->size >= 2)
    {
        while (check != NULL)
        {
            // list->match() �Լ��� ���� ����� �����Ϳ� input�� ���Ѵ�.
            if (list->match(check->data, input) == 1)
            {
                address[i] = check;  // ��ġ�ϸ� �ĺ� �迭�� ����
                i++;
            }
            check = check->next;
        }
    }

    /*
     * ��� 1: ���� ����� "��"�� �� ��带 �����ϴ� ���
     */

    if (mode == 1)
    {
        if (i > 1)
        {
            /*
             * �ĺ� ��尡 ���� ���� ���, choice() �Լ��� ���� ����ڰ� ���ϴ� �ĺ�
             * (�� �� ��° �ĺ��� �������� ����)�� �����Ѵ�.
             */

            int k = 0;  // ���õ� �ĺ� �ε���

            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }

            // ������ �ĺ�(address[k])�� �ڿ� �� �����͸� �����ϴ� ��带 ����
            if (dlist_ins_next(list, address[k], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else if (i == 1)
        {
            // �ĺ��� 1���̸�, address[0]�� ������ �ĺ��̹Ƿ� �ٷ� ���
            if (dlist_ins_next(list, address[0], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else
        {
            /*
             * �ĺ��� �ϳ��̰ų�(�Ǵ� ���� ���ų�) input ���� �����ϸ�,
             * ����Ʈ�� ��ȸ�Ͽ� input�� ��ġ�ϴ� ù ��° ��带 ã�� �� �ڿ� �����Ѵ�.
             */

            if (input != NULL)
            {
                while (current != NULL && list->match(current->data, input) == 0)
                    current = current->next;
                if (dlist_ins_next(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
            else
            {
                /* input�� NULL�̸�, �⺻������ ����Ʈ�� head(�Ǵ� current) �ڿ� ���� */
                if (dlist_ins_next(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
        }
        free(address);
        return 0;
    }

    /*
     * ��� 2: ���� ����� "��"�� �� ��带 �����ϴ� ���
     * (���� ���������� dlist_ins_next()�� ����ϹǷ� ���� ��ġ�� ��� ó������
     *  �ణ�� �߰� ���� �ʿ��� �� ����)
     */
    if (mode == 2)
    {
        if (i > 1)
        {
            /*
             * �ĺ� ��尡 ���� ���� ��쿡��, choice() �Լ��� ���� ����ڰ� ������ �ĺ��� ����Ѵ�.
             */
            int k = 0;
            if (choice(&k) != 1)
            {
                free(address);
                return -1;
            }
            // ���õ� �ĺ� ���(address[k])�� ��(�Ǵ� ������ ��ġ)�� �� ��带 ����
            if (dlist_ins_prev(list, address[k], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else if (i == 1)
        {
            // �ĺ��� 1���̸�, address[0]�� ������ �ĺ��̹Ƿ� �ٷ� ���
            if (dlist_ins_prev(list, address[0], data) != 0)
            {
                free(address);
                return -1;
            }
        }

        else
        {
            /*
             * �ĺ��� �ϳ��̰ų� input�� �����ϴ� ���, ����Ʈ ��ȸ�� ���� ���� ��带 ã��
             * �ش� ��� ��(�Ǵ� ������ ��ġ)�� �����Ѵ�.
             */
            if (input != NULL)
            {
                while (current != NULL && list->match(current->data, input) == 0)
                    current = current->next;
                if (dlist_ins_prev(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
            else
            {
                // input�� ���� ��� �⺻������ head(�Ǵ� current)�� ������� ����
                if (dlist_ins_prev(list, current, data) != 0)
                {
                    free(address);
                    return -1;
                }
            }
        }
        
    }

    free(address);
    return 0;
}


/*
 * rem_node - Ư�� �����Ϳ� ��ġ�ϴ� ��带 �����ϴ� �Լ�
 * ----------------------------------------------------------------------------
 * �� �Լ��� ���� ����Ʈ ������ �Է¹��� data�� ��ġ�ϴ� ��带 ���� �˻��Ͽ� �ĺ� �迭(address)��
 * ������ ��, ����Ʈ�� ��ü ũ��� �ĺ� ������ ���� ������ ���� ���� ����� �����Ѵ�.
 *
 *  1. ����Ʈ�� ũ�Ⱑ 3�� �̻��� ���:
 *     - �ĺ��� ���� ��(i > 1)�� ���, choice() �Լ��� ȣ���Ͽ� ����ڰ� ������ �ĺ��� �ε����� �����ϰ�
 *       �ش� ��带 �����Ѵ�.
 *     - �ĺ��� �� �ϳ����, while ������ ���� ��ġ�ϴ� ù ��° ��带 ã�Ƽ� �����Ѵ�.
 *
 *  2. ����Ʈ�� ũ�Ⱑ 2�� ������ ���:
 *     - ����Ʈ�� ũ�Ⱑ 1�̸� �ܼ��� head ��带 �����Ѵ�.
 *     - ����Ʈ�� ũ�Ⱑ 2�̸� while ������ ���� �Է� data�� ��ġ�ϴ� ��带 �˻��Ͽ� �����Ѵ�.
 *
 * �Ű�����:
 *   list - ��带 ������ ���� ���� ����Ʈ�� ������
 *   data - ������ ����� �����Ϳ� ���� ��
 *
 * ��ȯ��:
 *   ������ �����ϸ� 0��, �����ϸ� -1�� ��ȯ�Ѵ�.
 */
int rem_node(DList* list, const void* data)
{
    // current: ������ �ĺ��� ã�� ���� ��ȸ ������
    // check: ��ü ����Ʈ�� ��ȸ�ϸ� data�� ��ġ�ϴ� ��带 �˻��� ������
    DListElmt* current = list->head;
    DListElmt* check = list->head;

    // �ĺ� ������ ������ �迭 (����Ʈ ũ�⸸ŭ �Ҵ�)
    DListElmt** address = malloc(list->size * sizeof(DListElmt*));
    void* temp;
    int i = 0;  // ��Ī�� �ĺ��� ������ ������ ����

    // �޸� �Ҵ� ���� �� ���� ��ȯ
    if (address == NULL)
    {
        // address�� NULL�� �� free() ȣ���� ���ʿ�������, �����ϰ� ó��
        free(address);
        return -1;
    }

    // ��ü ����Ʈ�� ��ȸ�ϸ�, �Է� data�� ��ġ�ϴ� ��带 candidate �迭�� ����
    while (check != NULL)
    {
        // list->match() �Լ��� ��ġ�ϸ� 0�� ��ȯ�Ѵٰ� ����
        if (list->match(check->data, data))
        {
            address[i] = check;
            i++;
        }
        check = check->next;
    }

    /* �б�: ����Ʈ ��ü ũ�⿡ ���� ���� ��� ���� */

    // ����Ʈ�� ũ�Ⱑ 2�� �̻��� ��� ó��
    if (list->size >= 2)
    {
        // �ĺ� ��尡 ���� ���� ���: ����� �Է�(choice)�� ���� ������ �ε��� ����
        if (i >= 1)
        {
            int k = 0;

            if (choice(&k) != 1)  // ����ڰ� �ùٸ� ������ ���� ���ϸ� ���� ó��
            {
                free(address);
                return -1;
            }

            // ���õ� �ĺ�(address[k])�� ����
            if (dlist_remove(list, address[k], &temp) != 0)
            {
                free(address);
                return -1;
            }
        }
        // �ĺ��� �� ���� ���� ���: while ������ ���� ù ��° ��ġ�ϴ� ��� Ž�� �� ����
        else
        {
            // !list->match() ������ match()�� 0�̸� false�� �ǹǷ�,
            // ��ġ�ϴ� ��带 ã�� ���� "match(...) != 0" ��� "not(match(...))"�� ����ص� ��.
            // ���⼭�� "while (current != NULL && !list->match(current->data, data))"��,
            // ��ġ���� �ʴ� ��带 �ǳʶٸ�, ��ġ�ϴ� ��尡 ������ ������ �����Ѵ�.
            while (current != NULL && !list->match(current->data, data))
                current = current->next;

            if (dlist_remove(list, current, &temp) != 0)
            {
                free(address);
                return -1;
            }
        }
    }
    // ����Ʈ�� ũ�Ⱑ 2�� ������ ��� ó��
    else
    {
        // ����Ʈ �� ��尡 �� ���̸� head�� �ܼ��� ����
        if (list->size == 1 && dlist_remove(list, list->head, &temp) != 0)
        {
            free(address);
            return -1;
        }
        else
        {
            // ����Ʈ�� ũ�Ⱑ 2�� ���: head�� tail �� �Է� data�� ��ġ�ϴ� ��带 ã��
            while (current != NULL && !list->match(current->data, data))
                current = current->next;

            if (dlist_remove(list, current, &temp) != 0)
            {
                free(address);
                return -1;
            }
        }
    }

    // ���� �Ҵ��� �ĺ� �迭 �޸� ���� �� ���� ����
    free(address);
    return 0;
}

int choice(int* sequence) // �ߺ� ������ ���� �Լ�
{
    printf("�� ��° �ߺ� ��带 �������� �Ͻðڽ��ϱ�?\n");
    return scanf("%d", sequence);
} // �Է¹��� ���� sequence�� �����ϰ�, ��ȯ������ �Է� ���� ���θ� ��ȯ�մϴ�.
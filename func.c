#include "calculate.h"

/* Validates both numeric operands (optional sign + digits) and the single-character operator */
Status validate(char **argv)
{
    int i;
    i = 0;
    /* Validate first operand */
    if (argv[1][i] == '+' || argv[1][i] == '-')
        i++;
    if (argv[1][i] == '\0')
        return MISMATCH1;
    for (; argv[1][i]; i++)
    {
        if (!isdigit((unsigned char)argv[1][i]))
            return MISMATCH1;
    }
    /* Validate second operand */
    i = 0;
    if (argv[3][i] == '+' || argv[3][i] == '-')
        i++;
    if (argv[3][i] == '\0')
        return MISMATCH2;
    for (; argv[3][i]; i++)
    {
        if (!isdigit((unsigned char)argv[3][i]))
            return MISMATCH2;
    }
    /* Validate operator */
    int len = strlen(argv[2]);
    if (len != 1)
        return MISOP;
    char ch = argv[2][0];
    if ((ch != '+') && (ch != '-') &&
        (ch != '*') && (ch != '/') &&
        (ch != '%') && (ch != '^'))
    {
        return MISOP;
    }
    return SUCCESS;
}
/* Parses an operand string from right to left into 4-digit (BASE 10000) list nodes */
Status convert_to_list(Dlist **head, Dlist **tail, char *argv)
{
    int left = strlen(argv) - 1;
    int start = 0;
    /* Skip leading sign character */
    if (argv[0] == '+' || argv[0] == '-')
        start = 1;
    /* Read chunks of 4 digits from least significant to most significant */
    while (left >= start)
    {
        char buffer[5];
        int i = 4;
        buffer[i--] = '\0';
        while (i >= 0)
        {
            if (left < start)
                buffer[i--] = '0';
            else
                buffer[i--] = argv[left--];
        }
        int num;
        sscanf(buffer, "%d", &num);
        if (insert_at_first(head, tail, num) == FAILURE)
            return FAILURE;
    }
    /* Strip redundant leading zero nodes */
    remove_leading_zeros(head, tail);
    return SUCCESS;
}
/* Inserts a new node at the head (most significant end) of the doubly linked list */
Status insert_at_first(Dlist **head,Dlist **tail,int data)
{
    Dlist *new = (Dlist*)malloc(sizeof(Dlist));
    if(new == NULL) return FAILURE;
    if((*head) == NULL || (*tail) == NULL)
    {
        new -> data = data;
        new -> prev = NULL;
        new -> next = (*head);
        (*head) = new;
        (*tail) = new;
        return SUCCESS;
    }
    new -> data = data;
    new -> next = (*head);
    new -> prev = NULL;
    (*head) -> prev = new;
    (*head) = new;
    return SUCCESS;
}
/* Inserts a new node at the tail (least significant end) of the doubly linked list */
Status insert_at_last(Dlist **head,Dlist **tail,int data)
{
    Dlist *new = (Dlist*)malloc(sizeof(Dlist));
    if(new == NULL) return FAILURE;
    if((*head) == NULL || (*tail) == NULL)
    {
        new -> data = data;
        new -> prev = (*tail);
        new -> next = NULL;
        (*head) = new;
        (*tail) = new;
        return SUCCESS;
    }
    new -> data = data;
    new -> next = NULL;
    new -> prev = (*tail);
    (*tail) -> next = new;
    (*tail) = new;
    return SUCCESS;
}
/* Prints the big integer from head to tail, zero-padding non-head nodes to 4 digits */
Status print_list(Dlist *head,Dlist *tail)
{
    Dlist *temp = (head);
    if(head == NULL) return LIST_EMPTY;
    while(temp != NULL)
    {
        if(temp == head)
        printf("%d ",temp -> data);
        else 
        printf("%04d ",temp -> data);
        temp = temp -> next;
    }
    return SUCCESS;
}
/* Compares magnitudes of two numbers: returns 1 if num1 > num2, -1 if num1 < num2, 0 if equal */
int compare_numbers(Dlist *head1, Dlist *tail1,Dlist *head2, Dlist *tail2)
{
    int count1 = 0, count2 = 0;
    Dlist *temp1 = head1;
    Dlist *temp2 = head2;
    /* Count total nodes (chunks) in each operand */
    while (temp1 != NULL)
    {
        count1++;
        temp1 = temp1->next;
    }
    while (temp2 != NULL)
    {
        count2++;
        temp2 = temp2->next;
    }
    /* More chunks means a larger magnitude */
    if (count1 > count2)
        return 1;
    if (count1 < count2)
        return -1;
    /* Equal chunk count: compare node by node from most significant to least significant */
    temp1 = head1;
    temp2 = head2;
    while (temp1 != NULL && temp2 != NULL)
    {
        if (temp1->data > temp2->data)
            return 1;
        if (temp1->data < temp2->data)
            return -1;
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    return 0;
}
/* Frees all nodes in a doubly linked list and resets head and tail to NULL */
void delete_list(Dlist **head,Dlist **tail)
{
    Dlist* current = *head;
    Dlist* next = NULL;
    while (current != NULL) {
        next = current->next; 
        free(current);           
        current = next;       
    }
    *head = NULL;
    *tail = NULL;
}
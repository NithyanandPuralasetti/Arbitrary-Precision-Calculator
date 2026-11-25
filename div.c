#include "calculate.h"

/* Removes redundant leading zero chunks while preserving at least one node */
void remove_leading_zeros(Dlist **head, Dlist **tail)
{
    while (*head != NULL && (*head)->data == 0 && (*head)->next != NULL)
    {
        Dlist *temp = *head;
        *head = (*head)->next;
        (*head)->prev = NULL;
        free(temp);
    }
}
/* Creates a deep copy of a doubly linked list */
Status copy_list(Dlist *head, Dlist **newhead, Dlist **newtail)
{
    while (head != NULL)
    {
        if (insert_at_last(newhead, newtail, head -> data) == FAILURE)
        {
            delete_list(newhead, newtail);
            return FAILURE;
        }
        head = head->next;
    }
    return SUCCESS;
}
/* Checks whether all nodes in the list have value 0 */
int is_zero(Dlist *head)
{
    while (head != NULL)
    {
        if (head->data != 0)
            return 0;
        head = head->next;
    }
    return 1;
}
/* Multiplies a big integer list by a single base-10000 integer q */
Status mul_num(Dlist *divisortail, Dlist **head, Dlist **tail, int q)
{
    int carry = 0;
    long long int mul;
    Dlist *temp1 = divisortail;
    while (temp1 != NULL || carry != 0)
    {
        mul = 0;
        if (temp1 != NULL)
        {
            mul = (long long int)(temp1->data) * q;
        }
        mul += carry;
        carry = mul / BASE;
        if (insert_at_first(head, tail, mul % BASE) == FAILURE)
        {
            delete_list(head, tail);
            return FAILURE;
        }
        if (temp1 != NULL)
            temp1 = temp1->prev;
    }
    if (*head != NULL)
        remove_leading_zeros(head, tail);

    return SUCCESS;
}
/* Uses binary search over [0, BASE - 1] to find the largest quotient chunk q such that divisor * q <= current */
Status find_q(Dlist *divisorhead, Dlist *divisortail,Dlist *currenthead, Dlist *currenttail,int *quotient,Dlist **resulthead, Dlist **resulttail)
{
    int low = 0;
    int high = BASE - 1;
    int q = 0;
    Dlist *temphead = NULL;
    Dlist *temptail = NULL;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (mul_num(divisortail, &temphead, &temptail, mid) == FAILURE)
            return FAILURE;
        int cmp = compare_numbers(temphead, temptail,currenthead, currenttail);
        if (cmp <= 0)
        {
            q = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
        delete_list(&temphead, &temptail);
    }
    *quotient = q;

    /* Compute final product (divisor * q) to subtract from current dividend slice */
    if (mul_num(divisortail, resulthead, resulttail, q) == FAILURE)
        return FAILURE;
    return SUCCESS;
}
/* Computes integer quotient and remainder of dividend / divisor using base-10000 long division */
Status division(Dlist *head1, Dlist *tail1,Dlist *head2, Dlist *tail2,Dlist **quohead, Dlist **quotail,Dlist **remhead, Dlist **remtail)
{
    /* Guard against division by zero */
    if (is_zero(head2))
        return FAILURE;
    /* Handle trivial cases where dividend == divisor or dividend < divisor */
    int cmp = compare_numbers(head1, tail1, head2, tail2);
    if (cmp == 0)
    {
        if (insert_at_first(quohead, quotail, 1) == FAILURE)
            return FAILURE;
        if (insert_at_first(remhead, remtail, 0) == FAILURE)
        {
            delete_list(quohead, quotail);
            return FAILURE;
        }
        return SUCCESS;
    }
    else if (cmp < 0)
    {
        if (insert_at_first(quohead, quotail, 0) == FAILURE)
            return FAILURE;
        if (copy_list(head1, remhead, remtail) == FAILURE)
        {
            delete_list(quohead, quotail);
            return FAILURE;
        }
        return SUCCESS;
    }
    /* Initialize running remainder to 0 */
    if (insert_at_first(remhead, remtail, 0) == FAILURE)
        return FAILURE;
    Dlist *temp = head1;
    Dlist *currenthead = NULL;
    Dlist *currenttail = NULL;
    Dlist *reshead = NULL;
    Dlist *restail = NULL;
    Dlist *newremhead = NULL;
    Dlist *newremtail = NULL;
    /* Bring down one base-10000 chunk of the dividend per step */
    while (temp != NULL)
    {
        int q = 0;
        /* Prepend previous remainder and append next dividend chunk */
        if (copy_list(*remhead, &currenthead, &currenttail) == FAILURE)
            goto FAILURE_CLEANUP;
        if (is_zero(currenthead))
            delete_list(&currenthead, &currenttail);
        if (insert_at_last(&currenthead, &currenttail,temp->data) == FAILURE)
            goto FAILURE_CLEANUP;
        remove_leading_zeros(&currenthead, &currenttail);
        /* Find quotient digit q and compute divisor * q */
        if (find_q(head2, tail2,currenthead, currenttail,&q, &reshead, &restail) == FAILURE)
            goto FAILURE_CLEANUP;
        if (insert_at_last(quohead, quotail, q) == FAILURE)
            goto FAILURE_CLEANUP;
        /* Subtract (divisor * q) from current slice to get updated remainder */
        if (sub(currenthead, currenttail,reshead, restail,&newremhead, &newremtail) == FAILURE)
            goto FAILURE_CLEANUP;
        remove_leading_zeros(&newremhead, &newremtail);
        /* Replace old remainder with new remainder and free temporary lists */
        delete_list(remhead, remtail);
        *remhead = newremhead;
        *remtail = newremtail;
        newremhead = NULL;
        newremtail = NULL;
        delete_list(&currenthead, &currenttail);
        delete_list(&reshead, &restail);
        temp = temp->next;
    }
    remove_leading_zeros(quohead, quotail);
    return SUCCESS;
    FAILURE_CLEANUP:
    delete_list(&currenthead, &currenttail);
    delete_list(&reshead, &restail);
    delete_list(&newremhead, &newremtail);
    delete_list(quohead, quotail);
    delete_list(remhead, remtail);
    return FAILURE;
}
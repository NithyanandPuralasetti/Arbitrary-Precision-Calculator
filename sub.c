#include "calculate.h"

/* Subtracts two big integer magnitudes; returns SIGN if operand1 < operand2 */
Status sub(Dlist *head1,Dlist *tail1,Dlist *head2,Dlist *tail2,Dlist **head3, Dlist **tail3)
{
    Dlist *temp1 = tail1, *temp2 = tail2;
    int borrow = 0,res = 0, flag = 0;
    /* Compare magnitudes to ensure larger minus smaller subtraction */
    int cmp = compare_numbers(head1, tail1, head2, tail2);
    if (cmp == 0)
    {
        /* Equal magnitudes yield 0 */
        if (insert_at_first(head3, tail3, 0))
            return FAILURE;

        return SUCCESS;
    }
    else if (cmp < 0)
    {
        /* Swap traversal pointers so temp1 holds the larger magnitude */
        flag = 1;

        temp1 = tail2;
        temp2 = tail1;
    }
    /* Perform chunk-by-chunk subtraction with borrowing from tail to head */
    while (temp1 != NULL || temp2 != NULL)
    {
        int a = 0, b = 0;

        if (temp1 != NULL)
            a = temp1->data;

        if (temp2 != NULL)
            b = temp2->data;

        if (a < b + borrow)
        {
            res = a + BASE - b - borrow;
            borrow = 1;
        }
        else
        {
            res = a - b - borrow;
            borrow = 0;
        }
        if (insert_at_first(head3, tail3, res))
        {
            delete_list(head3, tail3);
            return FAILURE;
        }
        if (temp1 != NULL)
            temp1 = temp1->prev;

        if (temp2 != NULL)
            temp2 = temp2->prev;
    }
    /* Remove any leading zero chunks produced by subtraction */
    while (*head3 != NULL && (*head3)->data == 0 && (*head3)->next != NULL)
    {
        Dlist *temp = *head3;
        *head3 = (*head3)->next;
        (*head3)->prev = NULL;
        free(temp);
    }
    /* Signal to caller if operand1 was smaller than operand2 */
    if (flag)
        return SIGN;

    return SUCCESS;
}
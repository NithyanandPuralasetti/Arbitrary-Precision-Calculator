#include "calculate.h"

/* Adds two non-negative big integers starting from their tails (least significant chunks) */
Status add(Dlist *tail1,Dlist *tail2,Dlist **head3,Dlist **tail3)
{
    Dlist *temp1 = tail1,*temp2 = tail2;
    int carry = 0;
    /* Traverse backward through both lists until all digits and carry are processed */
    while(temp1 != NULL || (temp2 != NULL) || carry != 0)
    {
        int sum = carry;
        if(temp1 != NULL)
        {
            sum += temp1 -> data;
            temp1 = temp1 -> prev;
        }
        if(temp2 != NULL)
        {
            sum += temp2 -> data;
            temp2 = temp2 -> prev;
        }
        /* Prepend current base-10000 digit to result list */
        if(insert_at_first(head3,tail3,(sum % BASE)))
        {
            delete_list(head3, tail3);
            return FAILURE;
        } 
        carry = sum / BASE;
    }
    return SUCCESS;
}
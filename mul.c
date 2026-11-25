#include "calculate.h"

/* Multiplies two big integers using grade-school chunk-by-chunk multiplication */
Status mul(Dlist *tail1,Dlist *tail2,Dlist **resulthead ,Dlist ** resulttail)
{
    int count = 0;
    long long int carry,mul;
    Dlist *temp1 = NULL,*temp2 = tail2;
    Dlist *head = NULL,*tail = NULL;
    Dlist *newhead = NULL, *newtail = NULL;
    /* Outer loop: iterate through each chunk of the multiplier from right to left */
    while(temp2 != NULL)
    {
        carry = 0;
        temp1 = tail1;
        /* Inner loop: multiply multiplicand by current multiplier chunk */
        while(temp1 != NULL || carry != 0)
        {
            mul = 0;
            if(temp1 != NULL)
            {
                mul = (long long int) (temp1 -> data) * (temp2 -> data);
            }
            mul += carry;
            carry = mul / BASE;
            /* First partial product goes directly into result list; subsequent ones into temp list */
            if(count == 0)
            {
                if(insert_at_first(resulthead,resulttail,mul % BASE) == FAILURE)
                {
                    delete_list(resulthead,resulttail);
                     return FAILURE;
                }  
            }
            else
            {
                if(insert_at_first(&head,&tail,mul % BASE) == FAILURE)
                {
                    delete_list(resulthead, resulttail);
                    delete_list(&head, &tail);
                    delete_list(resulthead,resulttail);
                    return FAILURE;
                }
            }
            if(temp1 != NULL)
            temp1 = temp1 -> prev;
        }
        /* Shift partial product left by appending 'count' zero chunks, then add to running total */
        if(count != 0)
        {
            int i = 0;
            while(i < count)
            {
                if(insert_at_last(&head,&tail,0) == FAILURE)
                {
                    delete_list(resulthead,resulttail);
                    delete_list(&head,&tail);
                    return FAILURE;
                } 
                i ++;
            }
            if(add(*resulttail,tail,&newhead,&newtail) == FAILURE)
            {
                delete_list(&newhead,&newtail);
                delete_list(resulthead,resulttail);
                delete_list(&head,&tail);
                return FAILURE;
            } 
            delete_list(resulthead,resulttail);
            *resulthead = newhead;
            *resulttail = newtail;
            newhead = NULL;
            newtail = NULL;
            delete_list(&head,&tail);
        }
        count ++;
        temp2 = temp2 -> prev;
    }
    /* Strip leading zero chunks (e.g., when multiplying by 0) */
    while (*resulthead != NULL && (*resulthead)->data == 0 && (*resulthead)->next != NULL)
    {
        Dlist *temp = *resulthead;
        *resulthead = (*resulthead)->next;
        (*resulthead)->prev = NULL;
        free(temp);
    }
    return SUCCESS;
}
#include "calculate.h"

/* Computes base^exponent by repeatedly multiplying base and decrementing exponent */
Status power(Dlist *head1,Dlist *tail1,Dlist *head2,Dlist *tail2,Dlist **resulthead,Dlist **resulttail)
{
    *resulthead = NULL;
    *resulttail = NULL;
    Dlist *temphead = NULL;
    Dlist *temptail = NULL;
    Dlist *head = NULL;
    Dlist *tail = NULL;
    Dlist *powhead = NULL;
    Dlist *powtail = NULL;
    Dlist *onehead = NULL;
    Dlist *onetail = NULL;
    Dlist *subhead = NULL;
    Dlist *subtail = NULL;
    Dlist *mulhead = NULL;
    Dlist *multail = NULL;
    /* Initialize constants: 0 (head), 1 (onehead), and initial power accumulator = 1 (powhead) */
    if(insert_at_first(&head,&tail,0) == FAILURE) return FAILURE;
    if(insert_at_first(&onehead,&onetail,1) == FAILURE) goto FAILURE_CHECK;;
    if(insert_at_first(&powhead,&powtail,1) == FAILURE) goto FAILURE_CHECK;;
    /* Copy exponent into temphead so the original exponent list is preserved */
    if(copy_list(head2,&temphead,&temptail) == FAILURE)
    {
        goto FAILURE_CHECK;
    }
    /* Multiply accumulator by base until exponent counter reaches 0 */
    while(compare_numbers(temphead,temptail,head,tail) != 0)
    {
        if(mul(tail1,powtail,&mulhead,&multail) == FAILURE) 
        {
            goto FAILURE_CHECK;
        }
        if(sub(temphead,temptail,onehead,onetail,&subhead,&subtail) == FAILURE) 
        {
            goto FAILURE_CHECK; 
        }
        delete_list(&temphead,&temptail);
        temphead = subhead;
        temptail = subtail;
        subhead = NULL;
        subtail = NULL;
        delete_list(&powhead,&powtail);
        powhead = mulhead;
        powtail = multail;
        mulhead = NULL;
        multail = NULL;
    }
    /* Transfer ownership of final result and clean up helper lists */
    *resulthead = powhead;
    *resulttail = powtail;
    powhead = NULL;
    powtail = NULL;
    delete_list(&temphead,&temptail);
    delete_list(&head,&tail);
    delete_list(&onehead,&onetail);
    return SUCCESS;
    FAILURE_CHECK :
    /* Clean up all allocated lists on failure */
    delete_list(&temphead,&temptail);
    delete_list(&head,&tail);
    delete_list(&powhead,&powtail);
    delete_list(&mulhead,&multail);
    delete_list(&subhead,&subtail);
    delete_list(&onehead,&onetail);
    *resulthead = NULL;
    *resulttail = NULL;
    return FAILURE;
}
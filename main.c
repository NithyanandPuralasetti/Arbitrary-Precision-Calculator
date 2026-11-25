#include "calculate.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./a.out operand1 operator operand2\n");
        return FAILURE;
    }
    int flag = validate(argv);
    if (flag == MISMATCH1)
    {
        printf("Enter proper operand1\n");
        return FAILURE;
    }
    else if (flag == MISMATCH2)
    {
        printf("Enter proper operand2\n");
        return FAILURE;
    }
    else if (flag == MISOP)
    {
        printf("Enter proper operator\n");
        return FAILURE;
    }
    Dlist *head1 = NULL, *tail1 = NULL;
    Dlist *head2 = NULL, *tail2 = NULL;
    Dlist *head3 = NULL, *tail3 = NULL;
    Dlist *quohead = NULL, *quotail = NULL;
    Dlist *remhead = NULL, *remtail = NULL;
    /* Operand strings */
    char *num1 = argv[1];
    char *num2 = argv[3];
    /* Signs: +1 means positive, -1 means negative */
    int sign1 = 1;
    int sign2 = 1;
    int result_sign = 1;
    char ch = argv[2][0];
    Status status = SUCCESS;
    int sign = SUCCESS;
    /* Extract operand 1 sign */
    if (num1[0] == '-')
        sign1 = -1;
    /* Extract operand 2 sign */
    if (num2[0] == '-')
        sign2 = -1;
    /* Convert operands into linked lists */
    if (convert_to_list(&head1, &tail1, num1) == FAILURE)
        goto FAILURE;
    if (convert_to_list(&head2, &tail2, num2) == FAILURE)
        goto FAILURE;
    /* Normalize negative zero operands */
    if (is_zero(head1))
        sign1 = 1;
    if (is_zero(head2))
        sign2 = 1;
    printf("\n========================================\n");
    printf("       ARBITRARY PRECISION CALCULATOR\n");
    printf("========================================\n");
    printf("Operand 1 : ");
    if (sign1 == -1)
        printf("-");
    print_list(head1, tail1);
    printf("\nOperator  : %c\n", ch);
    printf("Operand 2 : ");
    if (sign2 == -1)
        printf("-");
    print_list(head2, tail2);
    printf("\n----------------------------------------\n");
    switch (ch)
    {
        case '+':
        {
            if (sign1 == sign2)
            {
                /* Same signs: add magnitudes */
                status = add(tail1, tail2, &head3, &tail3);
                result_sign = sign1;
            }
            else
            {
                /* Different signs: subtract magnitudes */
                sign = sub(head1, tail1, head2, tail2,&head3, &tail3);
                if (sign == FAILURE)
                    goto FAILURE;
                /*
                    -> SIGN means first magnitude is smaller.
                    -> Therefore result sign is sign2.
                 */
                if (sign == SIGN)
                    result_sign = sign2;
                else
                    result_sign = sign1;
            }
            if (status == FAILURE)
                goto FAILURE;
            /* Avoid printing negative zero */
            if (is_zero(head3))
                result_sign = 1;
            printf("Addition Result : ");
            if (result_sign == -1)
                printf("-");
            print_list(head3, tail3);
            break;
        }
        case '-':
        {
            if (sign1 != sign2)
            {
                /* Different signs: add magnitudes */
                status = add(tail1, tail2, &head3, &tail3);
                result_sign = sign1;
            }
            else
            {
                /* Same signs: subtract magnitudes */
                sign = sub(head1, tail1, head2, tail2,&head3, &tail3);
                if (sign == FAILURE)
                    goto FAILURE;
                /*
                    -> SIGN means first magnitude is smaller.
                    -> Result sign becomes opposite to sign1.
                 */
                if (sign == SIGN)
                    result_sign = -sign1;
                else
                    result_sign = sign1;
            }
            if (status == FAILURE)
                goto FAILURE;
            if (is_zero(head3))
                result_sign = 1;
            printf("Subtraction Result : ");
            if (result_sign == -1)
                printf("-");
            print_list(head3, tail3);
            break;
        }
        case '*':
        {
            status = mul(tail1, tail2, &head3, &tail3);
            if (status == FAILURE)
                goto FAILURE;
            result_sign = sign1 * sign2;
            if (is_zero(head3))
                result_sign = 1;
            printf("Multiplication Result : ");
            if (result_sign == -1)
                printf("-");
            print_list(head3, tail3);
            break;
        }
        case '/':
        case '%':
        {
            status = division(head1, tail1, head2, tail2,&quohead, &quotail,&remhead, &remtail);
            if (status == FAILURE)
            {
                if (is_zero(head2))
                    printf("Error: Division by zero\n");
                goto FAILURE;
            }
            if (ch == '/')
            {
                result_sign = sign1 * sign2;
                if (is_zero(quohead))
                    result_sign = 1;
                printf("Quotient : ");
                if (result_sign == -1)
                    printf("-");
                print_list(quohead, quotail);
            }
            else
            {
                /* Remainder sign follows dividend */
                result_sign = sign1;
                if (is_zero(remhead))
                    result_sign = 1;
                printf("Remainder : ");
                if (result_sign == -1)
                    printf("-");
                print_list(remhead, remtail);
            }
            break;
        }
        case '^':
        {
            if (sign2 == -1)
            {
                printf("Negative exponent is not supported\n");
                goto FAILURE;
            }
            status = power(head1, tail1, head2, tail2,
                           &head3, &tail3);
            if (status == FAILURE)
                goto FAILURE;
            result_sign = 1;
            /*
                -> Negative base with odd exponent
                -> produces a negative result.
             */
            if (sign1 == -1 && tail2->data % 2 != 0)
                result_sign = -1;
            if (is_zero(head3))
                result_sign = 1;
            printf("Power Result : ");
            if (result_sign == -1)
                printf("-");

            print_list(head3, tail3);
            break;
        }
        default:
            printf("Enter proper operator\n");
            goto FAILURE;
    }
    printf("\n========================================\n");
    printf("           OPERATION COMPLETED\n");
    printf("========================================\n");
    delete_list(&head1, &tail1);
    delete_list(&head2, &tail2);
    delete_list(&head3, &tail3);
    delete_list(&quohead, &quotail);
    delete_list(&remhead, &remtail);
    return SUCCESS;
FAILURE:
    printf("\nOperation failed!\n");
    delete_list(&head1, &tail1);
    delete_list(&head2, &tail2);
    delete_list(&head3, &tail3);
    delete_list(&quohead, &quotail);
    delete_list(&remhead, &remtail);
    return FAILURE;
}

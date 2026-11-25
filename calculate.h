#ifndef CALCULATE_H
#define CALCULATE_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define BASE 10000      /* Base 10000: each doubly linked list node stores up to 4 decimal digits */

/* Status codes returned across validation, list operations, and arithmetic */
typedef enum 
{
    SUCCESS,
    MISMATCH1,
    MISMATCH2,
    MISOP,
    FAILURE,
    LIST_EMPTY,
    SIGN
} Status;

/* Doubly linked list node representing one base-10000 chunk of a big integer */
typedef struct Dlist
{
    int data;
    struct Dlist *next;
    struct Dlist *prev;
}Dlist;

/* Input validation and linked list helper functions and arithmetic operations functions */
Status validate(char **);
Status convert_to_list(Dlist **,Dlist **,char *);
Status insert_at_last(Dlist **,Dlist **,int);
Status print_list(Dlist *,Dlist *);
Status insert_at_first(Dlist **,Dlist **,int);
Status add(Dlist *tail1,Dlist *tail2,Dlist **head3,Dlist **tail3);
Status sub(Dlist *head1,Dlist *tail1,Dlist *head2,Dlist *tail2,Dlist **head3,Dlist **tail3);
int compare_numbers(Dlist *head1, Dlist *tail1,Dlist *head2, Dlist *tail2);
Status mul(Dlist *tail1,Dlist *tail2,Dlist **resulthead ,Dlist ** resulttail);
void delete_list(Dlist **head,Dlist **tail);
int is_zero(Dlist *head);
void remove_leading_zeros(Dlist **head, Dlist **tail);
Status copy_list(Dlist *head, Dlist **newhead, Dlist **newtail);
Status mul_num(Dlist *divisortail, Dlist **head, Dlist **tail, int q);
Status find_q(Dlist *divisorhead, Dlist *divisortail,Dlist *currenthead, Dlist *currenttail,int *quotient,Dlist **resulthead, Dlist **resulttail);
Status division(Dlist *head1, Dlist *tail1,Dlist *head2, Dlist *tail2,Dlist **quohead, Dlist **quotail,Dlist **remhead, Dlist **remtail);
Status power(Dlist *head1,Dlist *tail1,Dlist *head2,Dlist *tail2,Dlist **resulthead,Dlist **resulttail);

#endif
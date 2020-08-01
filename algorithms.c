#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "algorithms.h"


int binary_search(int searched_val, int *lst, int lst_len)
{
    /* search for some value inside a vector
    
       returns: -1, if the does not find the value
                (index) searched_val, if it`s found
    */

    int start, end, midle;
    start = 0; end = lst_len-1; 

    while (start <= end)
    {
        midle = (start + end)/2;

        if (searched_val == lst[midle])  
            return midle;

        if (searched_val > lst[midle])  
            start = ++midle;

        else
            end = --midle;
    }    

    return -1;
}


int linear_search(int searched_val, int *lst, int lst_len)
{
    /* search for some value inside a vector
    
       returns: -1, if the does not find the value
                (index) searched_val, if it`s found
    */

    for (int i = 0; i < lst_len; i++)
        if (searched_val == lst[i])
            return i;
    return -1;
}


void cmp_linar_binary_search(void)
{
    int vet_len = 2*100000000;
    int *vet = (int *) malloc(vet_len * sizeof(int));

    for(int i = 0; i < vet_len; i++) 
        vet[i] = i;

    
    clock_t t; 
    double time_taken;
    int searched_num = vet[vet_len-1];
    int index;


    printf("numero buscado:%d\n\n", searched_num);



    printf("======= linear search for the number %d in vet ===========\n", searched_num);
    t = clock(); 

    index = linear_search(searched_num, vet, vet_len);

    if (index != -1)
        printf("encontrei o numero\n");

    t = clock() - t; 

    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("took %.15f seconds to execute \n", time_taken); 



    printf("======= binary search for the number %d in vet ===========\n", searched_num);
    t = clock(); 

    index = binary_search(searched_num, vet, vet_len);

    if (index != -1)
        printf("encontrei o numero\n");

    t = clock() - t; 

    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("took %.15f seconds to execute \n\n", time_taken); 
    


    free(vet);
}


void quick_sort(int *a, int left, int right)
{
    int i, j, x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
     
    while(i <= j) {
        while(a[i] < x && i < right) {
            i++;
        }
        while(a[j] > x && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
     
    if(j > left) {
        quick_sort(a, left, j);
    }
    if(i < right) {
        quick_sort(a, i, right);
    }
}


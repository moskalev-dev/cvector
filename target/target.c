#include "target.h"
#include "../cvector.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    cvector* vector;
    int i = 0;

    vector = cvector_new (0, sizeof(int));
    printf ("Create vector: adress = %p \n", vector);

    // push
    for(i=0; i<42; i++)
    {
        int a = i+1;
        vector->push(vector, (cv_byte*)&a);
        //printf ("push: count = %d capacity = %d \n", vector->count(vector), vector->capacity(vector));
    }
    printf ("End push: count = %lu capacity = %lu \n\n",vector->count(vector), vector->capacity(vector));

    // pop
    for(i=0; i<2; i++)
    {
        vector->pop(vector);
        printf ("Pop: count = %lu capacity = %lu \n",vector->count(vector), vector->capacity(vector));
    }

    // at
    for(i=0; i<42; i++)
    {
        int *at = (int*)vector->at(vector,i);
        if(at)
        {
            printf ("At: vector[%d] = %d | count = %lu capacity = %lu \n",
                   i,*at, vector->count(vector), vector->capacity(vector));
        }
    }

    printf ("Empty: count = %lu capacity = %lu empty=%d\n",
           vector->count(vector), vector->capacity(vector),vector->empty(vector));

    // replace
    int b = 77;
    vector->replace(vector, 10, (cv_byte*)&b);
    int *at2 = (int*)vector->at(vector,10);
    if(at2)
    {
        printf ("Replace: vector[10] = %d | count = %lu capacity = %lu \n",
               *at2, vector->count(vector), vector->capacity(vector));
    }

    // remove
    vector->remove(vector, 10);
    int *at3 = (int*)vector->at(vector,10);
    if(at3)
    {
        printf ("Remove: vector[10] = %d | count = %lu capacity = %lu \n",
               *at3, vector->count(vector), vector->capacity(vector));
    }

    // pop
    for(i=0; i<30; i++)
    {
        vector->pop(vector);
        printf ("Pop: count = %lu capacity = %lu \n",vector->count(vector), vector->capacity(vector));
    }

    // clear
    vector->clear(vector);
    printf ("Clear: count = %lu capacity = %lu \n",
           vector->count(vector), vector->capacity(vector));

    int *at4 = (int*)vector->at(vector,10);
    if(at4)
    {
        printf ("At after clear: vector[10] = %d | count = %lu capacity = %lu \n",
               *at4, vector->count(vector), vector->capacity(vector));
    }

    vector->free(vector);
    vector = NULL;
    printf ("Free vector = %p \n", vector);

    return 0;
}

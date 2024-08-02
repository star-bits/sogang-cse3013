#include <stdio.h>
#include "header.h"

void print_digits(int* digits) {
    for (int i=0; i<10; i++) { // for each element in the array 'digits'
        printf("%d ", digits[i]); // print with trailing space
    }
    printf("\n"); // newline
}

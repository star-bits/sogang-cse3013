#include "header.h"

void count_digits(int n, int* digits) { // n is the number of pages
    for (int i=1; i<=n; i++) { // for each pages up until n
        int temp = i; 
        while (temp>0) {
            int digit = temp%10; // get the last digit
            digits[digit]++; // ++
            temp /= 10; // remove the last digit
        }
    }
}

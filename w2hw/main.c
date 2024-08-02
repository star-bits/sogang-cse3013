#include <stdio.h>
#include "header.h"

int main() {
    int t;
    scanf("%d", &t); // take input t

    while (t--) { // for t times
        int n;
        scanf("%d", &n); // take input n

        int digits[10] = {0}; // array for 0 to 9
        count_digits(n, digits); // counting part
        print_digits(digits); // printing part
    }
    return 0;
}

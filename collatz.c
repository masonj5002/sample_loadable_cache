#include "collatz.h"

#define IS_EVEN(X) (!((X)&1))

int steps_for_number(int number) {
    if (number == 1)
        return 0;
    else
        return IS_EVEN(number)? number/2 : 3*number+1;
}



#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "collatz.h"

#include "cache.h"

// include any or all of your policies
#include "bozo_cache.h"

int main(int argc, char *argv[]) {
    // uncomment one of me
    //char* policy = "bozo";
    char* policy = "nuttin honey";

    // base function
    ProviderFunction test_me = steps_for_number;

    bool cache_installed = false;
    CacheProvider ppp;

    if (!strcmp(policy,"bozo")) {
        cache_installed = true;
        ppp = BozoProvider();
        test_me = set_provider(ppp, test_me);
    }
    // etc

    // do yo tests using test_me like:
    int randomnumber = 100;
    int steps = test_me(randomnumber);
    printf("%d took %d steps\n",randomnumber,steps);


    if (cache_installed) {
        CacheStats stats = (ppp.stats)();
        printf("%d, %d\n", stats.cache_requests, stats.cache_hits);

        (ppp.cleanup)();
    }
}

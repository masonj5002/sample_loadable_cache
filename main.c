#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "collatz.h"

/*
** This is a cache tester for NON-RECURSIVE functions.
** It does not support the provider function accessing the cache.
*/

#define TEST_COUNT 1000
#define MAX_TEST_NUMBER 500

int rand_between(int min, int max);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [cache.so]\n", argv[0]);
        return 1;
    }

    // base (real) function
    ProviderFunction get_me_a_value = steps_for_number;

    bool cache_installed            = argc > 1;
    Cache *cache = NULL;

    if (cache_installed) {
        cache = load_cache_module(argv[1]);

        if (cache == NULL) {
            fprintf(stderr, "Failed to load cache module\n");
            return 1;
        }
        // replace our real provider with a caching provider
        get_me_a_value = cache->set_provider_func(get_me_a_value);
    }

    // do yo tests using test_me like:
    for (int test_number = 0; test_number < TEST_COUNT; test_number++) {
        int randomnumber = rand_between(1, MAX_TEST_NUMBER);

        printf("\n\nBeginning test %2d: %d\n", test_number, randomnumber);

        int steps = get_me_a_value(randomnumber);

        printf("Done with test %2d: %d took %d steps\n", test_number,
               randomnumber, steps);

        if (test_number == TEST_COUNT / 2) {
            printf("Taking a break. Resetting cache statistics.\n");
            cache->reset_statistics();
        }
    }

    if (cache_installed) {
        printf("\n\n");

        CacheStat *list_of_stats = cache->get_statistics();
        print_cache_stats(fileno(stdout), list_of_stats);

        if (list_of_stats)
            free(list_of_stats);

        printf("\n\n");

        cache->cache_cleanup();
        free(cache);
    }
}

int rand_between(int min, int max) {
    int range = max - min;
    return min + arc4random_uniform(range);
}

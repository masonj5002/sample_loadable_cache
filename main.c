#include <dlfcn.h>
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

// load the shared lib, get the provider_loader
ProviderLoader load_cache_module(const char *libname);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [cache.so]\n", argv[0]);
        return 1;
    }

    // base function
    ProviderFunction test_me = steps_for_number;

    bool cache_installed     = argc > 1;

    CacheProvider ppp;

    if (cache_installed) {
        ProviderLoader cache_loader = load_cache_module(argv[1]);
        if (!cache_loader) {
            fprintf(stderr, "Failed to load cache module\n");
            return 1;
        }
        ppp                         = (*cache_loader)();
        test_me                     = set_provider(ppp, test_me);
    }

    // do yo tests using test_me like:
    for (int test_number = 0; test_number < TEST_COUNT; test_number++) {
        int randomnumber = rand_between(1, MAX_TEST_NUMBER);
        printf("\n\nBeginning test %2d: %d\n", test_number, randomnumber);
        int steps        = test_me(randomnumber);
        printf("Done with test %2d: %d took %d steps\n", test_number, randomnumber,
               steps);
    }

    if (cache_installed) {
        printf("\n\nCache Stats:\n");
        CacheStats stats = (ppp.stats)();
        printf("Cache Requests:  %4d\n", stats.cache_requests);
        printf("Cache hits:      %4d\n", stats.cache_hits);
        printf("Cache misses:    %4d\n", stats.cache_misses);
        printf("Cache evictions: %4d\n", stats.cache_evictions);
        printf("\n");
        (ppp.cleanup)();
    }
}

ProviderLoader load_cache_module(const char *libname) {
    void *handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return NULL;
    }

    ProviderLoader provider_functions_ptr =
        (ProviderLoader)dlsym(handle, "provider_functions");
    if (!provider_functions_ptr) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return NULL;
    }

    return provider_functions_ptr;
}

int rand_between(int min, int max) {
    int range = max - min;
    return min + arc4random_uniform(range);
}

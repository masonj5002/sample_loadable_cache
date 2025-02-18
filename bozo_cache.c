#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

#define UNUSED(x) (void)(x)

/* THIS IS A BOGUS CACHE PROVIDER */
/* It implements the required functions but NONE of them are real. */

ValueType _caching_provider(KeyType key) {
    fprintf(stderr, __FILE__ " caching_provider(" KEY_FMT ")\n", key);
    return arc4random_uniform(key*5);
}

ProviderFunction set_provider(ProviderFunction downstream) {
    fprintf(stderr, __FILE__ " set_provider()\n");
    UNUSED(downstream);
    return _caching_provider;
}

void initialize(void) {
    // set up your vars
    fprintf(stderr, __FILE__ " initialize()\n");
}
void cleanup(void) {
    // free yo junk
    fprintf(stderr, __FILE__ " cleanup()\n");
}

void reset_statistics(void) {
    fprintf(stderr, __FILE__ " reset_statistics()\n");
}

CacheStat* statistics(void) {
    // maybe better numbers would be nice
    fprintf(stderr, __FILE__ " statistics()\n");

    CacheStat *statsdata = malloc(5 * sizeof(CacheStat));
    statsdata[0] = (CacheStat){Cache_requests, 10};
    statsdata[1] = (CacheStat){Cache_hits, 15};
    statsdata[2] = (CacheStat){Cache_misses, 500};
    statsdata[3] = (CacheStat){Cache_evictions, 34};
    statsdata[4] = (CacheStat){END_OF_STATS, 0};
    return statsdata;
}

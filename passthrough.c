#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

// The simplest example. Not a cache.

int cache_requests;

ProviderFunction _downstream = NULL;


void reset_statistics(void) {
    fprintf(stderr, __FILE__ " reset_statistics()\n");
    cache_requests = 0;
}


CacheStat* statistics(void) {
    fprintf(stderr, __FILE__ " statistics()\n");

    CacheStat* stats_cache = malloc(2 * sizeof(CacheStat));
    stats_cache[0]         = (CacheStat){Cache_requests, cache_requests};
    stats_cache[1]         = (CacheStat){END_OF_STATS, 0};

    return stats_cache;
}

ValueType _caching_provider(KeyType key) {
    cache_requests++;

    return (*_downstream)(key);
}

ProviderFunction set_provider(ProviderFunction downstream) {
    fprintf(stderr, __FILE__ " set_provider()\n");
    _downstream = downstream;
    return _caching_provider;
}

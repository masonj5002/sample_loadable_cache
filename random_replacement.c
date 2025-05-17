// KNOWN ISSUE: random_replacement.c WRITES TO A RANDOM LOCATION IN THE ARRAY
// *EVERY TIME* (NOT JUST WHEN THE CACHE IS FULL).
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cache.h"

typedef struct {
    KeyType key;
    ValueType value;
    bool valid;
} CacheEntry;

#define CACHE_SIZE 1000
CacheEntry cache[CACHE_SIZE];
int current_cache_size;

#define VALUE_NOT_PRESENT 0

int cache_requests;
int cache_hits;
int cache_misses;

ProviderFunction _downstream = NULL;

void initialize(void) {
    fprintf(stderr, __FILE__ " initialize()\n");
    srand(time(NULL));

    current_cache_size = 0;
    cache_requests     = 0;
    cache_hits         = 0;
    cache_misses       = 0;

    for (int ix = 0; ix < CACHE_SIZE; ix++)
        cache[ix].valid = VALUE_NOT_PRESENT;
}

void cleanup(void) {
    fprintf(stderr, __FILE__ " cleanup()\n");
}

CacheStat* statistics(void) {
    fprintf(stderr, __FILE__ " statistics()\n");

    CacheStat* current_stats = malloc(5 * sizeof(CacheStat));
    current_stats[0]         = (CacheStat){Cache_requests, cache_requests};
    current_stats[1]         = (CacheStat){Cache_hits, cache_hits};
    current_stats[2]         = (CacheStat){Cache_misses, cache_misses};
    current_stats[3]         = (CacheStat){Cache_size, current_cache_size};
    current_stats[4]         = (CacheStat){END_OF_STATS, 0};

    return current_stats;
}

void reset_statistics(void) {
    fprintf(stderr, __FILE__ " reset_statistics()\n");
    cache_requests = 0;
    cache_hits     = 0;
    cache_misses   = 0;
}

// bool _is_present(KeyType key) {
//     bool present = false;
//     for (int ix = 0; ix < CACHE_SIZE; ix++)
//         if (cache[ix].valid && cache[ix].key == key)
//             present = true;
//     fprintf(stderr, __FILE__ " is_present(" KEY_FMT ") = %s \n", key,
//             present ? "true" : "false");
//     return present;
// }

ValueType _get(KeyType key, bool* found) {
    for (int ix = 0; ix < CACHE_SIZE; ix++) {
        if (cache[ix].valid && cache[ix].key == key) {
            fprintf(stderr, __FILE__ " is_present(" KEY_FMT ") = true\n", key);
            *found = true;
            fprintf(stderr, __FILE__ " get(" KEY_FMT ") -> " VALUE_FMT "\n",
                    key, cache[ix].value);
            return cache[ix].value;
        }
    }
    fprintf(stderr, __FILE__ " is_present(" KEY_FMT ") = false\n", key);
    *found = false;
    return 0;
}

void _insert(KeyType key, ValueType value) {
    fprintf(stderr, __FILE__ " insert(" KEY_FMT ") = " VALUE_FMT "\n", key,
            value);

    int index;
    if (current_cache_size < CACHE_SIZE) {
        // use next available empty slot
        for (index = 0; index < CACHE_SIZE; index++)
            if (!cache[index].valid)
                break;
        current_cache_size++;
    } else {
        // Cache is full, randomly evict
        index = rand() % CACHE_SIZE;
    }

    cache[index].key   = key;
    cache[index].value = value;
    cache[index].valid = true;
}

ValueType _caching_provider(KeyType key) {
    cache_requests++;

    bool found;
    ValueType result = _get(key, &found);
    if (found) {
        cache_hits++;
        return result;
    }

    cache_misses++;
    result = (*_downstream)(key);
    _insert(key, result);
    return result;
}

ProviderFunction set_provider(ProviderFunction downstream) {
    fprintf(stderr, __FILE__ " set_provider()\n");
    _downstream = downstream;
    return _caching_provider;
}

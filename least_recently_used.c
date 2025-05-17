// Using random_replacement.c as a template

// KNOWN ISSUE: random_replacement.c WRITES TO A RANDOM LOCATION IN THE ARRAY
// *EVERY TIME* (NOT JUST WHEN THE CACHE IS FULL).
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cache.h"

#define CACHE_SIZE 1000
#define VALUE_NOT_PRESENT 0

typedef struct {
    KeyType key;
    ValueType value;
    bool valid;
    struct CacheEntry *prev;
    struct CacheEntry *next;
} CacheEntry;

typedef struct {
    int capacity;
    int size;
    CacheEntry *head;
    CacheEntry *tail;
} CacheAttributes;

CacheEntry cache[CACHE_SIZE];

int cache_requests;
int cache_hits;
int cache_misses;

ProviderFunction _downstream = NULL;

void initialize(void) {
    fprintf(stderr, __FILE__ " initialize()\n");
    srand(time(NULL));
    cache_requests = 0;
    cache_hits     = 0;
    cache_misses   = 0;

    for (int ix = 0; ix < CACHE_SIZE; ix++)
        cache[ix].valid = VALUE_NOT_PRESENT;
}

void cleanup(void) {
    fprintf(stderr, __FILE__ " cleanup()\n");
}

CacheStat* statistics(void) {
    fprintf(stderr, __FILE__ " statistics()\n");

    CacheStat* current_stats = malloc(4 * sizeof(CacheStat));
    current_stats[0]         = (CacheStat){Cache_requests, cache_requests};
    current_stats[1]         = (CacheStat){Cache_hits, cache_hits};
    current_stats[2]         = (CacheStat){Cache_misses, cache_misses};
    current_stats[3]         = (CacheStat){Cache_size, CACHE_SIZE};
    current_stats[4]         = (CacheStat){END_OF_STATS, 0};

    return current_stats;
}

void reset_statistics(void) {
    fprintf(stderr, __FILE__ " reset_statistics()\n");
    cache_requests = 0;
    cache_hits     = 0;
    cache_misses   = 0;
}

bool _is_present(KeyType key) {
    bool present = false;
    for (int ix = 0; ix < CACHE_SIZE; ix++)
        if (cache[ix].valid && cache[ix].key == key)
            present = true;
    fprintf(stderr, __FILE__ " is_present(" KEY_FMT ") = %s \n", key,
            present ? "true" : "false");
    return present;
}

void _insert(KeyType key, ValueType value) {
    fprintf(stderr, __FILE__ " insert(" KEY_FMT ") = " VALUE_FMT "\n", key,
            value);
    int ix          = rand() % CACHE_SIZE;
    cache[ix].key   = key;
    cache[ix].value = value;
    cache[ix].valid = true;
}

ValueType _get(KeyType key) {
    for (int ix = 0; ix < CACHE_SIZE; ix++)
        if (cache[ix].valid && cache[ix].key == key) {
            fprintf(stderr, __FILE__ " get(" KEY_FMT ") -> " VALUE_FMT "\n",
                    key, cache[ix].value);
            return cache[ix].value;
        }
    return VALUE_NOT_PRESENT;
}

ValueType _caching_provider(KeyType key) {
    cache_requests++;

    if (_is_present(key)) {
        cache_hits++;
        return _get(key);
    }

    cache_misses++;
    ValueType result = (*_downstream)(key);
    _insert(key, result);

    return result;
}

ProviderFunction set_provider(ProviderFunction downstream) {
    fprintf(stderr, __FILE__ " set_provider()\n");
    _downstream = downstream;
    return _caching_provider;
}

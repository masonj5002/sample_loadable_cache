#include <stdio.h>

#include "cache.h"

/* THIS IS A BOGUS CACHE PROVIDER */
/* It implements the required functions but NONE of them are real. */

ValueType my_cache[10000];

void init(void) {
    // set up your vars
    fprintf(stderr, __FILE__ " init()\n");
}
void cleanup(void) {
    // free yo junk
    fprintf(stderr, __FILE__ " cleanup()\n");
}
bool is_present(KeyType key) {
    // in da cache?
    fprintf(stderr, __FILE__ " is_present(" KEY_FMT ") = %s\n", key,
        (key > 10000) ? "true" : "false");
    return key > 10000;

    return true;
}
void insert(KeyType key, ValueType value) {
    fprintf(stderr, __FILE__ " insert(" KEY_FMT ") = " VALUE_FMT "\n",
        key, value);
    // sure, trust me
}

ValueType get(KeyType key) {
    // true for something!
    ValueType result = key*3;
    fprintf(stderr, __FILE__ " get(" KEY_FMT ") -> " VALUE_FMT "\n",
        key, result);
    return result;
}

CacheStats stats(void) {
    // maybe better numbers would be nice
    fprintf(stderr, __FILE__ " stats()\n");
    return (CacheStats){
        .cache_requests = 10,
        .cache_hits = 15,
        .cache_misses = 199,
        .cache_evictions = 2727
    };
}


CacheProvider provider_functions(void) {
    fprintf(stderr, __FILE__ " functions retrieved!\n");
    return (CacheProvider){
        .initialize = init,
        .cleanup = cleanup,
        .is_present = is_present,
        .get = get,
        .insert = insert,
        .stats = stats
    };
}

#include <stdbool.h>
#include <stdio.h>

#include "cache.h"

/* Simple memoizer */

#define MAX_KEY 10000
#define CACHE_SIZE MAX_KEY+1
ValueType cache[CACHE_SIZE];

#define VALUE_NOT_PRESENT 0

CacheStats stats_cache = {0, 0, 0, 0};

void init(void) {
    fprintf(stderr, __FILE__ " init()\n");

    for (int ix = 0; ix < CACHE_SIZE; ix++) {
        cache[ix] = VALUE_NOT_PRESENT;
    }
}
void cleanup(void) {
    fprintf(stderr, __FILE__ " cleanup()\n");
}
bool is_present(KeyType key) {
    bool present = key >= 0
        && key <= MAX_KEY
        && cache[key] != VALUE_NOT_PRESENT;

    stats_cache.cache_requests++;
    if (present)
        stats_cache.cache_hits++;
    else
        stats_cache.cache_misses++;

    fprintf(stderr, __FILE__ " is_present(" KEY_FMT ") = %s\n", key,
        present ? "true" : "false");
    return present;
}
void insert(KeyType key, ValueType value) {
    if (key < 0 || key > MAX_KEY)
        return;

    fprintf(stderr, __FILE__ " insert(" KEY_FMT ") = " VALUE_FMT "\n",
        key, value);
    cache[key] = value;
}

ValueType get(KeyType key) {
    if (key < 0 || key > MAX_KEY)
        return VALUE_NOT_PRESENT;

    ValueType result = cache[key];
    fprintf(stderr, __FILE__ " get(" KEY_FMT ") -> " VALUE_FMT "\n",
        key, result);
    return result;
}

CacheStats stats(void) {
    fprintf(stderr, __FILE__ " stats()\n");
    return stats_cache;
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

#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

// Cache key and value types.
// Changeable, but examine how they're used
// before assuming it's all good.
// FMTs are for printf (%p for pointer, %u for unsigned int, etc.)
typedef int KeyType;
#define KEY_FMT "%d"

typedef int ValueType;
#define VALUE_FMT "%d"

/*
* Basic Cache Stats, probably appropriate for every
* policy. If not, do something different.
*/
typedef struct stats {
    int cache_requests;
    int cache_hits;
    int cache_misses;
    int cache_evictions;
} CacheStats;

typedef ValueType (*ProviderFunction)(KeyType key);

typedef void (*InitializeFunc)(void);
typedef bool (*BoolFunc)(KeyType key);
typedef void (*InsertFunc)(KeyType key, ValueType value);
typedef CacheStats (*CacheStatsFunc)(void);
typedef void (*CleanupFunc)(void);

typedef struct provider {
    InitializeFunc initialize;
    CleanupFunc cleanup;
    BoolFunc is_present;
    ProviderFunction get;
    InsertFunc insert;
    CacheStatsFunc stats;
} CacheProvider;

// a library must provide a function called "provider_functions" that has
// THIS signature
typedef CacheProvider (*ProviderLoader)(void);


// given a downstream provider, this returns a caching provider function
ProviderFunction set_provider(CacheProvider prov,
                    ProviderFunction downstream);

CacheStats stats(void);

void cleanup(void);

#endif

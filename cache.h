#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

typedef struct stats {
    int cache_requests;
    int cache_hits;
} CacheStats;

typedef int (*ProviderFunction)(int val);

typedef void (*InitializeFunc)();
typedef bool (*BoolFunc)(int val);
typedef void (*VoidFunc)(int val);
typedef CacheStats (*CacheStatsFunc)(void);
typedef void (*CleanupFunc)(void);

typedef struct provider {
    InitializeFunc initialize;
    CleanupFunc cleanup;
    BoolFunc is_present;
    ProviderFunction get;
    VoidFunc insert;
    CacheStatsFunc stats;
} CacheProvider;

// given a downstream provider, this returns a caching provider function
ProviderFunction set_provider(CacheProvider prov,
                    ProviderFunction downstream);

CacheStats stats();

void cleanup();

#endif

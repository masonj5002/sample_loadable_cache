#include "cache.h"

#include <stdlib.h>

// private vars
CacheProvider _provider;
ProviderFunction _downstream;

// our one function that honors the cache provider
int cached_steps_provider(int val) {
    BoolFunc is_pres = _provider.is_present;
    ProviderFunction get = _provider.get;
    VoidFunc insert = _provider.insert;

    if ( (*is_pres)(val)) {
        return (*get)(val);
    }
    int result = (*_downstream)(val);
    (*insert)(val);
    return result;
}

// given a downstream provider, this returns a cached version
ProviderFunction set_provider(CacheProvider cp, ProviderFunction dsp) {
    _provider = cp;
    _downstream = dsp;

    InitializeFunc its_init = _provider.initialize;
    (*its_init)();

    // our function, abstracts all provider funcs
    return cached_steps_provider;
}


CacheStats stats() {
    return (*(_provider.stats))();
}

void cleanup() {
    (*(_provider.cleanup))();
}

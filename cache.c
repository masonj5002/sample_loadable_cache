#include "cache.h"

#include <stdlib.h>

// private vars
CacheProvider _provider;
ProviderFunction _downstream;

/*
** Note that this module cannot abstract the eviction process because it varies in
** what it returns. Sometimes it returns the place to insert the new value, sometimes
** it does not. So eviction is left to the cache provider (in the insert function, with
** tracking in the is_present function).
*/
// our one function that honors the cache provider
// this is provider to the application.
ValueType caching_provider(KeyType key) {
    BoolFunc is_pres     = _provider.is_present;
    ProviderFunction get = _provider.get;
    InsertFunc insert    = _provider.insert;

    if ((*is_pres)(key))
        return (*get)(key);

    ValueType result = (*_downstream)(key);
    (*insert)(key, result);
    return result;
}

// given a downstream provider, this returns a cached version
ProviderFunction set_provider(CacheProvider cp, ProviderFunction dsp) {
    _provider               = cp;
    _downstream             = dsp;

    InitializeFunc its_init = _provider.initialize;
    (*its_init)();

    // our function, abstracts all provider funcs
    return caching_provider;
}

CacheStats stats(void) {
    return (*(_provider.stats))();
}

void cleanup(void) {
    (*(_provider.cleanup))();
}

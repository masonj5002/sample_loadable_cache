#include <stdio.h>

#include "cache.h"

#define UNUSED(x) (void)(x)

/* THIS IS A BOGUS CACHE PROVIDER */
/* It implements the required functions but NONE of them are real. */

ValueType _caching_provider(KeyType key) {
    fprintf(stderr, __FILE__ " caching_provider(" KEY_FMT ")\n", key);
    return key * 3;
}

ProviderFunction set_provider(ProviderFunction downstream) {
    fprintf(stderr, __FILE__ " set_provider()\n");
    UNUSED(downstream);
    return _caching_provider;
}


#include "bozo_cache.h"

int my_bozo_cache[10000];

void bozo_init() {
    // set up your vars
}
void bozo_cleanup() {
    // free yo junk
}
bool bozo_is_present(int val) {
    // in da cache?
    return true;
}
void bozo_insert(int val) {
    // sure, trust me
}

int bozo_get(int val) {
    // true for something!
    return 5; // a guaranteed random number by dice roll (c.f. xkcd)
}

CacheStats bozo_stats() {
    // maybe better numbers would be nice
    return (CacheStats){
        .cache_requests = 10,
        .cache_hits = 15
    };
}


CacheProvider BozoProvider() {
    return (CacheProvider){
        .initialize = bozo_init,
        .cleanup = bozo_cleanup,
        .is_present = bozo_is_present,
        .get = bozo_get,
        .insert = bozo_insert,
        .stats = bozo_stats
    };
}

## Example of **cache policies** and **dynamically loading a library**

`cache.h` is a wrapper around a cache policy. The policy is loaded by `main()` from the `.so` file listed on the command line. If no `.so` is listed, no cache is present.

For example, type `./main librex_cache.so`

## TO DO

Since we're loading the library, the abstraction of the cache is not really helpful. I'm going to move the entire cache concept into the providers that are loaded.


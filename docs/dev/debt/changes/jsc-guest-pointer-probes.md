# libSceJsc

## change [JSString ABI and ownership](../../../core/libs/prx/libSceJsc/Export.cpp)

- [JSString ABI and ownership](../../../core/libs/prx/libSceJsc/Export.cpp): console JavaScriptCore version and object layout remain unverified; parameter pointers across exported string and helper routines are bound safely to local aliases. A null handle given to retain, release or maximum-size query, UTF-8 above `INT32_MAX` bytes and UTF-16 length above 2147483635 throw where WebKit faults. Other JavaScriptCore APIs remain unavailable.

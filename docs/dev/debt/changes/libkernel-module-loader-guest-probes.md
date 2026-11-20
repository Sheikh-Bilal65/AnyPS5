# libkernel

## add Robustness

- [Module loader and reachability guest access](../../../core/libs/prx/libkernel/Module/src/DynamicLoader.cpp) (libkernel) - `sceKernelCheckReachability`, `dlopen`, `dlsym`, `sceKernelGetModuleInfoForUnwind`, and `__tls_get_addr` access guest paths, symbol names, TLS indices, and unwind output buffers through guarded references without parameter modification or unvalidated writes.

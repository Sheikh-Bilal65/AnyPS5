# libkernel

## add Robustness

- [System and signal guest pointer probing](../../../core/libs/prx/libkernel/System/src/SystemConfiguration.cpp) (libkernel) - Signal disposition, alternate stack, sysctl MIB lookups, sysctlbyname, sysctlnametomib, and user context capture access guest memory and register frames through checked copies and guarded local buffers without parameter mutation.

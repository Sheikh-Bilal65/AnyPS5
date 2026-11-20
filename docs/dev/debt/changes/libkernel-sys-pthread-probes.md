# libkernel_sys

## add Functional

- [pthread_attr_setstackaddr and pthread_get_specificarray_np](../../../core/libs/prx/libkernel_sys/Export.Discovery.cpp) (libkernel_sys) - copying `attr` via `GuestCopyIn_nid_no_patch` (returning `EINVAL` on unmapped memory) and writing keys via `GuestCopyOut_nid_no_patch` (returning `EFAULT` while preserving `errno` on unmapped memory), are host-safety fallbacks; console fault behavior on invalid buffers has not been measured.

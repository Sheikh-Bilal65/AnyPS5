# libSceFreeTypeOl

## add Functional

- [sceFreeTypeOlAlloc](../../../core/libs/prx/libSceFreeTypeOl/ModuleWrappers.cpp) (libSceFreeTypeOl) - probing guest memory through `GuestCopyIn_nid_no_patch` and `GuestCopyOut_nid_no_patch` and returning error 6 with caller errno preserved on unmapped or unwritable buffers is a host-safety fallback; console fault behavior has not been measured.

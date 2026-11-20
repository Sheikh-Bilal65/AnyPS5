# libScePsm

## add Functional

- [ScePsmMonoArrayLength, ScePsmMonoGcWbarrierGenericStore, ScePsmMonoStringLength](../../../core/libs/prx/libScePsm/Export.cpp) (libScePsm) - probing guest memory through `GuestCopyIn_nid_no_patch` and `GuestCopyOut_nid_no_patch` and throwing via `NotImplemented_nid_no_patch` on invalid or unmapped pointers is a host-safety fallback; console fault behavior on invalid buffers has not been measured.

# libSceImeBackend

## add Functional

- [sceImeBackendOpen, sceImeBackendGetCandidateList, sceImeBackendGetCandidateSelect, sceImeBackendGetConvertMode](../../../core/libs/prx/libSceImeBackend/Export.cpp) (libSceImeBackend) - safely probing and copying guest pointers via `GuestCopyIn_nid_no_patch`, `GuestCopyOut_nid_no_patch`, and `GuestWritable_nid_no_patch` and returning `kNullArgument` on unmapped or unwritable addresses is a host-safety fallback; console fault behavior on invalid buffers has not been measured.

# libSceJxr

## change [attachISWrite](../../../core/libs/prx/libSceJxr/src/BitIo.cpp#L98)

- [attachISWrite](../../../core/libs/prx/libSceJxr/src/BitIo.cpp#L98) not measured on a PS5: LP64 prototypes/layouts are inferred from export names; invalid pointers and callback executability remain unverified. Functions access parameter pointers through local aliases and safe helpers to avoid direct guest pointer dereferences. Null state/stream/callback throws. Calls GetPos with the original offset field, ignores its result, then initializes the writer with the preceding 8192-byte buffer, preserving shadow and offset.

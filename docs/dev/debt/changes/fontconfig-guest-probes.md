# libfontconfig

## change FcMatrixMultiply and FcCharSetMerge

- [FcMatrixMultiply and FcCharSetMerge](../../../core/libs/prx/libfontconfig/Export.cpp) (libfontconfig) - fontconfig functions access pointer parameters through local aliases and safe helpers to avoid direct guest pointer dereferences; returning or failing safely without modifying memory when given unmapped or unwritable pointer arguments is a host-safety fallback; console hardware behavior on unmapped memory is unmeasured.

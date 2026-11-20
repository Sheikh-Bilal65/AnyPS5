# libSceAmpr

## change AMPR command buffer and marker guest copies

- [AMPR command buffer and marker guest copies](../../../core/libs/prx/libSceAmpr/Export.cpp) (libSceAmpr) - `sceAmprAprCommandBufferConstructor`, `sceAmprAprCommandBufferMapEnd`, `sceAmprAprCommandBufferReadFileGather`, `sceAmprAprCommandBufferResetGatherScatterState`, `sceAmprCommandBufferConstructNop`, `sceAmprCommandBufferNop`, `sceAmprCommandBufferNopWithData`, `sceAmprCommandBufferReset`, `sceAmprAmmCommandBufferUnmap`, `sceAmprAmmCommandBufferRemapIntoPrt`, and `sceAmprAmmCommandBufferUnmapToPrt` access command buffer fields through `CopyGuestIn` and `CopyGuestOut` helpers to avoid direct guest pointer dereferences. Returning `SCE_KERNEL_ERROR_EFAULT` for an unreadable or unwritable pointer is a host-safety fallback; console behaviour on unmapped buffer objects is unmeasured.

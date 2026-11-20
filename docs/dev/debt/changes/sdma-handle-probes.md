# libSceSdma

## add Functional

- [sceSdmaBeginQueueMode, sceSdmaCancelQueue, sceSdmaClose, sceSdmaEndQueueMode, sceSdmaFenceNonBlocking, sceSdmaFlushAll, sceSdmaKickQueue, sceSdmaTryFlush](../../../core/libs/prx/libSceSdma/Export.cpp) (libSceSdma) - validating handles against open slots in `g_slots` and returning `SdmaErrorInvalidArgument` (`0x80990005`) for unmapped or invalid pointers without dereferencing them is a host-safety fallback; console fault behavior on invalid handles has not been measured.

# libSceCesCs

## add Functional

- [sceCesIso2022JpUcsProfileSetSbcs, sceCesGbUcsProfileSet4ByteCharRange and sceCesGbUcsProfileSetUdaMapping](../../../core/libs/prx/libSceCesCs/Export.cpp) (libSceCesCs) - writing profile bytes through `CesGuest::Store`, returning `CES_ERROR_INVALID_PROFILE` while preserving `errno` for unmapped or unwritable sheet pointers, is a host-safety fallback; console fault behavior on invalid buffers has not been measured.

# libharfbuzz

## add Functional

- [hb_ft_face_create_cached](../../../core/libs/prx/libharfbuzz/Export.cpp) (libharfbuzz) - computing the generic destination address from `offsetof(FreeType::GuestFaceRec, generic)` without dereferencing guest pointers, is a host-safety fallback; console fault behavior on invalid buffers has not been measured.

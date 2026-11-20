# libfreetype

## add Functional

- [FT_Set_Default_Properties, FT_Library_SetLcdGeometry, FT_Bitmap_Blend](../../../core/libs/prx/libfreetype/Export.cpp) (libfreetype) - probing the guest library structure and copying the host handle safely via `GuestCopyIn_nid_no_patch` is a host-safety fallback; console fault behavior on invalid library pointers has not been measured.

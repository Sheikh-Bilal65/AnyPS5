# libSceFreeTypeOt

## change FT_Render_Glyph_Internal

- [FT_Render_Glyph_Internal](../../../core/libs/prx/libSceFreeTypeOt/Export.cpp) (NID `8A0cAU4yBVY`, the name `tools/nid_names.py` gives it) - the prototype is FreeType's internal `ftobjs.h` one, unverified for the provider. FreeType exports access parameter pointers through local aliases and safe helpers to avoid direct guest pointer dereferences. A null library or slot, a slot without a face, or a slot that is not the glyph slot of one of the library's faces throws, as rendering with another library's renderers or a slot built outside libSceFreeType is not modelled.

# libSceFont

## change [sceFontStringRefersRenderCharacters](../../../core/libs/prx/libSceFont/src/Text.cpp) follows the shadPS4

- [sceFontStringRefersRenderCharacters](../../../core/libs/prx/libSceFont/src/Text.cpp) follows the shadPS4 implementation (`src/core/libraries/font/font.cpp`, commit 0614ce324b) and its results are unmeasured on a console; parameter pointers across string and render functions are accessed safely through local bounds aliases. The `guest_font_render_characters` test pins only the cases that do not depend on those rules: an empty string, a missing output or string, no start character, a start or last character outside the string, a start after the last character and sub-ranges of a left-to-right run.

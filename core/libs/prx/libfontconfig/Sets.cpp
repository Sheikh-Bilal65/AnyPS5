#include "prx/libfontconfig/include/Sets.hpp"

#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace {

char* CopyObject(const char* object) {
    const std::size_t size = std::strlen(object) + 1;
    char* copy = static_cast<char*>(std::malloc(size));
    if (!copy) return nullptr;
    std::memcpy(copy, object, size);
    return copy;
}

}

extern "C" {

FcFontSet* APS5_VABI FcFontSetCreate() {
    return static_cast<FcFontSet*>(std::calloc(1, sizeof(FcFontSet)));
}

int APS5_VABI FcFontSetAdd(FcFontSet* set, FcPattern* pattern) {
    auto* const fontSet = set;
    auto* const pat = pattern;
    if (!fontSet || !pat || fontSet->nfont < 0 || fontSet->sfont < 0 || fontSet->nfont >= 1024)
        return 0;
    if (fontSet->nfont >= fontSet->sfont / static_cast<int>(sizeof(FcPattern*))) {
        if (fontSet->sfont >= 8192) return 0;
        constexpr int increment = 16 * sizeof(FcPattern*);
        if (fontSet->sfont > std::numeric_limits<int>::max() - increment) return 0;
        const auto next = static_cast<std::size_t>(fontSet->sfont + increment);
        void* storage = std::realloc(fontSet->fonts, next);
        if (!storage) return 0;
        fontSet->fonts = static_cast<FcPattern**>(storage);
        fontSet->sfont = static_cast<int>(next);
    }
    FcPattern* duplicate = FcPatternDuplicate_nid_postfix(pat);
    if (!duplicate) return 0;
    fontSet->fonts[fontSet->nfont++] = duplicate;
    return 1;
}

void APS5_VABI FcFontSetDestroy(FcFontSet* set) {
    auto* const fontSet = set;
    if (!fontSet) return;
    for (int i = 0; i < fontSet->nfont; ++i) FcPatternDestroy_nid_postfix(fontSet->fonts[i]);
    std::free(fontSet->fonts);
    std::free(fontSet);
}

FcObjectSet* APS5_VABI FcObjectSetBuild(const char* first, ...) {
    std::size_t count = first ? 1 : 0;
#ifdef _WIN32
    __builtin_sysv_va_list args;
    __builtin_sysv_va_start(args, first);
#else
    va_list args;
    va_start(args, first);
#endif
    if (first) {
        for (const char* object = va_arg(args, const char*); object;
             object = va_arg(args, const char*)) {
            if (count >= static_cast<std::size_t>(std::numeric_limits<int>::max() / 8)) {
#ifdef _WIN32
                __builtin_sysv_va_end(args);
#else
                va_end(args);
#endif
                return nullptr;
            }
            ++count;
        }
    }
#ifdef _WIN32
    __builtin_sysv_va_end(args);
#else
    va_end(args);
#endif
    if (count > (std::numeric_limits<std::size_t>::max() - sizeof(FcObjectSet)) / sizeof(const char*))
        return nullptr;
    auto* set = static_cast<FcObjectSet*>(std::malloc(sizeof(FcObjectSet) + count * sizeof(const char*)));
    if (!set) return nullptr;
    set->nobject = 0;
    set->sobject = static_cast<int>(count * sizeof(const char*));
    set->objects = reinterpret_cast<const char**>(set + 1);
    if (!first) return set;

#ifdef _WIN32
    __builtin_sysv_va_start(args, first);
#else
    va_start(args, first);
#endif
    bool ok = true;
    for (const char* object = first; object; object = va_arg(args, const char*)) {
        char* copy = CopyObject(object);
        if (!copy) {
            ok = false;
            break;
        }
        set->objects[set->nobject++] = copy;
    }
#ifdef _WIN32
    __builtin_sysv_va_end(args);
#else
    va_end(args);
#endif
    if (!ok) {
        FcObjectSetDestroy(set);
        return nullptr;
    }
    return set;
}

void APS5_VABI FcObjectSetDestroy(FcObjectSet* set) {
    auto* const objectSet = set;
    if (!objectSet) return;
    for (int i = 0; i < objectSet->nobject; ++i)
        std::free(const_cast<char*>(objectSet->objects[i]));
    std::free(objectSet);
}

}

#include "prx/libSceJxr/libSceJxr.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/General.hpp"
#include <array>
#include <cstring>
#include <stdexcept>

extern "C" void JxrMetadataCleanup_nid_no_patch(std::int32_t type, void* payload);

extern "C" long JxrMetadataFree_nid_no_patch(void** memory) {
    return static_cast<long>(PKFree_nid_postfix(memory));
}

extern "C" void APS5_VABI FreeDescMetadata_nid_postfix(Jxr::MetadataVariant* value) {
    auto* const meta = value;
    if (!meta) throw std::invalid_argument("FreeDescMetadata requires a metadata record");
    const auto type = meta->type;
    if (type != 0 && type != 18 && type != 19 && type != 30 && type != 31)
        throw std::invalid_argument("FreeDescMetadata: unsupported metadata tag");
    std::array<std::byte, sizeof(meta->value)> payload;
    std::memcpy(payload.data(), &meta->value, payload.size());
    JxrMetadataCleanup_nid_no_patch(type, payload.data());
    if (type == 30 || type == 31)
        std::memcpy(&meta->value, payload.data(), payload.size());
}

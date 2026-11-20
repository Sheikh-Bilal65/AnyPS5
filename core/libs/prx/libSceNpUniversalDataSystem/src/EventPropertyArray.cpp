#include "prx/libSceNpUniversalDataSystem/include/Api.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include "NpUniversalDataSystem.hpp"
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

namespace {

Uds::Owned<NpUniversalDataSystemEventPropertyArray>& arrays() {
    static Uds::Owned<NpUniversalDataSystemEventPropertyArray> registry;
    return registry;
}

}

extern "C" {

int APS5_VABI sceNpUniversalDataSystemCreateEventPropertyArray(NpUniversalDataSystemEventPropertyArray** new_array) {
    if (new_array == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_INVALID_ARGUMENT;
    auto* created = arrays().Adopt(std::make_unique<NpUniversalDataSystemEventPropertyArray>());
    const int savedErrno = errno;
    const bool copied = GuestCopyOut_nid_no_patch(new_array, &created, sizeof(created));
    errno = savedErrno;
    if (!copied) {
        arrays().Release(created);
        return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_INVALID_ARGUMENT;
    }
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemDestroyEventPropertyArray(NpUniversalDataSystemEventPropertyArray* array) {
    arrays().Release(array);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetString(
    NpUniversalDataSystemEventPropertyArray* array, const char* value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    if (value == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_VALUE;
    arr->Add(std::string(value));
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetInt32(
    NpUniversalDataSystemEventPropertyArray* array, int32_t value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetUInt32(
    NpUniversalDataSystemEventPropertyArray* array, uint32_t value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetInt64(
    NpUniversalDataSystemEventPropertyArray* array, int64_t value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetUInt64(
    NpUniversalDataSystemEventPropertyArray* array, uint64_t value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetFloat32(
    NpUniversalDataSystemEventPropertyArray* array, float value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetFloat64(
NpUniversalDataSystemEventPropertyArray* array, double value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetBool(
    NpUniversalDataSystemEventPropertyArray* array, bool value)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    arr->Add(value);
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetBinary(
    NpUniversalDataSystemEventPropertyArray* array, const void* value, size_t value_size)
{
    auto* const arr = array;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    if (value == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_VALUE;
    const auto* bytes = static_cast<const std::uint8_t*>(value);
    arr->Add(Uds::Binary(bytes, bytes + value_size));
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetObject(
    NpUniversalDataSystemEventPropertyArray* array,
    const NpUniversalDataSystemEventPropertyObject* value,
    NpUniversalDataSystemEventPropertyObject** value_ptr)
{
    auto* const arr = array;
    const auto* const val = value;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    auto child = std::make_shared<NpUniversalDataSystemEventPropertyObject>(val != nullptr ? val->Clone() : NpUniversalDataSystemEventPropertyObject{});
    if (value_ptr != nullptr) {
        auto* pointer = child.get();
        const int savedErrno = errno;
        const bool copied = GuestCopyOut_nid_no_patch(value_ptr, &pointer, sizeof(pointer));
        errno = savedErrno;
        if (!copied) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_INVALID_ARGUMENT;
    }
    arr->Add(std::move(child));
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

int APS5_VABI sceNpUniversalDataSystemEventPropertyArraySetArray(
    NpUniversalDataSystemEventPropertyArray* array,
    const NpUniversalDataSystemEventPropertyArray* value,
    NpUniversalDataSystemEventPropertyArray** value_ptr)
{
    auto* const arr = array;
    const auto* const val = value;
    if (arr == nullptr) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_NULL_ARRAY;
    auto child = std::make_shared<NpUniversalDataSystemEventPropertyArray>(val != nullptr ? val->Clone() : NpUniversalDataSystemEventPropertyArray{});
    if (value_ptr != nullptr) {
        auto* pointer = child.get();
        const int savedErrno = errno;
        const bool copied = GuestCopyOut_nid_no_patch(value_ptr, &pointer, sizeof(pointer));
        errno = savedErrno;
        if (!copied) return SCE_NP_UNIVERSAL_DATA_SYSTEM_ERROR_INVALID_ARGUMENT;
    }
    arr->Add(std::move(child));
    return SCE_NP_UNIVERSAL_DATA_SYSTEM_OK;
}

}

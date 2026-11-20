#include "prx/libSceAgcDriver/Eq/include/Query.hpp"

#include <cstdint>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

extern "C" {

uint32_t APS5_VABI sceAgcDriverGetEqContextId(const KernelEvent* ev) {
    const auto* const e = ev;
    if (e == nullptr || reinterpret_cast<std::uintptr_t>(e) % alignof(KernelEvent) != 0) {
        throw std::runtime_error(std::string(__func__) + ": null or misaligned event");
    }
    if (e->filter != -14) {
        throw std::runtime_error(std::string(__func__) + ": not a graphics event");
    }
    return static_cast<uint32_t>(static_cast<std::uint64_t>(e->data) >> 16u);
}

int APS5_VABI sceAgcDriverGetEqEventType(const KernelEvent* ev) {
    const auto* const e = ev;
    if (e == nullptr || reinterpret_cast<std::uintptr_t>(e) % alignof(KernelEvent) != 0) {
        throw std::runtime_error(std::string(__func__) + ": null or misaligned event");
    }
    if (e->filter == -14) return static_cast<int>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(e->data)));
    if (e->data < std::numeric_limits<int>::min() || e->data > std::numeric_limits<int>::max()) {
        throw std::runtime_error(std::string(__func__) + ": event type overflow");
    }
    return static_cast<int>(e->data);
}

}

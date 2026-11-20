#include "prx/libSceNpManager/include/Api.hpp"
#include "prx/libSceUserService/UserService.hpp"
#include "prx/libc/include/general/ExportMacros.hpp"
#include "prx/libc/include/General.hpp"
#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <string>

namespace {

constexpr int NP_ERROR_INVALID_ARGUMENT = static_cast<int>(0x80550003);
constexpr int NP_ERROR_SIGNED_OUT = static_cast<int>(0x80550006);
constexpr int NP_ERROR_USER_NOT_FOUND = static_cast<int>(0x80550007);
constexpr int NP_ERROR_ONLINE_ID_TOO_LONG = static_cast<int>(0x80559E11);
constexpr int NP_ERROR_USER_INVALID = static_cast<int>(0x80559E2A);
constexpr int NP_ERROR_ACCOUNT_ID_MISSING = static_cast<int>(0x80559E2B);
constexpr std::size_t NP_ONLINE_ID_SIZE = 16;

template <typename T>
bool ReadGuest(const T* source, T& destination) {
    const int savedErrno = errno;
    const bool copied = GuestCopyIn_nid_no_patch(&destination, source, sizeof(destination));
    errno = savedErrno;
    return copied;
}

template <typename T>
bool ProbeGuestWritable(T* destination) {
    T value{};
    const int savedErrno = errno;
    const bool ok = GuestCopyIn_nid_no_patch(&value, destination, sizeof(value)) &&
        GuestCopyOut_nid_no_patch(destination, &value, sizeof(value));
    errno = savedErrno;
    return ok;
}

bool SameOnlineId(const char* lhs, const char* rhs) {
    return std::strncmp(lhs, rhs, NP_ONLINE_ID_SIZE) == 0;
}

void* const NpOnlineIdVtable[] = {
    nullptr, nullptr,
    reinterpret_cast<void*>(&_ZN3sce2np10NpOnlineIdD1Ev),
    reinterpret_cast<void*>(&_ZN3sce2np10NpOnlineIdD0Ev),
};

void* const NpIdVtable[] = {
    nullptr, nullptr,
    reinterpret_cast<void*>(&_ZN3sce2np4NpIdD1Ev),
    reinterpret_cast<void*>(&_ZN3sce2np4NpIdD0Ev),
};

void* const UserVtable[] = {
    nullptr, nullptr,
    reinterpret_cast<void*>(&_ZN3sce2np4UserD1Ev),
    reinterpret_cast<void*>(&_ZN3sce2np4UserD0Ev),
};

void* const NpTitleTokenVtable[] = {
    nullptr, nullptr,
    reinterpret_cast<void*>(&_ZN3sce2np12NpTitleTokenD1Ev),
    reinterpret_cast<void*>(&_ZN3sce2np12NpTitleTokenD0Ev),
};

void* const NpAccessTokenVtable[] = {
    nullptr, nullptr,
    reinterpret_cast<void*>(&_ZN3sce2np13NpAccessTokenD1Ev),
    reinterpret_cast<void*>(&_ZN3sce2np13NpAccessTokenD0Ev),
};

}

extern "C" {

void APS5_VABI _ZN3sce2np10NpOnlineIdC1Ev(NpOnlineIdObject* self) {
    auto* const s = self;
    s->vtable = &NpOnlineIdVtable[2];
    std::memset(s->data, 0, sizeof(s->data));
    s->term = 0;
}

void APS5_VABI _ZN3sce2np10NpOnlineIdC2Ev(NpOnlineIdObject* self) {
    _ZN3sce2np10NpOnlineIdC1Ev(self);
}

void APS5_VABI _ZN3sce2np10NpOnlineIdC1ERKS1_(NpOnlineIdObject* self, const NpOnlineIdObject* other) {
    auto* const s = self;
    const auto* const o = other;
    s->vtable = &NpOnlineIdVtable[2];
    std::memcpy(s->data, o->data, sizeof(s->data));
    s->term = o->term;
}

void APS5_VABI _ZN3sce2np10NpOnlineIdC2ERKS1_(NpOnlineIdObject* self, const NpOnlineIdObject* other) {
    _ZN3sce2np10NpOnlineIdC1ERKS1_(self, other);
}

void APS5_VABI _ZN3sce2np10NpOnlineIdC1ERK13SceNpOnlineId(NpOnlineIdObject* self, const SceNpOnlineId* source) {
    auto* const s = self;
    const auto* const src = source;
    s->vtable = &NpOnlineIdVtable[2];
    std::memcpy(s->data, src->data, sizeof(s->data));
    s->term = src->term;
}

void APS5_VABI _ZN3sce2np10NpOnlineIdC2ERK13SceNpOnlineId(NpOnlineIdObject* self, const SceNpOnlineId* source) {
    _ZN3sce2np10NpOnlineIdC1ERK13SceNpOnlineId(self, source);
}

void APS5_VABI _ZN3sce2np10NpOnlineId5ClearEv(NpOnlineIdObject* self) {
    auto* const s = self;
    std::memset(s->data, 0, sizeof(s->data));
    s->term = 0;
}

int APS5_VABI _ZN3sce2np10NpOnlineId16SetNpOnlineIdStrEPKc(NpOnlineIdObject* self, const char* text) {
    if (text == nullptr) return NP_ERROR_INVALID_ARGUMENT;
    std::string copy;
    const int error = CopyGuestString_nid_no_patch(text, NP_ONLINE_ID_SIZE + 1, copy);
    if (error == 63) return NP_ERROR_ONLINE_ID_TOO_LONG;
    if (error != 0) return NP_ERROR_INVALID_ARGUMENT;
    auto* const s = self;
    std::memset(s->data, 0, sizeof(s->data));
    s->term = 0;
    std::memcpy(s->data, copy.data(), copy.size());
    return 0;
}

bool APS5_VABI _ZNK3sce2np10NpOnlineId7IsEmptyEv(const NpOnlineIdObject* self) {
    const auto* const s = self;
    return s->data[0] == '\0';
}

int APS5_VABI _ZN3sce2np10NpOnlineId13GetNpOnlineIdERKNS0_4UserEP13SceNpOnlineId(const UserObject* user, SceNpOnlineId* out) {
    if (user == nullptr || out == nullptr) return NP_ERROR_INVALID_ARGUMENT;
    UserObject localUser{};
    if (!ReadGuest(user, localUser)) return NP_ERROR_INVALID_ARGUMENT;
    if (!ProbeGuestWritable(out)) return NP_ERROR_INVALID_ARGUMENT;
    if (localUser.userId == USER_SERVICE_USER_ID_INVALID) return NP_ERROR_USER_NOT_FOUND;
    return NP_ERROR_SIGNED_OUT;
}

int APS5_VABI _ZN3sce2np10NpOnlineId13GetNpOnlineIdERKNS0_4UserEPS1_(const UserObject* user, NpOnlineIdObject* out) {
    if (user == nullptr || out == nullptr) return NP_ERROR_INVALID_ARGUMENT;
    UserObject localUser{};
    if (!ReadGuest(user, localUser)) return NP_ERROR_INVALID_ARGUMENT;
    if (!ProbeGuestWritable(out)) return NP_ERROR_INVALID_ARGUMENT;
    if (localUser.userId == USER_SERVICE_USER_ID_INVALID) return NP_ERROR_USER_NOT_FOUND;
    return NP_ERROR_SIGNED_OUT;
}

bool APS5_VABI _ZN3sce2npeqERKNS0_10NpOnlineIdES3_(const NpOnlineIdObject* lhs, const NpOnlineIdObject* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return SameOnlineId(l->data, r->data);
}

bool APS5_VABI _ZN3sce2npeqERK13SceNpOnlineIdRKNS0_10NpOnlineIdE(const SceNpOnlineId* lhs, const NpOnlineIdObject* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return SameOnlineId(l->data, r->data);
}

bool APS5_VABI _ZN3sce2npeqERKNS0_10NpOnlineIdERK13SceNpOnlineId(const NpOnlineIdObject* lhs, const SceNpOnlineId* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return SameOnlineId(l->data, r->data);
}

bool APS5_VABI _ZN3sce2npneERKNS0_10NpOnlineIdES3_(const NpOnlineIdObject* lhs, const NpOnlineIdObject* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return !SameOnlineId(l->data, r->data);
}

bool APS5_VABI _ZN3sce2npneERK13SceNpOnlineIdRKNS0_10NpOnlineIdE(const SceNpOnlineId* lhs, const NpOnlineIdObject* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return !SameOnlineId(l->data, r->data);
}

bool APS5_VABI _ZN3sce2npneERKNS0_10NpOnlineIdERK13SceNpOnlineId(const NpOnlineIdObject* lhs, const SceNpOnlineId* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return !SameOnlineId(l->data, r->data);
}

void APS5_VABI _ZN3sce2np4NpIdC1Ev(NpIdObject* self) {
    auto* const s = self;
    s->vtable = &NpIdVtable[2];
    std::memset(&s->id, 0, sizeof(s->id));
}

void APS5_VABI _ZN3sce2np4NpIdC2Ev(NpIdObject* self) {
    _ZN3sce2np4NpIdC1Ev(self);
}

void APS5_VABI _ZN3sce2np4NpIdC1ERKS1_(NpIdObject* self, const NpIdObject* other) {
    auto* const s = self;
    const auto* const o = other;
    s->vtable = &NpIdVtable[2];
    std::memcpy(&s->id, &o->id, sizeof(s->id));
}

void APS5_VABI _ZN3sce2np4NpIdC2ERKS1_(NpIdObject* self, const NpIdObject* other) {
    _ZN3sce2np4NpIdC1ERKS1_(self, other);
}

void APS5_VABI _ZN3sce2np4NpIdC1ERK7SceNpId(NpIdObject* self, const SceNpId* source) {
    auto* const s = self;
    const auto* const src = source;
    s->vtable = &NpIdVtable[2];
    std::memcpy(&s->id, src, sizeof(s->id));
}

void APS5_VABI _ZN3sce2np4NpIdC2ERK7SceNpId(NpIdObject* self, const SceNpId* source) {
    _ZN3sce2np4NpIdC1ERK7SceNpId(self, source);
}

void APS5_VABI _ZN3sce2np4NpId5ClearEv(NpIdObject* self) {
    auto* const s = self;
    std::memset(&s->id, 0, sizeof(s->id));
}

void APS5_VABI _ZN3sce2np4UserC1Ev(UserObject* self) {
    auto* const s = self;
    s->vtable = &UserVtable[2];
    s->userId = USER_SERVICE_USER_ID_INVALID;
}

void APS5_VABI _ZN3sce2np4UserC2Ev(UserObject* self) {
    _ZN3sce2np4UserC1Ev(self);
}

void APS5_VABI _ZN3sce2np4UserC1Ei(UserObject* self, int userId) {
    auto* const s = self;
    s->vtable = &UserVtable[2];
    s->userId = userId;
}

void APS5_VABI _ZN3sce2np4UserC2Ei(UserObject* self, int userId) {
    _ZN3sce2np4UserC1Ei(self, userId);
}

void APS5_VABI _ZN3sce2np4UserC1ERKS1_(UserObject* self, const UserObject* other) {
    auto* const s = self;
    const auto* const o = other;
    s->vtable = &UserVtable[2];
    s->userId = o->userId;
}

void APS5_VABI _ZN3sce2np4UserC2ERKS1_(UserObject* self, const UserObject* other) {
    _ZN3sce2np4UserC1ERKS1_(self, other);
}

void APS5_VABI _ZN3sce2np4User5ClearEv(UserObject* self) {
    auto* const s = self;
    s->userId = USER_SERVICE_USER_ID_INVALID;
}

bool APS5_VABI _ZNK3sce2np4User7IsEmptyEv(const UserObject* self) {
    const auto* const s = self;
    return s->userId == USER_SERVICE_USER_ID_INVALID;
}

int APS5_VABI _ZNK3sce2np4User9GetUserIdEv(const UserObject* self) {
    const auto* const s = self;
    return s->userId;
}

bool APS5_VABI _ZNK3sce2np4User10IsLoggedInEv(const UserObject* self) {
    const auto* const s = self;
    if (s->userId == USER_SERVICE_USER_ID_INVALID) return false;
    std::uint64_t accountId = 0;
    return sceUserServiceGetNpAccountId(s->userId, &accountId) == 0;
}

bool APS5_VABI _ZNK3sce2np4User7IsGuestEv(const UserObject* self) {
    const auto* const s = self;
    return sceUserServiceIsGuestUser(s->userId) != 0;
}

int APS5_VABI _ZNK3sce2np4User12GetAccountIdEPm(const UserObject* self, std::uint64_t* accountId) {
    if (accountId == nullptr) return NP_ERROR_INVALID_ARGUMENT;
    std::uint64_t value = 0;
    if (!GuestCopyOut_nid_no_patch(accountId, &value, sizeof(value))) return NP_ERROR_INVALID_ARGUMENT;
    const auto* const s = self;
    const int result = sceUserServiceGetNpAccountId(s->userId, &value);
    if (result < 0) return result;
    if (!GuestCopyOut_nid_no_patch(accountId, &value, sizeof(value))) return NP_ERROR_INVALID_ARGUMENT;
    if (value == 0) return NP_ERROR_ACCOUNT_ID_MISSING;
    return 0;
}

int APS5_VABI _ZNK3sce2np4User12HasAccountIdEPb(const UserObject* self, bool* hasAccountId) {
    if (hasAccountId == nullptr) return NP_ERROR_INVALID_ARGUMENT;
    std::uint64_t accountId = 0;
    const auto* const s = self;
    const int result = sceUserServiceGetNpAccountId(s->userId, &accountId);
    if (result < 0) return result;
    const bool has = accountId != 0;
    if (!GuestCopyOut_nid_no_patch(hasAccountId, &has, sizeof(has))) return NP_ERROR_INVALID_ARGUMENT;
    return 0;
}

int APS5_VABI _ZNK3sce2np4User25GetAccountIdFromRegistoryEPm(const UserObject* self, std::uint64_t* accountId) {
    const auto* const s = self;
    const int result = sceUserServiceGetNpAccountId(s->userId, accountId);
    return result < 0 ? result : 0;
}

int APS5_VABI _ZN3sce2np4User7GetUserEiPS1_(int userId, UserObject* out) {
    if (out == nullptr) return NP_ERROR_INVALID_ARGUMENT;
    if (userId == USER_SERVICE_USER_ID_INVALID) return NP_ERROR_USER_INVALID;
    UserServiceRegisteredUserIdList users {};
    const int result = sceUserServiceGetRegisteredUserIdList(&users);
    if (result < 0) return result;
    if (std::find(std::begin(users.user_id), std::end(users.user_id), userId) == std::end(users.user_id)) {
        return NP_ERROR_USER_INVALID;
    }
    auto* const o = out;
    if (!GuestCopyOut_nid_no_patch(&o->userId, &userId, sizeof(userId))) return NP_ERROR_INVALID_ARGUMENT;
    return 0;
}

bool APS5_VABI _ZN3sce2npeqERKNS0_4UserES3_(const UserObject* lhs, const UserObject* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return l->userId == r->userId;
}

bool APS5_VABI _ZN3sce2npeqERKNS0_4UserERKi(const UserObject* lhs, const int* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return l->userId == *r;
}

bool APS5_VABI _ZN3sce2npneERKNS0_4UserES3_(const UserObject* lhs, const UserObject* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return l->userId != r->userId;
}

bool APS5_VABI _ZN3sce2npneERKNS0_4UserERKi(const UserObject* lhs, const int* rhs) {
    const auto* const l = lhs;
    const auto* const r = rhs;
    return l->userId != *r;
}

void APS5_VABI _ZN3sce2np12NpTitleTokenC1Ev(NpTitleTokenObject* self) {
    auto* const s = self;
    s->vtable = &NpTitleTokenVtable[2];
    std::memset(s->data, 0, sizeof(s->data));
}

void APS5_VABI _ZN3sce2np12NpTitleTokenC2Ev(NpTitleTokenObject* self) {
    _ZN3sce2np12NpTitleTokenC1Ev(self);
}

void APS5_VABI _ZN3sce2np12NpTitleTokenC1ERKS1_(NpTitleTokenObject* self, const NpTitleTokenObject* other) {
    auto* const s = self;
    const auto* const o = other;
    s->vtable = &NpTitleTokenVtable[2];
    std::memcpy(s->data, o->data, sizeof(s->data));
}

void APS5_VABI _ZN3sce2np12NpTitleTokenC2ERKS1_(NpTitleTokenObject* self, const NpTitleTokenObject* other) {
    _ZN3sce2np12NpTitleTokenC1ERKS1_(self, other);
}

void APS5_VABI _ZN3sce2np12NpTitleToken5ClearEv(NpTitleTokenObject* self) {
    auto* const s = self;
    std::memset(s->data, 0, sizeof(s->data));
}

const char* APS5_VABI _ZNK3sce2np12NpTitleToken6GetStrEv(const NpTitleTokenObject* self) {
    const auto* const s = self;
    return s->data;
}

void APS5_VABI _ZN3sce2np13NpAccessTokenC1Ev(NpAccessTokenObject* self) {
    auto* const s = self;
    s->vtable = &NpAccessTokenVtable[2];
    std::memset(s->data, 0, sizeof(s->data));
}

void APS5_VABI _ZN3sce2np13NpAccessTokenC2Ev(NpAccessTokenObject* self) {
    _ZN3sce2np13NpAccessTokenC1Ev(self);
}

void APS5_VABI _ZN3sce2np13NpAccessTokenC1ERKS1_(NpAccessTokenObject* self, const NpAccessTokenObject* other) {
    auto* const s = self;
    const auto* const o = other;
    s->vtable = &NpAccessTokenVtable[2];
    std::memcpy(s->data, o->data, sizeof(s->data));
}

void APS5_VABI _ZN3sce2np13NpAccessTokenC2ERKS1_(NpAccessTokenObject* self, const NpAccessTokenObject* other) {
    _ZN3sce2np13NpAccessTokenC1ERKS1_(self, other);
}

void APS5_VABI _ZN3sce2np13NpAccessTokenC1ERK16SceNpAccessToken(NpAccessTokenObject* self, const SceNpAccessToken* source) {
    auto* const s = self;
    const auto* const src = source;
    s->vtable = &NpAccessTokenVtable[2];
    std::memcpy(s->data, src->data, sizeof(s->data));
}

void APS5_VABI _ZN3sce2np13NpAccessTokenC2ERK16SceNpAccessToken(NpAccessTokenObject* self, const SceNpAccessToken* source) {
    _ZN3sce2np13NpAccessTokenC1ERK16SceNpAccessToken(self, source);
}

void APS5_VABI _ZN3sce2np13NpAccessToken5ClearEv(NpAccessTokenObject* self) {
    auto* const s = self;
    std::memset(s->data, 0, sizeof(s->data));
}

bool APS5_VABI _ZNK3sce2np13NpAccessToken7IsEmptyEv(const NpAccessTokenObject* self) {
    const auto* const s = self;
    std::uint8_t empty[sizeof(s->data)] {};
    return std::memcmp(s->data, empty, sizeof(s->data)) == 0;
}

void* APS5_VABI _ZNK3sce2np8NpTicket13GetTicketDataEv(const NpTicketObject* self) {
    const auto* const s = self;
    if (s->data == nullptr) throw std::runtime_error("NpTicket data is empty");
    return s->data;
}

std::size_t APS5_VABI _ZNK3sce2np8NpTicket13GetTicketSizeEv(const NpTicketObject* self) {
    const auto* const s = self;
    return s->size;
}

}

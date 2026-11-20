# libSceNpManager

## change NpOnlineId, NpId, User, NpTitleToken, NpAccessToken and NpTicket value methods

- [NpOnlineId, NpId, User, NpTitleToken, NpAccessToken and NpTicket value methods](../../../core/libs/prx/libSceNpManager/ValueClasses.cpp) (libSceNpManager) - constructors, `Clear`, comparisons, `SetNpOnlineIdStr`, `GetStr`, `IsEmpty`, `GetUserId` and User account lookups follow firmware 11.00 `libSceNpManager.sprx` disassembly, and parameter pointers are safely bound to local aliases. `SetNpOnlineIdStr` refuses >16 characters with `0x80559E11`. `GetUser` returns `0x80559E2A` for an invalid or unregistered id, and `GetAccountId` returns `0x80559E2B` for zero account id. `GetNpOnlineId` returns `0x80550007` for invalid user and `0x80550006` otherwise. `NpTicket::GetTicketData` throws for null data.

# libScePsml_debug

## add Guest pointers

- [scePsmlMfsrGetSharedResourcesInitRequirement, scePsmlMfsrCreateSharedResources, scePsmlMfsrCreateContext, scePsmlMfsrGetDispatchMfsrPacketSizeInDwords, scePsmlMfsrGetMipmapBias](../../../core/libs/prx/libScePsml_debug/Export.cpp) (libScePsml_debug) - safely probe and copy guest memory for shared resources parameters, requirement outputs, context parameters, handles, packet size outputs and mipmap bias outputs using CopyInPreservingErrno and CopyOutPreservingErrno rather than dereferencing guest pointers directly.

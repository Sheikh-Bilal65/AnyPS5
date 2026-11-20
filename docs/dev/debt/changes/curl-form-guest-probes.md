# libcurl

## change [`curl_formadd`]

- [`curl_formadd`](../../../core/libs/prx/libcurl/Export.cpp) - the guest node layout is the unverified LP64 mirror of curl 7.81.0's `struct curl_httppost`. On LLP64 hosts a `CURLFORM_NAMELENGTH` or `BUFFERLENGTH` value that does not fit a host `long` throws `std::out_of_range`, like the Guest long entry above. Guest varargs and arrays are passed to curl 7.81.0's [FormAdd](https://github.com/curl/curl/blob/801bd5138ce31aa0d906fa4e2eabfc599d74e793/lib/formdata.c#L203-L690) on an empty list; it allocates names, contents and nodes through the guest heap of the Allocator entry. The nodes it adds are copied into guest nodes and linked as `AddHttpPost` does: after `*last_post`, or as `*httppost` when the list is empty; probing pointers with `GuestCopyIn_nid_no_patch` and `GuestWritable_nid_no_patch` is a host-safety fallback with console behavior unmeasured. A null `CURLFORM_ARRAY` returns the error of an earlier option or `CURL_FORMADD_NULL`, with nothing added.

## change [`curl_formfree`]

- [`curl_formfree`](../../../core/libs/prx/libcurl/src/FormFree.cpp) - the guest node layout is the unverified LP64 mirror of curl 7.81.0's `struct curl_httppost`, and nodes are released through the guest heap described under Allocator. Adapted from [lib/formdata.c](https://github.com/curl/curl/blob/801bd5138ce31aa0d906fa4e2eabfc599d74e793/lib/formdata.c#L746-L771): it frees each node of the `next` chain after its `more` chain, keeping names marked `CURL_HTTPPOST_PTRNAME` and contents marked `PTRCONTENTS`, `BUFFER` or `CALLBACK`; reading each node via `GuestCopyIn_nid_no_patch` is a host-safety fallback with console behavior unmeasured.

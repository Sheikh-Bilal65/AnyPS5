# libkernel

## add Silent stubs

- [Timer, event flag, and semaphore timeout bounds](../../../core/libs/prx/libkernel/Time/Time.cpp) (libkernel) - clock queries, POSIX ktimer creation, event flag waits, and semaphore wait timeouts access guest parameter pointers through safe copy helpers and bounds aliases.

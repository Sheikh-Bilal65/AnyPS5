# libkernel

## add Robustness

- [Pthread guest pointer probing](../../../core/libs/prx/libkernel/Pthread/src/Thread.cpp) (libkernel) - POSIX thread, attribute, mutex, condition variable, read-write lock, semaphore, spinlock, and one-time initialization exports access guest parameters and outputs through guarded references and checked copies without unvalidated writes.

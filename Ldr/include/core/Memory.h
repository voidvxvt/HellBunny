#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

#include <common/phnt.h>

PVOID
MmHeapAlloc(
    _In_ ULONG Length
);

PVOID
MmHeapReAlloc(
    _In_ PVOID Memory,
    _In_ ULONG Length
);

PVOID
MmHeapFree(
    _In_ PVOID Memory
);

#endif
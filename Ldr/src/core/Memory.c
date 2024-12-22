#include <Ldr.h>
#include <core/Memory.h>
#include <core/MiniStd.h>

PVOID
MmHeapAlloc(
    _In_ ULONG Length
) {
    return Instance->NtDll.RtlAllocateHeap( NtProcessHeap(), HEAP_ZERO_MEMORY, Length );
}

PVOID
MmHeapReAlloc(
    _In_ PVOID Memory,
    _In_ ULONG Length
) {
    return Instance->NtDll.RtlReAllocateHeap( NtProcessHeap(), HEAP_ZERO_MEMORY, Memory, Length );
}

PVOID
MmHeapFree(
    _In_ PVOID Memory
) {
    return Instance->NtDll.RtlFreeHeap( NtProcessHeap(), HEAP_ZERO_MEMORY, Memory );
}

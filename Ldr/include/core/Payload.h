#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <Ldr.h>
#include <common/phnt.h>


BOOL
DownloadPayload(
    OUT PBUFFER PayloadBuffer
);

DWORD
HttpQueryStatus(
    _In_ PVOID Request
);

VOID 
DecryptPayload(
    IN OUT PBUFFER PayloadBuffer
);


#endif // PAYLOAD_H
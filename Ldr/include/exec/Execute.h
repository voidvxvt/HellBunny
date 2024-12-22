#ifndef EXECUTE_H
#define EXECUTE_H

#include <Ldr.h>
#include <common/phnt.h>

#define OPT

BOOL
ExecutePayload(
    IN PBUFFER PayloadBuffer
);

NTSTATUS
ExecClassicInjection(
    IN     PBUFFER PayloadBuffer,
    IN OPT HANDLE  ProcessHandle
);


#endif//EXECUTE_H

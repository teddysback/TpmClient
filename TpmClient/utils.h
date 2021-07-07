#pragma once

#include "main.h"


BOOLEAN
isLittleEndian(
    VOID
);

VOID
printDevInfo(
    _In_ TPM_DEVICE_INFO DevInfo
);

VOID
endianSwap32(
    _Inout_ PUINT32 Data
);

VOID
endianSwap16(
    _Inout_ PUINT16 Data
);

UINT16
Swap16Bit(
    _In_ UINT16 Data
);

UINT32
Swap32Bit(
    _In_ UINT32 Data
);

/**
 * Determine if Data needs endian swap and write to Buffer.
 */
VOID
writeTpmCommand(
    _Out_writes_bytes_(DataSize) PVOID   Buffer,        // Storage location
    _In_                         SIZE_T  BufferSize,    // Size in bytes
    _Inout_                      PSIZE_T Offset,        // Number of bytes from origin
    _In_                         UINT32  Data,          // Data to be written
    _In_                         SIZE_T  DataSize       // Data size in bytes
);

/*
 * Print struct <Type> from command buffer
 */
VOID
printCommand(
    _In_      PVOID        Buffer,
    _In_ enum TPM_CMD_TYPE Type
);

/*
 * Alloc command
 *
 * Return:
 *      - Command = valid pointer 
 *      - Command = NULL 
 *
 * \warning: use deleteCommand() to free alloc 
 */
PVOID
createCommand(
    ///_Inout_ PVOID        Command, // PTPM_GET_RAND_IN || PTPM_GET_RAND_OUT
    _In_    enum TPM_CMD_TYPE Type,
    _In_opt_ UINT16 BytesRequested
);

/*
 * Free memory
 */
VOID
deleteCommand(
    _Inout_ PVOID Buffer
);

/*
 * Init Command request for random
 */
VOID
initCommandRquRand(
    _Inout_  PVOID Buffer,
    _In_     TPM_CMD_TYPE Type,
    _In_opt_ UINT16 BytesRequested
);
#include "utils.h"


BOOLEAN
isLittleEndian(
    VOID
)
{
    UINT32 x = 1;

    return !!(((CHAR *)&x)[0]);
}


VOID
printDevInfo(
    _In_ TPM_DEVICE_INFO DevInfo
)
{
    printf_s("TPM Version: [%u]\n", DevInfo.tpmVersion);
    printf_s("TPM InterfaceType: ");
    if (DevInfo.tpmInterfaceType == TPM_IFTYPE_UNKNOWN  ) printf_s("[unknown]\n");
    if (DevInfo.tpmInterfaceType == TPM_IFTYPE_1        ) printf_s("[I/O-port or MMIO]\n");
    if (DevInfo.tpmInterfaceType == TPM_IFTYPE_TRUSTZONE) printf_s("[Trustzone]\n");
    if (DevInfo.tpmInterfaceType == TPM_IFTYPE_HW       ) printf_s("[HW TPM]\n");
    if (DevInfo.tpmInterfaceType == TPM_IFTYPE_EMULATOR ) printf_s("[SW-emulator]\n");
    

    return;
}


VOID
endianSwap32(
    _Inout_ PUINT32 Data
)
{
    assert(Data != NULL);

    *Data = (((*Data << 24) & 0xFF000000) | ((*Data << 8) & 0x00FF0000) | ((*Data >> 8) & 0x0000FF00) | ((*Data >> 24) & 0x000000FF));
}

VOID
endianSwap16(
    _Inout_ PUINT16 Data
)
{
    assert(Data != NULL);

    *Data = (*Data << 8) | (*Data >> 8);
}

UINT16
Swap16Bit(
    _In_ UINT16 Data
)
{
    return (Data << 8) | (Data >> 8);
}

UINT32
Swap32Bit(
    _In_ UINT32 Data
)
{
    return (((Data << 24) & 0xFF000000) | ((Data << 8) & 0x00FF0000) | ((Data >> 8) & 0x0000FF00) | ((Data >> 24) & 0x000000FF));
}



VOID
writeTpmCommand(
    _Out_writes_bytes_(DataSize) PVOID   Buffer,        // Storage location
    _In_                         SIZE_T  BufferSize,    // Size in bytes
    _Inout_                      PSIZE_T Offset,        // Number of bytes from origin
    _In_                         UINT32  Data,          // Data to be written
    _In_                         SIZE_T  DataSize       // Data size in bytes
)
{
    PBYTE bufferCursor = NULL;

    assert(Buffer != NULL);
    assert(Offset != NULL);
    assert(*Offset + DataSize <= BufferSize);

    bufferCursor = (PBYTE)Buffer + *Offset;

    if (isLittleEndian())
    {
        dbg("endian swap before: [0x%x] \n", Data);
        if (DataSize == sizeof(UINT32))
        {
            endianSwap32(&Data);
        }
        else
        {
            endianSwap16((PUINT16)&Data);
        }
        dbg("endian swap after: [0x%x] \n", Data);
    }
    CopyMemory(bufferCursor, (PVOID)&Data, DataSize);

    *Offset = *Offset + DataSize;

    return;
}


VOID
printCommand(
    _In_ PVOID        Buffer,
    _In_ TPM_CMD_TYPE Type
)
{
    assert(Buffer != NULL);

    switch (Type)
    {
        case TpmRandRqu:
        {
            PTPM_RAND_REQUEST buf = (PTPM_RAND_REQUEST)Buffer;

            dbg("TPM GetRand IN cmd \n");
            dbg("\tTag            [0x%x] \n", buf->Tag);
            dbg("\tParamSize      [0x%x] \n", buf->ParamSize);
            dbg("\tOrdinal        [0x%x] \n", buf->Ordinal);
            dbg("\tBytesRequested [0x%x] \n", buf->BytesRequested);

            break;
        }
        case TpmRandRsp:
        {
            PTPM_RAND_RESPONSE buf = (PTPM_RAND_RESPONSE)Buffer;

            dbg("TPM GetRand OUT cmd \n");
            dbg("\tTag             [0x%x] \n", buf->Tag);
            dbg("\tParamSize       [0x%x] \n", buf->ParamSize);
            dbg("\tReturnCode      [0x%x] \n", buf->ReturnCode);
            dbg("\tOrdinal         [0x%x] \n", buf->Ordinal);
            dbg("\tRandomBytesSize [0x%x] \n", buf->RandomBytesSize);

            break;
        }
        default:
        {
            dbg("UNKOWN COMMAND [%u]", Type);
            break;
        }
    }

    return;
}


PVOID
createCommand(
    ///_Inout_ PVOID        Command, // PTPM_GET_RAND_IN || PTPM_GET_RAND_OUT
    _In_    TPM_CMD_TYPE Type,
    _In_opt_ UINT16 BytesRequested
)
{
    SIZE_T size = 0;
    PVOID  buffer  = NULL;

    switch (Type)
    {
        case TpmRandRqu:
            size = sizeof(TPM_RAND_REQUEST);

            buffer = (PTPM_RAND_REQUEST)malloc(BytesRequested);
            if (buffer == NULL)
            {
                dbg("[ERROR] malloc failed");
                break;
            }
            ZeroMemory(buffer, BytesRequested);

            initCommandRquRand(buffer, Type, BytesRequested);

            break;
        case TpmRandRsp:
            
            size = sizeof(TPM_RAND_RESPONSE) + BytesRequested * sizeof(BYTE);
            buffer = (PTPM_RAND_RESPONSE)malloc(size);
            if (buffer == NULL)
            {
                dbg("[ERROR] malloc failed");
                break;
            }
            ZeroMemory(buffer, size);

            size = sizeof(BYTE) * BytesRequested;
            ((PTPM_RAND_RESPONSE)buffer)->RandomBytes = (PBYTE)malloc(size);
            if (((PTPM_RAND_RESPONSE)buffer)->RandomBytes == NULL)
            {
                dbg("[ERROR] malloc failed");
                break;
            }
            ZeroMemory(((PTPM_RAND_RESPONSE)buffer)->RandomBytes, size);

            break;
        default:
            dbg("UNKWOWN TYPE [%u] \n ", Type);
            return NULL;
    }

    


    return buffer;
}


VOID
deleteCommand(
    _Inout_ PVOID Buffer
)
{
    assert(Buffer != NULL);

    free(Buffer);
    Buffer = NULL;

    return;
}


VOID
initCommandRquRand(
    _Inout_  PVOID Buffer,
    _In_     TPM_CMD_TYPE Type,
    _In_opt_ UINT16 BytesRequested
)
{
    SIZE_T size = 0;
    SIZE_T offset = 0;
    UINT16 x = 0xC1;
    assert(Buffer != NULL);

    switch (Type)
    {
    case TpmRandRqu:

        printCommand(Buffer, TpmRandRqu);

        size = sizeof(TPM_RAND_REQUEST);

        ((PTPM_RAND_REQUEST)Buffer)->Tag            = (UINT16)TPM_TAG_RQU_COMMAND;
        ((PTPM_RAND_REQUEST)Buffer)->ParamSize      = (UINT32)size;
        ((PTPM_RAND_REQUEST)Buffer)->Ordinal        = (UINT32)TPM_ORD_GETRANDOM;
        ((PTPM_RAND_REQUEST)Buffer)->BytesRequested = (UINT32)BytesRequested;

        writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->Tag, sizeof(UINT16));
        writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->ParamSize, sizeof(UINT32));
        writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->Ordinal, sizeof(UINT32));
        writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->BytesRequested, sizeof(UINT32));

        printCommand(Buffer, TpmRandRqu);

        break;
    case TpmRandRsp:
        size = sizeof(PTPM_RAND_RESPONSE);;
        //
        //((PTPM_RAND_RESPONSE)Buffer)->Tag = (UINT32)TPM_TAG_RQU_COMMAND;
        //((PTPM_RAND_RESPONSE)Buffer)->ParamSize = size;
        //((PTPM_RAND_RESPONSE)Buffer)->Ordinal = TPM_ORD_GETRANDOM;
       // ((PTPM_RAND_RESPONSE)Buffer)->BytesRequested = 100;

        //writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->Tag, sizeof(UINT16));
        //writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->ParamSize, sizeof(UINT32));
        //writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->Ordinal, sizeof(UINT32));
        //writeTpmCommand(Buffer, size, &offset, ((PTPM_RAND_REQUEST)Buffer)->BytesRequested, sizeof(UINT32));


        break;
    default:
        dbg("UNKWOWN TYPE [%u] \n ", Type);
    }

    


    

    return;
}

// todo: add _In_ buffer and cast it to contexthandle to submit

//void submitCommand()
//{
//
//    auto retCode = Tbsip_Submit_Command(
//        gTpm.ContextHandle,
//        TBS_COMMAND_LOCALITY_ZERO,      // only one currently supported
//        TBS_COMMAND_PRIORITY_MAX,    // Used for normal priority application use. // [!] TBS_COMMAND_PRIORITY_HIGH - Used for high priority application use.
//        (PCBYTE)requestCmd, sizeof(TPM_RAND_REQUEST),
//        (PBYTE)requestCmd, &resBytes                       // [!] If the supplied buffer is too small, this parameter, on output, is set to the required size, in bytes, for the result.
//    );
//
//    printf_s("\n after resBytes: [%d]\n", resBytes);
//
//    if (retCode != TBS_SUCCESS)
//    {
//        printf_s("\n\nDBG");
//        LOG_ERRCODE(retCode);
//        //break;
//    }
//}
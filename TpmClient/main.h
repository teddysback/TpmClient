#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <Windows.h>
//#include <Winuser.h>
#include <string.h>
#include <assert.h>
#include <tbs.h>

#include "utils.h"
#include "op.h"
#include "dbg.h"




/*
* User defined structures 
*/

enum TPM_CMD_TYPE
{
    TpmRandRqu = 0,    // type TPM_RAND_REQUEST
    TpmRandRsp         // type TPM_RAND_RESPONSE

};

/**
* source:
*   https://trustedcomputinggroup.org/wp-content/uploads/TPM-Main-Part-3-Commands_v1.2_rev116_01032011.pdf
*   https://trustedcomputinggroup.org/wp-content/uploads/TPM-Main-Part-2-TPM-Structures_v1.2_rev116_01032011.pdf
*
*TPM_GetRandom returns the next bytesRequested bytes from the random number
* generator to the caller.
* It is recommended that a TPM implement the RNG in a manner that would allow it to return
* RNG bytes such that the frequency of bytesRequested being more than the number of bytes
* available is an infrequent occurrence.
*
*/


#define TPM_TAG_RQU_COMMAND  (0x00C1)           // A command with no authentication
#define TPM_TAG_RSP_COMMAND  (0x00C4)           // A response from a command with no authentication

#define TPM_ORD_GETRANDOM       (0x00000046)       // Command ordinal

#pragma pack(push, 1)
typedef struct  _TPM_RAND_REQUEST
{
    UINT16 Tag;                     // TPM_TAG_RQU_COMMAND 
    UINT32 ParamSize;               // Total number of input bytes including paramSize and tag
    UINT32 Ordinal;                 // Command ordinal: TPM_ORD_GetRandom
    UINT32 BytesRequested;          // Number of bytes to return

}TPM_RAND_REQUEST, *PTPM_RAND_REQUEST;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct  _TPM_RAND_RESPONSE
{
    UINT16 Tag;                     // TPM_TAG_RSP_COMMAND
    UINT32 ParamSize;               // Total number of output bytes including paramSize and 
    UINT32 ReturnCode;              // The return code of the operation
    UINT32 Ordinal;                 // TPM_ORD_GetRandom 
    UINT32 RandomBytesSize;         // The number of bytes returned
    PBYTE  RandomBytes;             // The returned bytes

}TPM_RAND_RESPONSE, *PTPM_RAND_RESPONSE;
#pragma pack(pop)

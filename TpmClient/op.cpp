#include "op.h"

#include "stdio.h"
#include "errorString.h"


typedef struct _TPM_DEV_EX
{
    TBS_CONTEXT_PARAMS2 ContextParams;
    TBS_HCONTEXT        ContextHandle;

}TPM_DEV_EX, *PTPM_DEV_EX;

TPM_DEV_EX gTpm;

DWORD
TpmInit(
    VOID
)
{
    TBS_RESULT      res = 0;
    TPM_DEVICE_INFO devInfo = { 0 };

    ZeroMemory(&gTpm, sizeof(gTpm));

    res = Tbsi_GetDeviceInfo(sizeof(devInfo), &devInfo);
    if (res != TBS_SUCCESS)
    {
        LOG_ERRCODE(res);
        return -1;
    }

    printDevInfo(devInfo);

    if (devInfo.tpmVersion == TPM_VERSION_20)
    {
        printf_s("%s\n", "Setting context to version 2.0");
        gTpm.ContextParams.version = TBS_CONTEXT_VERSION_TWO;
        gTpm.ContextParams.includeTpm20 = 1;
        gTpm.ContextParams.includeTpm12 = 0;
        //gTpm.ContextParams.requestRaw = 1;
    }
    else
    {
        printf_s("%s\n", "Setting context to version 1.2");
        gTpm.ContextParams.version = TBS_CONTEXT_VERSION_ONE;
    }

    res = Tbsi_Context_Create((PCTBS_CONTEXT_PARAMS)&gTpm.ContextParams, &gTpm.ContextHandle);///?
    if (res != TBS_SUCCESS)
    {
        LOG_ERRCODE(res);
        return -1;
    }

    return 0;
}


VOID
TpmUnint(
    VOID
)
{
    printf("[CLEANUP] Started ... \n ");

    if (gTpm.ContextHandle != NULL)
    {
        Tbsip_Context_Close(gTpm.ContextHandle);
        gTpm.ContextHandle = NULL;
    }

    printf("[CLEANUP] done \n ");
    return;
}

void
RunRandom(
    _In_ UINT32 NoBytesRequested
)
{
    UINT32 resBytes = 4 * 1024; // resBytes * sizeof(BYTE) + sizeof(TPM_RAND_RESPONSE) + sizeof(UINT32);

                                //how many times? the number of bytes req?
    for (int i = 0; i < 2; ++i)
    {
        auto requestCmd = (PTPM_RAND_REQUEST)createCommand(TpmRandRqu, resBytes);

        printf_s("\n    [[  requestCmd  ]]");
        printf_s("\nTag             [0x%x]", Swap16Bit(requestCmd->Tag));
        printf_s("\nParamSize       [%d]", Swap32Bit(requestCmd->ParamSize));
        printf_s("\nOrdinal         [0x%x]", Swap32Bit(requestCmd->Ordinal));
        printf_s("\nBytesRequested: [%d]", Swap32Bit(requestCmd->BytesRequested));

        auto retCode = Tbsip_Submit_Command(
            gTpm.ContextHandle,
            TBS_COMMAND_LOCALITY_ZERO,      // only one currently supported
            TBS_COMMAND_PRIORITY_NORMAL,    // Used for normal priority application use. // [!] TBS_COMMAND_PRIORITY_HIGH - Used for high priority application use.
            (PCBYTE)requestCmd, sizeof(TPM_RAND_REQUEST),
            (PBYTE)requestCmd, &resBytes                       // [!] If the supplied buffer is too small, this parameter, on output, is set to the required size, in bytes, for the result.
        );

        printf_s("\n after resBytes: [%d]\n", resBytes);

        if (retCode != TBS_SUCCESS)
        {
            printf_s("\n\nDBG");
            LOG_ERRCODE(retCode);
            //break;
        }

        auto response = ((PTPM_RAND_RESPONSE)requestCmd);
        if (Swap32Bit(response->ReturnCode) != 0 || Swap16Bit(response->Tag) != TPM_TAG_RSP_COMMAND)
        {
            printf_s("\nTbsip_Submit_Command failed! error: [0x%x] tag [0x%x]", Swap32Bit(response->ReturnCode), Swap16Bit(response->Tag));
        }

        printf_s("\n\n[[response]]");

        printf_s("\nTag              [0x%x]", Swap16Bit(response->Tag));
        printf_s("\nParamSize        [%d]", Swap32Bit(response->ParamSize));
        printf_s("\nReturnCode       [0x%x]", Swap32Bit(response->ReturnCode));
        printf_s("\nOrdinal          [0x%x]", Swap32Bit(response->Ordinal));
        printf_s("\nRandomBytesSize: [%d]", Swap32Bit(response->RandomBytesSize));
        printf_s("\nRandomBytes      [%s]", response->RandomBytes);


        deleteCommand(requestCmd);
        requestCmd = NULL;
        /// 
        /*deleteCommand(responseCmd);
        responseCmd = NULL;*/
    }
}


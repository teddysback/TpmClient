#pragma once
#include "tbs.h"
#include "winerror.h"
#include "stdio.h"


#define LOG_ERRCODE(E)      printError(E); printErrorDesc(E);


__forceinline void
printError(int errorCode)
{
    if (errorCode == TBS_SUCCESS)                   printf_s("[0x%08x] TBS_SUCCESS                  \n", errorCode);
    if (errorCode == TBS_E_BAD_PARAMETER)           printf_s("[0x%08x] TBS_E_BAD_PARAMETER          \n", errorCode);
    if (errorCode == TBS_E_INTERNAL_ERROR)          printf_s("[0x%08x] TBS_E_INTERNAL_ERROR         \n", errorCode);
    if (errorCode == TBS_E_INVALID_CONTEXT_PARAM)   printf_s("[0x%08x] TBS_E_INVALID_CONTEXT_PARAM  \n", errorCode);
    if (errorCode == TBS_E_INVALID_OUTPUT_POINTER)  printf_s("[0x%08x] TBS_E_INVALID_OUTPUT_POINTER \n", errorCode);
    if (errorCode == TBS_E_SERVICE_DISABLED)        printf_s("[0x%08x] TBS_E_SERVICE_DISABLED       \n", errorCode);
    if (errorCode == TBS_E_SERVICE_NOT_RUNNING)     printf_s("[0x%08x] TBS_E_SERVICE_NOT_RUNNING    \n", errorCode);
    if (errorCode == TBS_E_SERVICE_START_PENDING)   printf_s("[0x%08x] TBS_E_SERVICE_START_PENDING  \n", errorCode);
    if (errorCode == TBS_E_TOO_MANY_TBS_CONTEXTS)   printf_s("[0x%08x] TBS_E_TOO_MANY_TBS_CONTEXTS  \n", errorCode);
    if (errorCode == TBS_E_TPM_NOT_FOUND)           printf_s("[0x%08x] TBS_E_TPM_NOT_FOUND          \n", errorCode);
    else                                            printf_s("[0x%08x] unknown                      \n", errorCode);
}

__forceinline void
printErrorDesc(int errorCode)
{
    if (errorCode == TBS_SUCCESS)                   printf_s("The function was successful.\n");
    if (errorCode == TBS_E_BAD_PARAMETER)           printf_s("One or more parameter values are not valid.\n");
    if (errorCode == TBS_E_INTERNAL_ERROR)          printf_s("An internal software error occurred.\n");
    if (errorCode == TBS_E_INVALID_CONTEXT_PARAM)   printf_s("A context parameter that is not valid was passed when attempting to create a TBS context.\n");
    if (errorCode == TBS_E_INVALID_OUTPUT_POINTER)  printf_s("A specified output pointer is not valid.\n");
    if (errorCode == TBS_E_SERVICE_DISABLED)        printf_s("The TBS service has been disabled.\n");
    if (errorCode == TBS_E_SERVICE_NOT_RUNNING)     printf_s("The TBS service is not running and could not be started.\n");
    if (errorCode == TBS_E_SERVICE_START_PENDING)   printf_s("The TBS service has been started but is not yet running.\n");
    if (errorCode == TBS_E_TOO_MANY_TBS_CONTEXTS)   printf_s("A new context could not be created because there are too many open contexts.\n");
    if (errorCode == TBS_E_TPM_NOT_FOUND)           printf_s("A compatible Trusted Platform Module(TPM) Security Device cannot be found on this computer.\n");
    else                                            printf_s("(N/A)\n");
}
 



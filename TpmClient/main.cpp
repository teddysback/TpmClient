#include "main.h"


VOID
RunCommand(
    _In_ enum TPM_CMD_TYPE Type
)
{

}

#define TPM_ARG_MAX_LEN         (8)
#define TPM_USER_INPUT_MAX_LEN  (64)
#define TPM_ALWAYS_TRUE         (TRUE)


VOID
ParseInputAndExec(
    void
)
{
    CHAR userIput[TPM_USER_INPUT_MAX_LEN] = "";

    while (TPM_ALWAYS_TRUE)
    {
        printf_s(">");
        if (fgets(userIput, TPM_USER_INPUT_MAX_LEN, stdin) == NULL)
        {
            printf_s("[input error] could not read input!");
            continue;
        }
        userIput[strlen(userIput) - 1] = 0;
        printf_s("\n");

        if (0 == strcmp(userIput, "exit"))
        {
            break;
        }
        else if (0 == strcmp(userIput, "rand"))
        {
            RunRandom(1);
        }
        else
        {
            printf_s("[input error] unkown:[%s]\n", userIput);
            //call help
        }
    }
}


void
main()
{
    // check args?

    __try
    {
        if (TpmInit())
        {
            dbg("[ERROR] InitTpm failed");
            __leave;
        }

        ParseInputAndExec();
        
       /// RunRandom(1);
        


    }
    __finally
    {
        TpmUnint();

        printf_s("bye \n");
        getchar();
    }

    return;
}
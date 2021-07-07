#pragma once

#include "main.h"

DWORD
TpmInit(
    VOID
);

VOID
TpmUnint(
    VOID
);

void
RunRandom(
    _In_ UINT32 NoBytesRequested
);
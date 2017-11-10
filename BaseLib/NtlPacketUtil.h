#pragma once

#include "Base.h"
#include <iostream>

const char* NtlGetPacketName(WORD wOpCode);

bool NtlCheckPacketName(FILE * fp = NULL);
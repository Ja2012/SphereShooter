// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

class SPHERESHOOTER_API SsUtils
{
public:
    FText static TextFromInt(int64 Number) { return FText::FromString(FString::FromInt(Number)); }
};

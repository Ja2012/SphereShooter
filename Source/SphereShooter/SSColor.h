// LICENSE.md

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESSColor : uint8
{
    ESSC_Green UMETA(DisplayName = "Green"),
    ESSC_Blue UMETA(DisplayName = "Blue"),
    ESSC_Red UMETA(DisplayName = "Red"),
    ESSC_Cyan UMETA(DisplayName = "Cyan"),
    ESSC_Purple UMETA(DisplayName = "Purple"),
    ESSC_Orange UMETA(DisplayName = "Orange")
};

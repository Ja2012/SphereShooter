// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "SsGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SPHERESHOOTER_API ASsGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
    SIZE_T PlayerPoints {0};
    uint8 MissesCount {0};
	
};

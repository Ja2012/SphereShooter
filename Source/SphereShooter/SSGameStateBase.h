// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SSGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SPHERESHOOTER_API ASSGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
    SIZE_T PlayerPoints = 0;
    uint8 MissesCount = 0;
	
};

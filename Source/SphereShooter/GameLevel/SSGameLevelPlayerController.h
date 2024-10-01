// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SSGameLevelPlayerController.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSGameLevelPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};

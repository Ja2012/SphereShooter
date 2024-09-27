// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSMainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPHERESHOOTER_API ASSMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};

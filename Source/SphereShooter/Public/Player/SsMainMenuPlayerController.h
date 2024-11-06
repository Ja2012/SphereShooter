// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SsMainMenuPlayerController.generated.h"


UCLASS()
class SPHERESHOOTER_API ASsMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};

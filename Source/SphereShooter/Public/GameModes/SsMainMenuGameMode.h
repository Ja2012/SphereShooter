// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SsMainMenuGameMode.generated.h"

class ASsMainMenuPlayerController;
class ASsGameStateBase;
class ASsPlayerState;
class USsGameInstance;
class ASsMainMenuHUD;

UCLASS()
class SPHERESHOOTER_API ASsMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASsMainMenuGameMode();

protected:
    virtual void BeginPlay() override;

	UPROPERTY()
    TObjectPtr<ASsMainMenuPlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<ASsGameStateBase> MyGameState;

    UPROPERTY()
    TObjectPtr<ASsPlayerState> PlayerState;

    UPROPERTY()
    TObjectPtr<USsGameInstance> GameInstance;

    UPROPERTY()
    TObjectPtr<ASsMainMenuHUD> HUD;

	
};

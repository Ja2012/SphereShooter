// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SSGameLevelPlayerController.generated.h"

class ASSGameLevelGameMode;
class ASSPawn;

UCLASS()
class SPHERESHOOTER_API ASSGameLevelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

private:
    TObjectPtr<ASSGameLevelGameMode> SSGameMode;
    TObjectPtr<ASSPawn> SSPawn;
    FVector RollBallSpawnLoc;
};

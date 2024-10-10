// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SsGameLevelPlayerController.generated.h"

class ASsGameLevelGameMode;
class ASsPawn;

UCLASS()
class SPHERESHOOTER_API ASsGameLevelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

private:
    TObjectPtr<ASsGameLevelGameMode> SSGameMode;
    TObjectPtr<ASsPawn> SSPawn;
    FVector RollBallSpawnLoc;
};

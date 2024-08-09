#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SSGameLevelGameMode.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    TSubclassOf<class ASSSphere> RollBallClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    FVector PlayerBallLoc;

    // from UE Editor by default (usually 100)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float BallSizeDefault = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float BallSize = 50.f;
};

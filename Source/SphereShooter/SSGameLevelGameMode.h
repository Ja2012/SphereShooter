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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01")
    TSubclassOf<class ASSSphere> RollBallClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01")
    FVector PlayerBallLoc;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01")
    float BallSize = 50.f;
};

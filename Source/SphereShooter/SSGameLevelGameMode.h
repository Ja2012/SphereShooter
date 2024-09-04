#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SSTile.h"
#include "SSColor.h"

#include "SSGameLevelGameMode.generated.h"

class UBallType;

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void BeginPlay() override;

    FVector GetPlayerBallLocation() { return PlayerBallLocation; };
    TObjectPtr<UBallType> GetBallType() { return BallType; }

protected:
    void Init();
    void FindPlayerBallStartPosition();

    FVector PlayerBallLocation;

    TSoftObjectPtr<UBallType> BallTypeSoftPtr;
    TObjectPtr<UBallType> BallType;

    // tag of scene actor that point to player roll ball start position
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball", meta = (DisplayPriority = "1"))
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";

    void LoadBallTypeDataAsset();
    void OnLoadBallTypeDataAsset();

    void SetBallCDO();
    void SetRollBall();
    void SetBallsGrid();

    TArray<FTile> Tiles;
};

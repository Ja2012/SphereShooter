#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tile.h"
#include "SSGameLevelGameMode.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void BeginPlay() override;

    float GetBallSize() const { return BallSize; };
    AActor* GetPlayerBallPositionMarker() { return PlayerBallPositionMarker; };

protected:
    UPROPERTY()
    AActor* PlayerBallPositionMarker;

    // from UE Editor by default (usually 100)
    // TODO: Delete and remake with mesh bounding box
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float BallSizeDefault = 100.f;

    // all game balls size 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float BallSize = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    TSubclassOf<class ASSSphere> BallClass;

    // tag of scene actor that point to player roll ball start position
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";

    void SetRollBall();
    void SetBallsGrid();
    
    TArray<FTile> Tiles;
};

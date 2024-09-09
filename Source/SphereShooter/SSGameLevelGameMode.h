#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SSTile.h"

#include "SSGameLevelGameMode.generated.h"

class ASSGrid;
class UBallType;

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void BeginPlay() override;
    TObjectPtr<UBallType> GetBallType() const { return BallType; }
    FVector FindPlayerBallStartPosition() const;

protected:
    void Init();

    UPROPERTY()
    ASSGrid* Grid;

    TSoftObjectPtr<UBallType> BallTypeSoftPtr;
    TObjectPtr<UBallType> BallType;

    // tag of scene actor that point to player roll ball start position
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball", meta = (DisplayPriority = "-1"))
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";
    
    // Grid with balls will take all space between walls and will have this quantity of rows.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (DisplayPriority = "-1"))
    uint8 BallRowsNum = 10;

    void LoadBallTypeDataAsset();
    void OnLoadBallTypeDataAsset();

    void SetBallCDO() const;
    void SetupRollBall() const;
    void SetBallsGrid();

    UFUNCTION()
    void OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    TObjectPtr<class APlayerController> PlayerController;
    TObjectPtr<class ASSPawn> Pawn;

};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SSGameLevelGameMode.generated.h"

class ASSGrid;
class UBallType;
class ASSPawn;
class ASSGameStateBase;

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    TObjectPtr<UBallType> GetBallType() const { return BallType; }
    FVector FindPlayerBallStartPosition() const;
    float GetGridMoveDistance() const { return GridMoveDistance; }
    uint8 GetNumOfGridRowsWithBalls() const { return NumOfGridRowsWithBalls; }

protected:
    UPROPERTY()
    TObjectPtr<ASSGrid> Grid;

    UPROPERTY()
    TSoftObjectPtr<UBallType> BallTypeSoftPtr;

    UPROPERTY()
    TObjectPtr<UBallType> BallType;

    // tag of scene actor that point to player roll ball start position
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball", meta = (DisplayPriority = "-1"))
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";
    void SetCrossLineActor();

    // CrossLine
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game", meta = (DisplayPriority = "-1"))
    FName CrossLineActorTag = "CrossLine";
    UPROPERTY()
    TObjectPtr<AActor> CrossLine;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (DisplayPriority = "-1"))
    uint8 NumOfGridRowsWithBalls = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Rules", meta = (DisplayPriority = "-1"))
    uint8 MissesLimitNum = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Rules", meta = (DisplayPriority = "-1"))
    float GridMoveDistance = 1.f;

    void Init();
    void InitGrid();
    void LoadBallTypeDataAsset();
    void OnLoadBallTypeDataAsset();
    void SetBallCDO() const;
    void SetupRollBall() const;
    bool IsBallsCrossedLine() const;
    void GameOver();

    UFUNCTION()
    void OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    UPROPERTY()
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<ASSPawn> Pawn;

    UPROPERTY()
    TObjectPtr<ASSGameStateBase> MyGameState;
};
#pragma once

#include "CoreMinimal.h"
#include "SSColor.h"
#include "GameFramework/GameModeBase.h"

#include "SSGameLevelGameMode.generated.h"

struct FTile;
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
    virtual void Tick(float DeltaSeconds) override;
    
    TObjectPtr<UBallType> GetBallType() const { return BallType; }
    FVector FindPlayerBallStartPosition() const;
    float GetGridMoveDistance() const { return GridMoveDistance; }
    uint8 GetNumOfGridRowsWithBalls() const { return NumOfGridRowsWithBalls; }

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY()
    TObjectPtr<ASSGrid> Grid;

    UPROPERTY()
    TSoftObjectPtr<UBallType> BallTypeSoftPtr;

    UPROPERTY()
    TObjectPtr<UBallType> BallType;

    // tag of scene actor that point to player roll ball start position
    UPROPERTY(EditDefaultsOnly, Category = "qq")
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";
    void SetCrossLineActor();

    // CrossLine
    UPROPERTY(EditDefaultsOnly, Category = "qq")
    FName CrossLineActorTag = "CrossLine";

    UPROPERTY()
    TObjectPtr<AActor> CrossLine;

    UPROPERTY(EditAnywhere, Category = "qq")
    uint8 NumOfGridRowsWithBalls = 10;

    UPROPERTY(EditAnywhere, Category = "qq")
    uint8 MissesLimitNum = 5;

    UPROPERTY(EditAnywhere, Category = "qq")
    float GridMoveDistance = 1.f;

    void Init();
    void InitGrid();
    void LoadBallTypeDataAsset();
    void OnLoadBallTypeDataAsset();
    void SetBallCDO() const;
    void SetupRollBall(ESSColor Color = ESSColor::ESSC_NoColor) const;
    bool IsBallsCrossedLine() const;
    void GameOver();

    UFUNCTION()
    void OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    void AddPoints(const uint8 StrikeCount, const uint8 DropCount) const;
    void HandleMisses() const;
    uint8 HandleStrikes(const std::unordered_set<FTile*>& SameColorTiles) const;
    
    UPROPERTY()
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<ASSPawn> Pawn;

    UPROPERTY()
    TObjectPtr<ASSGameStateBase> MyGameState;
};
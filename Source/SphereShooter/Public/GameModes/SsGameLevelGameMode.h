#pragma once

#include "SphereShooter/Public/CoreTypes/SsColor.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SsGameLevelGameMode.generated.h"

class ASsTaggedActor;
class ASsSphere;
struct FGameplayTag;
struct FSsTile;
class ASsGrid;
class USsBallType;
class ASsPawn;
class ASsGameStateBase;
class ASsGameLevelPlayerController;
class ASsPlayerState;
class USsGameInstance;

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void Tick(float DeltaSeconds) override;

    TObjectPtr<USsBallType> GetBallType() const { return BallType; }
    float GetGridMoveDistance() const { return GridMoveDistance; }
    uint8 GetNumOfGridRowsWithBalls() const { return NumOfGridRowsWithBalls; }
    AActor* GetRollBallSpawn()
    {
        if (!RollBallSpawn) SetKeyActors();
        return RollBallSpawn;
    }

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TObjectPtr<ASsGrid> Grid;

    UPROPERTY()
    TObjectPtr<AActor> RollBallSpawn;

    UPROPERTY()
    TObjectPtr<AActor> CrossLine;

    UPROPERTY()
    TSoftObjectPtr<USsBallType> BallTypeSoftPtr;

    UPROPERTY()
    TObjectPtr<USsBallType> BallType;

    // tag of scene actor that point to player roll ball start position
    UPROPERTY(EditDefaultsOnly, Category = "qq")
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";
    void SetKeyActors();

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
    void SetRollBall(ESsColor Color = ESsColor::ESSC_NoColor) const;
    bool IsBallsCrossedLine() const;
    void GameOver();

    UFUNCTION()
    void OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    void AddPoints(const uint8 StrikeCount, const uint8 DropCount) const;
    void HandleMisses() const;
    uint8 HandleStrikes(const std::unordered_set<FSsTile*>& SameColorTiles) const;

    UPROPERTY()
    TObjectPtr<ASsGameLevelPlayerController> PlayerController;

    UPROPERTY()
    TObjectPtr<ASsPawn> Pawn;

    UPROPERTY()
    TObjectPtr<ASsGameStateBase> MyGameState;

    UPROPERTY()
    TObjectPtr<ASsPlayerState> PlayerState;

    UPROPERTY()
    TObjectPtr<USsGameInstance> GameInstance;

    void ExitLevel();
};
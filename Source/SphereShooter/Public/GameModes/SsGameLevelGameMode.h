// SPDX-License-Identifier: MIT

#pragma once

#include "CoreTypes/SsColor.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include <unordered_set>

#include "SsGameLevelGameMode.generated.h"

class ASsGameLevelHUD;
class ASsGameLevelPlayerController;
class ASsGameStateBase;
class ASsGrid;
class ASsPawn;
class ASsPlayerState;
class ASsBaseSphere;
class ASsTaggedActor;
class USsBallType;
class USsGameInstance;
struct FGameplayTag;
struct FSsTile;

UCLASS()
class SPHERESHOOTER_API ASsGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASsGameLevelGameMode();

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

    UPROPERTY(EditAnywhere, Category = "qq")
    uint8 NumOfGridRowsWithBalls = 10;

    UPROPERTY(EditAnywhere, Category = "qq")
    uint8 MissesLimitNum = 5;

    UPROPERTY(EditAnywhere, Category = "qq")
    float GridMoveDistance = 1.f;

    UPROPERTY(EditAnywhere, Category = "qq")
    uint8 BallsMinimumToGetStrike = 3;

    void SetKeyActors();
    void Init();
    void InitGrid();
    void LoadBallTypeDataAsset();
    void OnLoadBallTypeDataAsset();
    void SetBallCDO() const;
    void SetRollBall(ESsColor Color = ESsColor::ESSC_NoColor) const;

    void AddPoints(const uint8 StrikeCount, const uint8 DropCount) const;
    void HandleStrikesDestroy(const std::unordered_set<FSsTile*>& Tiles);
    void HandleDropsDestroy(const std::unordered_set<FSsTile*>& Tiles);
    bool IsBallsCrossedLine() const;
    void HandleMisses() const;

    void GameOver();
    void ExitLevel();

    UFUNCTION()
    void OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    //
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

    //
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

    UPROPERTY()
    TObjectPtr<ASsGameLevelHUD> HUD;
};
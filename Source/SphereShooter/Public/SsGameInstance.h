#pragma once

#include "SphereShooter/Public/CoreTypes/SsBallType.h"
#include "SphereShooter/Public/CoreTypes/SsLevelData.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "SsGameInstance.generated.h"


UCLASS()
class SPHERESHOOTER_API USsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    TSoftObjectPtr<USsBallType> GetCurrentBallType() { return CurrentBallType; }
    void SetCurrentBallTape(const TSoftObjectPtr<USsBallType>& BallTypeSoftPtr) { CurrentBallType = BallTypeSoftPtr; }

    TArray<FSsLevelData>* GetLevels() { return &LevelsData; }

    FSsLevelData* GetStartupLevel() const { return StartupLevel; };
    void SetStartupLevel(FSsLevelData* Data) { StartupLevel = Data; }

    FName GetMainMenuLevelName() const { return MainMenuLevelName; }

private:
    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<USsBallType> CurrentBallType;

    UPROPERTY(EditDefaultsOnly)
    TArray<FSsLevelData> LevelsData;

    FSsLevelData* StartupLevel;

    UPROPERTY(EditDefaultsOnly)
    FName MainMenuLevelName = NAME_None;
};

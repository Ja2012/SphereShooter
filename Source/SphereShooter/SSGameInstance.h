#pragma once

#include "Auxiliary/BallType.h"
#include "Auxiliary/SSLevelData.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "SSGameInstance.generated.h"


UCLASS()
class SPHERESHOOTER_API USSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    TSoftObjectPtr<UBallType> GetCurrentBallType() { return CurrentBallType; }
    void SetCurrentBallTape(const TSoftObjectPtr<UBallType>& BallTypeSoftPtr) { CurrentBallType = BallTypeSoftPtr; }

    TArray<FSSLevelData>* GetLevels() { return &LevelsData; }

    FSSLevelData* GetStartupLevel() const { return StartupLevel; };
    void SetStartupLevel(FSSLevelData* Data) { StartupLevel = Data; }

private:
    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UBallType> CurrentBallType;

    UPROPERTY(EditDefaultsOnly)
    TArray<FSSLevelData> LevelsData;

    FSSLevelData* StartupLevel;
};

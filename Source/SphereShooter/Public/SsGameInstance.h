// SPDX-License-Identifier: MIT

#pragma once

#include "CoreTypes/SsBallType.h"
#include "CoreTypes/SsLevelData.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "SsGameInstance.generated.h"

class USsSaveGame;

UCLASS()
class SPHERESHOOTER_API USsGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    TSoftObjectPtr<USsBallType> GetCurrentBallType() { return CurrentBallType; }
    void SetCurrentBallTape(const TSoftObjectPtr<USsBallType>& BallTypeSoftPtr) { CurrentBallType = BallTypeSoftPtr; }

    TArray<FSsLevelData>* GetLevels() { return &GameLevelsData; }

    FSsLevelData* GetStartupLevel() const { return StartupLevel; };
    void SetStartupLevel(FSsLevelData* Data) { StartupLevel = Data; }

    FName GetMainMenuLevelName() const { return MainMenuLevelName; }

    TObjectPtr<USsSaveGame> GetSaveGameInstance() const { return SaveGameInstance; };
    void SaveGame(USsSaveGame* NewSaveGameInstance);

private:
    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<USsBallType> CurrentBallType;

    UPROPERTY(EditDefaultsOnly)
    TArray<FSsLevelData> GameLevelsData;

    FSsLevelData* StartupLevel;

    UPROPERTY(EditDefaultsOnly)
    FName MainMenuLevelName = NAME_None;

    UPROPERTY()
    TObjectPtr<USsSaveGame> SaveGameInstance;

    FString DefaultPlayerName = TEXT("Unknown Player");
    FString DefaultGameSlotName = TEXT("GameSlotName0");
    int32 DefaultUserIndex{0};
};

// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SsSaveGame.generated.h"

class ASsPlayerState;

USTRUCT()
struct FSsPlayerData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    uint64 MaxScore{0};

    UPROPERTY()
    FDateTime WhenMaxScore;

    UPROPERTY()
    FName LevelName = NAME_None;

    void UpdateFromPlayerState(const ASsPlayerState* PlayerState);
};

UCLASS()
class SPHERESHOOTER_API USsSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    FSsPlayerData* GetLastPlayer() { return &Players[LastPlayerName]; }

    TMap<FString, FSsPlayerData>& GetPlayers() { return Players; }

    FString GetLastPlayerName() const { return LastPlayerName; }
    void SetLastPlayerName(const FString Name) { LastPlayerName = Name; }

    void AddPlayer(const FString PlayerName, const FSsPlayerData& PlayerData);

    void SortPlayersByScore();

private:
    UPROPERTY()
    TMap<FString, FSsPlayerData> Players;

    UPROPERTY()
    FString LastPlayerName;
};

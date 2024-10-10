// LICENSE.md

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
    FString PlayerName;

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
    UPROPERTY()
    TArray<FSsPlayerData> Players;

    UPROPERTY()
    uint64 LastPlayerIndex{0};

    FSsPlayerData* GetLastPlayerData() { return &Players[LastPlayerIndex]; }
};

// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "SsPlayerState.generated.h"

struct FSsPlayerData;

UCLASS()
class SPHERESHOOTER_API ASsPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    void UpdateFromPlayerData(FSsPlayerData* PlayerData);

    uint64 GetMaxScore() const { return MaxScore; };
    void SetMaxScore(const uint64 NewScore) { MaxScore = NewScore; }
    void UpdateMaxScore(const uint64 NewScore)
    {
        if (NewScore > MaxScore)
        {
            MaxScore = NewScore;
            WhenMaxScore = FDateTime::Now();
        }
    }

    FDateTime GetWhenMaxScore() const { return WhenMaxScore; };
    void SetWhenMaxScore(const FDateTime Date) { WhenMaxScore = Date; }

    uint8 GetMissesCount() const { return MissesCount; }
    void SetMissesCount(const uint8 Count) { MissesCount = Count; }
    void AddMissesCount() { ++MissesCount; }
    void ResetMissesCount() { MissesCount = 0; }

private:
    uint64 MaxScore{0};
    FDateTime WhenMaxScore;
    uint8 MissesCount{0};
};

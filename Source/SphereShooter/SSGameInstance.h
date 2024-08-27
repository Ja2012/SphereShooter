#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BallType.h"

#include "SSGameInstance.generated.h"

UCLASS()
class SPHERESHOOTER_API USSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    TSoftObjectPtr<UBallType> GetCurrentBallType() { return CurrentBallType; }
    void SetCurrentBallTape(TSoftObjectPtr<UBallType> BallTypeSoftPtr) { CurrentBallType = BallTypeSoftPtr; }

private:
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UBallType> CurrentBallType;
};

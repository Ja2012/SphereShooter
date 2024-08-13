#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SSGameLevelGameMode.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSGameLevelGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASSGameLevelGameMode();
    virtual void BeginPlay() override;

    UPROPERTY()
    AActor* PlayerBallPositionMarkActor;

    UPROPERTY()
    class UNiagaraComponent* AimBeamNiagaraComponent;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    TSubclassOf<class ASSSphere> RollBallClass;

    // from UE Editor by default (usually 100)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float BallSizeDefault = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float BallSize = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    FName PlayerBallPositionMarkActorTag = "PlayerBallXYLocation";


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|AimBeam")
    class UNiagaraSystem* AimBeamNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|AimBeam")
    FString AimBeamLengthVarName = "BeamLength";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|AimBeam")
    FVector AimBeamLengthVarValue;

    void SpawnAimBeam();
};

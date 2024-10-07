#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "SSPawn.generated.h"

struct FInputActionValue;
class ASSSphere;
class ASSGameLevelGameMode;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
class UNiagaraComponent;
class UCameraComponent;

UCLASS()
class SPHERESHOOTER_API ASSPawn : public APawn
{
    GENERATED_BODY()

public:
    ASSPawn();
    virtual void BeginPlay() override;
    virtual void PawnClientRestart() override;

    void SetRollBall(ASSSphere* Ball) { CurrentRollBoll = Ball; }
    ASSSphere* GetRollBall() const { return CurrentRollBoll; }
    void Roll(const FVector& Impulse);    
    void SetAimBeamRotation(FRotator Rotator);

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void ShootRollBall(const FInputActionValue& Value);
    
    UPROPERTY()
    TObjectPtr<ASSGameLevelGameMode> GameMode;

    UPROPERTY(EditDefaultsOnly, Category = "qq|Input")
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "qq|Input")
    TObjectPtr<UInputAction> ShootAction;

    UPROPERTY(EditDefaultsOnly, Category = "qq|Input")
    float ShootScaleImpulse = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "qq|Input")
    TObjectPtr<UInputAction> MouseMoveAction;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> SceneComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UNiagaraComponent> AimBeamNiagaraComponent;

    UPROPERTY(VisibleAnywhere, Category = "qq|VFX")
    TObjectPtr<UNiagaraSystem> AimBeamNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, Category = "qq|VFX")
    FName AimBeamLengthVarName = "AimBeamLength";

    UPROPERTY(EditDefaultsOnly, Category = "qq|VFX")
    FVector AimBeamLengthVarValue;

    UPROPERTY()
    TObjectPtr<ASSSphere> CurrentRollBoll;

    UPROPERTY()
    TObjectPtr<APlayerController> PlayerController;
};
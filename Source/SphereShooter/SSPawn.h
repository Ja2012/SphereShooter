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

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PawnClientRestart() override;
    
    void SetRollBall(ASSSphere* Ball) { CurrentRollBoll = Ball; }
    ASSSphere* GetRollBall() const { return CurrentRollBoll; }
    void Roll(const FVector& Impulse);

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    ASSGameLevelGameMode* GameMode;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyInput", meta = (DisplayPriority = "-1"))
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyInput", meta = (DisplayPriority = "-1"))
    UInputAction* ShootAction;

    UFUNCTION()
    void ShootRollBall(const FInputActionValue& Value);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyInput", meta = (DisplayPriority = "-1"))
    float ShootScaleImpulse = 2000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyInput", meta = (DisplayPriority = "-1"))
    UInputAction* MouseMoveAction;

    UFUNCTION()
    void MoveAimBeam(const FInputActionValue& Value);

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleAnywhere)
    UNiagaraComponent* AimBeamNiagaraComponent;

    UPROPERTY(VisibleAnywhere)
    UNiagaraSystem* AimBeamNiagaraSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim Beam", meta = (DisplayPriority = "-1"))
    FName AimBeamLengthVarName = "AimBeamLength";

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim Beam", meta = (DisplayPriority = "-1"))
    FVector AimBeamLengthVarValue;

    UPROPERTY()
    TObjectPtr<ASSSphere> CurrentRollBoll;

    UPROPERTY()
    TObjectPtr<APlayerController> PlayerController;
};
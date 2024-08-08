#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SSPawn.generated.h"

class ASSSphere;

UCLASS()
class SPHERESHOOTER_API ASSPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASSPawn();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PawnClientRestart() override;
    void SetRollBall(ASSSphere* Ball);
    ASSSphere* GetRollBall() const { return CurrentRollBoll; }
    void Roll(FVector Direction);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA") 
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
	class UInputAction* ShootAction;

	UFUNCTION()
	void Shoot(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
	float ShootScaleImpulse = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
	class UInputAction* MouseMoveAction;

	UFUNCTION()
	void MoveMouse(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    class USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    class UNiagaraSystem* NiagaraSystemBeam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    class UNiagaraComponent* NiagaraComponent;

private:
    UPROPERTY()
    ASSSphere* CurrentRollBoll;

};

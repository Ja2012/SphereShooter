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

    void Roll(FVector Impulse);

    UPROPERTY()
    class UNiagaraComponent* AimBeamNiagaraComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|Input") 
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|Input")
	class UInputAction* ShootAction;

	UFUNCTION()
	void ShootRollBall(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|Input")
	float ShootScaleImpulse = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|Input")
	class UInputAction* MouseMoveAction;

	UFUNCTION()
	void MoveAimBeam(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA|Components")
    class USceneComponent* SceneComponent;


private:
    UPROPERTY()
    ASSSphere* CurrentRollBoll;

	class ASSPlayerController* PlayerController;
};

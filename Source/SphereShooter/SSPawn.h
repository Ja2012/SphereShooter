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
    void Roll();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01|Input") 
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01|Input")
	class UInputAction* ShootAction;

	UFUNCTION()
	void Shoot(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01|Components")
	class UCameraComponent* CameraComponent;

private:
    UPROPERTY()
    ASSSphere* CurrentRollBoll;

};

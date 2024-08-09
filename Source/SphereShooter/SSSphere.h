#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSSphere.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSSphere : public AActor
{
    GENERATED_BODY()

public:
    ASSSphere();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(FVector Direction);
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    class USphereComponent* SphereCollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    class UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float RollImpulseValue;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSColor.h"

#include "SSSphere.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSSphere : public AActor
{
    GENERATED_BODY()

public:
    ASSSphere();
    virtual void BeginPlay() override;
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(FVector Impulse);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA")
    class USphereComponent* SphereCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA")
    class UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AAA")
    float RollImpulseValue;

    ESSColor Color = ESSColor::ESSC_Green;
};

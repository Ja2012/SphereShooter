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
    void Roll();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class USphereComponent* SphereCollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "01")
    float RollImpulseValue;
};

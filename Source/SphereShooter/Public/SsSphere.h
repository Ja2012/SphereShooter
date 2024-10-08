#pragma once

#include "SphereShooter/Public/CoreTypes/SsColor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SsSphere.generated.h"

struct FSsTile;
class USphereComponent;

UCLASS()
class SPHERESHOOTER_API ASsSphere : public AActor
{
    GENERATED_BODY()

public:
    ASsSphere();
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(const FVector& Impulse) const;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> SphereCollisionComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
    
    ESsColor Color = ESsColor::ESSC_NoColor;
    FSsTile* Tile;
    
protected:
    virtual void BeginPlay() override;
};

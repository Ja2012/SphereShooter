#pragma once

#include "Auxiliary/SSColor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SSSphere.generated.h"

struct FTile;
class USphereComponent;

UCLASS()
class SPHERESHOOTER_API ASSSphere : public AActor
{
    GENERATED_BODY()

public:
    ASSSphere();
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(const FVector& Impulse) const;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> SphereCollisionComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
    
    ESSColor Color = ESSColor::ESSC_NoColor;
    FTile* Tile;
    
protected:
    virtual void BeginPlay() override;
};

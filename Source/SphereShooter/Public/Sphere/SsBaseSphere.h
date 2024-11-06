#pragma once

#include "CoreTypes/SsColor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SsBaseSphere.generated.h"

struct FSsTile;
class USphereComponent;

UCLASS(Abstract)
class SPHERESHOOTER_API ASsBaseSphere : public AActor
{
    GENERATED_BODY()

public:
    ASsBaseSphere();
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(const FVector& Impulse) const;

    virtual float GetMeshRadius() const = 0;
    virtual void SetMeshScale(FVector Scale) = 0;
    virtual void SetMaterial(UMaterialInstance* Material) = 0;
    virtual void Kill() = 0;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> SphereCollisionComponent;

    ESsColor Color = ESsColor::ESSC_NoColor;
    FSsTile* Tile;
    
protected:
    virtual void BeginPlay() override;
};

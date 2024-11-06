// SPDX-License-Identifier: MIT

#pragma once

#include "CoreTypes/SsColor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SsBaseSphere.generated.h"

struct FSsTile;
class USphereComponent;

UCLASS()
class SPHERESHOOTER_API ASsBaseSphere : public AActor
{
    GENERATED_BODY()

public:
    ASsBaseSphere();
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(const FVector& Impulse) const;

    virtual float GetMeshRadius() const;
    virtual void SetMeshScale(FVector Scale);
    virtual void SetMaterial(UMaterialInstance* Material);
    virtual void Kill();

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> SphereCollisionComponent;

    ESsColor Color = ESsColor::ESSC_NoColor;
    FSsTile* Tile;
    
protected:
    virtual void BeginPlay() override;
};

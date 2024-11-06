// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "Sphere/SsBaseSphere.h"

#include "SsDestructibleSphere.generated.h"

UCLASS()
class SPHERESHOOTER_API ASsDestructibleSphere : public ASsBaseSphere
{
    GENERATED_BODY()

public:
    ASsDestructibleSphere();
    virtual float GetMeshRadius() const override;
    virtual void SetMeshScale(FVector Scale) override;
    virtual void SetMaterial(UMaterialInstance* Material) override;
    virtual void Kill() override;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
};

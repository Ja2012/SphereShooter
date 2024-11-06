// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "Sphere/SsBaseSphere.h"

#include "SsSphere.generated.h"


UCLASS()
class SPHERESHOOTER_API ASsSphere : public ASsBaseSphere
{
	GENERATED_BODY()

public:
    ASsSphere();
    virtual float GetMeshRadius() const override;
    virtual void SetMeshScale(FVector Scale) override;
    virtual void SetMaterial(UMaterialInstance* Material) override;
    virtual void Kill() override;

protected:
	UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};

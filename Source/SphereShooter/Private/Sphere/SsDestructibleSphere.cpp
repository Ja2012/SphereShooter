// SPDX-License-Identifier: MIT


#include "Sphere/SsDestructibleSphere.h"

#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ASsDestructibleSphere::ASsDestructibleSphere()
{
    GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollectionComponent");
    GeometryCollectionComponent->SetupAttachment(SphereCollisionComponent);
    GeometryCollectionComponent->SetSimulatePhysics(false);
    GeometryCollectionComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

float ASsDestructibleSphere::GetMeshRadius() const
{
    return GeometryCollectionComponent->CalcBounds(FTransform()).GetBox().GetExtent().X;
}

void ASsDestructibleSphere::SetMeshScale(FVector Scale)
{
    GeometryCollectionComponent->SetWorldScale3D(Scale);
}

void ASsDestructibleSphere::SetMaterial(UMaterialInstance* Material)
{
    GeometryCollectionComponent->SetMaterial(0, Material);
}

void ASsDestructibleSphere::Kill()
{
    GeometryCollectionComponent->SetSimulatePhysics(false);
    GeometryCollectionComponent->Deactivate();
    Destroy();
}

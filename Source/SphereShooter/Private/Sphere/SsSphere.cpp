// SPDX-License-Identifier: MIT


#include "Sphere/SsSphere.h"

#include "Components/SphereComponent.h"

ASsSphere::ASsSphere() 
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(SphereCollisionComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

float ASsSphere::GetMeshRadius() const
{
    return StaticMeshComponent->GetStaticMesh()->GetBounds().GetSphere().W;
}

void ASsSphere::SetMeshScale(FVector Scale)
{
    StaticMeshComponent->SetWorldScale3D(Scale);
}

void ASsSphere::SetMaterial(UMaterialInstance* Material) 
{
    StaticMeshComponent->SetMaterial(0, Material);
}

void ASsSphere::Kill() 
{
    Destroy();
}

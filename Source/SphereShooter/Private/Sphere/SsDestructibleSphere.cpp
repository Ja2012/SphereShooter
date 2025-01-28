// SPDX-License-Identifier: MIT


#include "Sphere/SsDestructibleSphere.h"

#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "TimerManager.h"

ASsDestructibleSphere::ASsDestructibleSphere()
{
    GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollectionComponent");
    GeometryCollectionComponent->SetupAttachment(SphereCollisionComponent);
    GeometryCollectionComponent->SetSimulatePhysics(false);
    GeometryCollectionComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

float ASsDestructibleSphere::GetMeshRadius() const
{
    return GeometryCollectionComponent->GetLocalBounds().GetExtent().X;
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
    // TODO WIP


    //GeometryCollectionComponent->SetSimulatePhysics(true);
    //GeometryCollectionComponent->AddImpulse();
    //GeometryCollectionComponent->AddForce(FVector(0.f, 0.f, 3000.f));

    //FTimerHandle _;
    //GetWorld()->GetTimerManager().SetTimer(_, this, &ASsDestructibleSphere::K2_DestroyActor, 5.f);
    Destroy();
}

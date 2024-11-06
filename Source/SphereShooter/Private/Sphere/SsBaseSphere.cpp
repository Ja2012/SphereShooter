// SPDX-License-Identifier: MIT

#include "Sphere/SsBaseSphere.h"

#include "Components/SphereComponent.h"

ASsBaseSphere::ASsBaseSphere()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
    SetRootComponent(SphereCollisionComponent);
    SphereCollisionComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
    SphereCollisionComponent->SetLinearDamping(0.f);
    SphereCollisionComponent->SetAngularDamping(0.f);

    TurnIntoGridBall();
}

void ASsBaseSphere::Roll(const FVector& Impulse) const
{
    if (!SphereCollisionComponent) return;
    SphereCollisionComponent->AddImpulse(Impulse, NAME_None, true);
}

float ASsBaseSphere::GetMeshRadius() const
{
    return 0.0f;
}

void ASsBaseSphere::SetMeshScale(FVector Scale) {}

void ASsBaseSphere::SetMaterial(UMaterialInstance* Material) {}

void ASsBaseSphere::Kill() {}

void ASsBaseSphere::TurnIntoRollBall() 
{
    SphereCollisionComponent->SetSimulatePhysics(true);
    SphereCollisionComponent->SetNotifyRigidBodyCollision(true);
}

void ASsBaseSphere::TurnIntoGridBall() 
{
    SphereCollisionComponent->SetSimulatePhysics(false);
    SphereCollisionComponent->SetNotifyRigidBodyCollision(false);
    SphereCollisionComponent->OnComponentHit.Clear();
}

void ASsBaseSphere::BeginPlay() 
{
    Super::BeginPlay();

    FBodyInstance* BodyInstance = SphereCollisionComponent->GetBodyInstance();
    BodyInstance->bLockZTranslation = true;
    BodyInstance->SetDOFLock(EDOFMode::SixDOF);
}




#include "SsSphere.h"

#include "Components/SphereComponent.h"

ASsSphere::ASsSphere()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
    SetRootComponent(SphereCollisionComponent);
    SphereCollisionComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(SphereCollisionComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

    TurnIntoGridBall();
}

void ASsSphere::Roll(const FVector& Impulse) const
{
    if (!SphereCollisionComponent) return;
    SphereCollisionComponent->AddImpulse(Impulse, NAME_None, true);
}

void ASsSphere::TurnIntoRollBall() 
{
    PrimaryActorTick.bCanEverTick = true;
    SphereCollisionComponent->SetSimulatePhysics(true);
    SphereCollisionComponent->SetNotifyRigidBodyCollision(true);

    // no friction so we can roll indefinitely 
    SphereCollisionComponent->SetLinearDamping(0.f);
    SphereCollisionComponent->SetAngularDamping(0.f);
}

void ASsSphere::TurnIntoGridBall() 
{
    PrimaryActorTick.bCanEverTick = false;
    SphereCollisionComponent->SetSimulatePhysics(false);
    SphereCollisionComponent->SetNotifyRigidBodyCollision(false);
    SphereCollisionComponent->OnComponentHit.Clear();
}

void ASsSphere::BeginPlay() 
{
    Super::BeginPlay();

    FBodyInstance* BodyInstance = SphereCollisionComponent->GetBodyInstance();
    BodyInstance->bLockZTranslation = true;
    BodyInstance->SetDOFLock(EDOFMode::SixDOF);
}



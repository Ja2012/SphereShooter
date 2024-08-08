#include "SSSphere.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

ASSSphere::ASSSphere()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
    SphereCollisionComponent->SetSimulatePhysics(true);
    SphereCollisionComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
    // no friction so we can roll indefinitely 
    SphereCollisionComponent->SetLinearDamping(0.f);
    SphereCollisionComponent->SetAngularDamping(0.f);
    // gravity allow us to use angular impulse.
    SphereCollisionComponent->SetEnableGravity(true);
    SphereCollisionComponent->SetPhysicsMaxAngularVelocityInDegrees(100000.f);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

    SetRootComponent(SphereCollisionComponent);
    StaticMeshComponent->SetupAttachment(SphereCollisionComponent);

}

void ASSSphere::Roll(FVector Direction)
{
    //SphereCollisionComponent->AddAngularImpulseInDegrees(FVector(0.f, RollImpulseValue, 0.f), NAME_None, true);
    if (!SphereCollisionComponent) return;
    SphereCollisionComponent->AddImpulse(Direction, NAME_None, true);
}

void ASSSphere::BeginPlay() 
{
    Super::BeginPlay();

    // to roll only in XY plane (see BeginPlay).
    FBodyInstance* BodyInstance = SphereCollisionComponent->GetBodyInstance();
    BodyInstance->bLockZTranslation = true;
    BodyInstance->SetDOFLock(EDOFMode::SixDOF);
}

void ASSSphere::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::Printf(TEXT("Roll ball velocity: %s"), *GetVelocity().ToCompactString()));
    
}



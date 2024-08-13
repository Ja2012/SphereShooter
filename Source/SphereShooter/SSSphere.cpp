#include "SSSphere.h"
#include "Components/SphereComponent.h"

ASSSphere::ASSSphere()
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

void ASSSphere::Roll(FVector Impulse)
{
    //SphereCollisionComponent->AddAngularImpulseInDegrees(FVector(0.f, RollImpulseValue, 0.f), NAME_None, true);
    if (!SphereCollisionComponent) return;
    SphereCollisionComponent->AddImpulse(Impulse, NAME_None, true);
}

void ASSSphere::TurnIntoRollBall() 
{
    PrimaryActorTick.bCanEverTick = true;
    SphereCollisionComponent->SetSimulatePhysics(true);

    // no friction so we can roll indefinitely 
    SphereCollisionComponent->SetLinearDamping(0.f);
    SphereCollisionComponent->SetAngularDamping(0.f);
}

void ASSSphere::TurnIntoGridBall() 
{
    PrimaryActorTick.bCanEverTick = false;
    SphereCollisionComponent->SetSimulatePhysics(false);
}

void ASSSphere::BeginPlay() 
{
    Super::BeginPlay();

    FBodyInstance* BodyInstance = SphereCollisionComponent->GetBodyInstance();
    BodyInstance->bLockZTranslation = true;
    BodyInstance->SetDOFLock(EDOFMode::SixDOF);
}

void ASSSphere::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::Printf(TEXT("Roll ball velocity: %s"), *GetVelocity().ToCompactString()), false);
    
}



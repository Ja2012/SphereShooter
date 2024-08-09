#include "SSSphere.h"
#include "Components/SphereComponent.h"

ASSSphere::ASSSphere()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
    SetRootComponent(SphereCollisionComponent);
    SphereCollisionComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);

    // no friction so we can roll indefinitely 
    SphereCollisionComponent->SetLinearDamping(0.f);
    SphereCollisionComponent->SetAngularDamping(0.f);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(SphereCollisionComponent);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

    TurnIntoGridBall();
}

void ASSSphere::Roll(FVector Direction)
{
    //SphereCollisionComponent->AddAngularImpulseInDegrees(FVector(0.f, RollImpulseValue, 0.f), NAME_None, true);
    if (!SphereCollisionComponent) return;
    SphereCollisionComponent->AddImpulse(Direction, NAME_None, true);
}

void ASSSphere::TurnIntoRollBall() 
{
    PrimaryActorTick.bCanEverTick = true;
    SphereCollisionComponent->SetSimulatePhysics(true);
}

void ASSSphere::TurnIntoGridBall() 
{
    PrimaryActorTick.bCanEverTick = false;
    SphereCollisionComponent->SetSimulatePhysics(false);
}

void ASSSphere::BeginPlay() 
{
    Super::BeginPlay();

    // to roll only in XY plane
    FBodyInstance* BodyInstance = SphereCollisionComponent->GetBodyInstance();
    BodyInstance->bLockZTranslation = true;
    BodyInstance->SetDOFLock(EDOFMode::SixDOF);
}

void ASSSphere::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::Printf(TEXT("Roll ball velocity: %s"), *GetVelocity().ToCompactString()));
    
}



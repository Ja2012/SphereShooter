// Sphere Shooter by Evgeny Grigoryev. Check "License.MD" file.


#include "SSSphere.h"

// Sets default values
ASSSphere::ASSSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASSSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASSSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Sphere Shooter by Evgeny Grigoryev. Check "License.MD" file.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSSphere.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

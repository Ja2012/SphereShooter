// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSGrid.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ASSGrid();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

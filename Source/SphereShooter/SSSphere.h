#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSColor.h"

#include "SSSphere.generated.h"

struct FTile;
class USphereComponent;

UCLASS()
class SPHERESHOOTER_API ASSSphere : public AActor
{
    GENERATED_BODY()

public:
    ASSSphere();
    virtual void BeginPlay() override;
    void TurnIntoRollBall();
    void TurnIntoGridBall();
    void Roll(const FVector& Impulse) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayPriority = "-1"))
    USphereComponent* SphereCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayPriority = "-1"))
    UStaticMeshComponent* StaticMeshComponent;
    
    ESSColor Color = ESSColor::ESSC_NoColor;
    FTile* Tile;
};

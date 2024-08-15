// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSGrid.generated.h"

/**
 * Point top regular hexagon
 */
USTRUCT()
struct FTile
{
    GENERATED_USTRUCT_BODY()
    
    FTile(){};
    FTile(uint32 ID, FVector Location) : ID(ID), Location(Location){};

    uint32 ID = 0;
    FVector Location;

    UPROPERTY()
    class ASSSphere* Sphere;
};

/**
 * Should be placed in top left corner of available game field (inside walls)
 * So that when you play, rows go from left -Y to right +Y
 * and columns go from "up" +X to "down" -X
 */
UCLASS()
class SPHERESHOOTER_API ASSGrid : public AActor
{
    GENERATED_BODY()

public:
    ASSGrid();
    void GenerateGrid(TArray<FTile>& Tiles) const;

protected:
    virtual void BeginPlay() override;

    // Grid will take all space between walls and will have this quantity of rows.
    // Ball size variable from GameMode will be used.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA")
    uint8 Rows;
};

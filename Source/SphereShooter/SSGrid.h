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
    FTile(uint32 ID, FVector Location, uint8 Row, uint8 Column, bool bIsOutOfRightEdge = false)
        : ID(ID), Location(Location), Row(Row), Column(Column), bIsOutOfRightEdge(bIsOutOfRightEdge) {}

    uint32 ID = 0;
    uint8 Row;
    uint8 Column;
    FVector Location;

    FTile* L = nullptr;
    FTile* R = nullptr;
    FTile* TL = nullptr;
    FTile* TR = nullptr;
    FTile* BL = nullptr;
    FTile* BR = nullptr;

    UPROPERTY()
    TWeakObjectPtr<class ASSSphere> Ball;

    bool bIsOutOfRightEdge = false;
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

    using FTileMemberPtr = FTile* FTile::*;
    void SetValidNeighbor(TArray<FTile>& Tiles, FTile& Tile, const FTileMemberPtr TileMember, const uint8 ColumnsQuantity) const;
    void IDToRowColumn(const uint32 ID, const uint8 ColumnsQuantity, uint8& Row, uint8& CurrentColumn) const
    {
        Row = ID / ColumnsQuantity;
        CurrentColumn = ID % ColumnsQuantity;
    }
    uint32 RowColumnToID(const uint8 ColumnsQuantity, const uint32 Row, const uint8 CurrentColumn) const
    {
        return Row * ColumnsQuantity + CurrentColumn;
    };
};

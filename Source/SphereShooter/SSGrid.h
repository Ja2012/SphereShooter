// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "SSTile.h"
#include "SSGrid.generated.h"


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
    void GenerateGrid();

    void IDToRowColumn(const uint32 ID, uint8& OutRow, uint8& OutCurrentColumn) const
    {
        OutRow = ID / ColumnsNum;
        OutCurrentColumn = ID % ColumnsNum;
    }

    uint32 RowColumnToID(const uint8 Row, const uint8 CurrentColumn) const
    {
        return Row * ColumnsNum + CurrentColumn;
    };

    TArray<FTile> Tiles{};
    uint8 ColumnsNum = 0;
    uint8 RowsNum = 0;

    FTile* GetLowestTileWithBall()
    {
        const int32 Index = Tiles.FindLastByPredicate([](const FTile& Tile) { return !Tile.Empty(); });
        return &Tiles[Index];
    }

protected:
    using FTileMemberPtr = FTile* FTile::*;
    void SetValidNeighbor(FTile& Tile, const FTileMemberPtr TileMember);
};
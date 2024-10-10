// LICENSE.md

#pragma once

#include "CoreTypes/SsTaggedActor.h"
#include "CoreTypes/SsTile.h"
#include "CoreTypes/SsTags.h"

#include "CoreMinimal.h"

#include "SsGrid.generated.h"


struct FGameplayTag;

/**
 * Should be placed in top left corner of available game field (inside walls)
 * So that when you play, rows go from left -Y to right +Y
 * and columns go from "up" +X to "down" -X
 */
UCLASS()
class SPHERESHOOTER_API ASsGrid : public ASsTaggedActor
{
    GENERATED_BODY()

public:
    ASsGrid();
    void GenerateGrid();
    
    const FSsTile* GetLowestTileWithBall() const
    {
        return &Tiles[ //
            Tiles.FindLastByPredicate([](const FSsTile& Tile) { return !Tile.Empty(); })
            ];
    }

    static bool IsTileWithBallConnectedToTop(const FSsTile* TargetTile);
    static void GetTilesWithBallsNotConnectedToTop(FSsTile* TargetTile, std::unordered_set<FSsTile*>& TilesNotConnectedToGrid);
    static void GetSameColorConnectedTilesWithBalls(FSsTile* TargetTile, std::unordered_set<FSsTile*>& SameColorConnectedTiles);
    static void GetTilesNeighboursCloseToPointSorted(const FVector& PointLoc, const FSsTile* TargetTile, TArray<FSsTile*>& TilesCloseToPoint);
    void MoveDown();
    void SpawnBalls();

protected:
    TArray<FSsTile> Tiles{};
    uint8 ColumnsNum{0};
    uint8 RowsNum{0};
    using FTileMemberPtr = FSsTile* FSsTile::*;
    void SetNeighbor(FSsTile& Tile, const FTileMemberPtr TileMember);
    
    void IDToRowColumn(const uint32 ID, uint8& OutRow, uint8& OutCurrentColumn) const
    {
        OutRow = ID / ColumnsNum;
        OutCurrentColumn = ID % ColumnsNum;
    }

    uint32 RowColumnToID(const uint8 Row, const uint8 CurrentColumn) const
    {
        return Row * ColumnsNum + CurrentColumn;
    };
};

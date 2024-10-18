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
    void MoveDown();
    void SpawnBalls();

    const FSsTile* GetLowestTileWithBall() const
    {
        return &Tiles[  //
            Tiles.FindLastByPredicate([](const FSsTile& Tile) { return !Tile.Empty(); })];
    }

    // Using tiles with balls only
    void GetColorConnectedBalls(FSsTile* TargetTile, std::unordered_set<FSsTile*>& ResultSameColorConnectedTiles) const;

    void GetTilesNeighboursCloseToPointSorted(
        const FVector& PointLoc, const FSsTile* TargetTile, TArray<FSsTile*>& ResultTilesCloseToPointSorted) const;

    // Get tiles with balls not connected to grid, flying balls. Using tiles with balls only.
    void GetOutOfGridBalls(const std::unordered_set<FSsTile*>& StarTiles, std::unordered_set<FSsTile*>& ResultNotAttachedTiles) const;

protected:
    TArray<FSsTile> Tiles{};
    uint8 ColumnsNum{0};
    uint8 RowsNum{0};
    using FTileMemberPtr = FSsTile* FSsTile::*;

    // Using tiles with balls only. Check if ball is connected to any ball on grid top.
    // GhostTiles is tiles with balls that will be destroyed in next moment, so assume there is no ball.
    static bool IsBallConnectedToTop(const FSsTile* TargetTile, const std::unordered_set<FSsTile*>& GhostTiles);

    // Using tiles with balls only. GhostTiles is tiles with balls that will be destroyed in next moment, so assume there is no ball.
    static void GetBallsNotConnectedToTop(
        FSsTile* TargetTile, std::unordered_set<FSsTile*>& TilesNotConnectedToGrid, std::unordered_set<FSsTile*>& GhostTiles);

    void SetNeighbor(FSsTile& Tile, const FTileMemberPtr TileMember);
    void IDToRowColumn(const uint32 ID, uint8& OutRow, uint8& OutCurrentColumn) const
    {
        OutRow = ID / ColumnsNum;
        OutCurrentColumn = ID % ColumnsNum;
    }
    uint32 RowColumnToID(const uint8 Row, const uint8 CurrentColumn) const { return Row * ColumnsNum + CurrentColumn; };
};

// LICENSE.md

#include "SSGrid.h"
#include "SSGameLevelGameMode.h"
#include "BallType.h"

DEFINE_LOG_CATEGORY_STATIC(ASSGridLog, All, All);

// Sets default values
ASSGrid::ASSGrid()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASSGrid::GenerateGrid()
{
    ASSGameLevelGameMode* GM = Cast<ASSGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const float BallSize = GM->GetBallType()->MeshDiameter;
    const FVector PlayerBallLocation = GM->GetPlayerBallLocation();
    const FVector GridStartLoc = GetActorLocation();

    const float TileWidth = BallSize;
    // r is horizontal small radius (Inradius) in point top regular hexagon
    const float r = TileWidth / 2.f;
    // R is vertical big Radius (Circumradius) in point top regular hexagon
    const float R = 2.f * r / FMath::Sqrt(3.f);
    const float TileHeight = 2.f * R;

    const float GridWidth = (-1.f) * GridStartLoc.Y * 2.f;
    const float GridHeight = GridStartLoc.X - PlayerBallLocation.X;
    // find Rows from GridHeight = R + Rows * (R + R / 2.f) + R;
    RowsNum = (2.f * (GridHeight - 1.f * R)) / (3.f * R);

    ColumnsNum = uint8(GridWidth / TileWidth);

    Tiles.Reserve(ColumnsNum * RowsNum);

    const float TileCenterZ = BallSize / 2.f;
    for (uint8 Row = 0; Row < RowsNum; ++Row)
    {
        for (uint8 Column = 0; Column < ColumnsNum; ++Column)
        {
            // rows go from left -Y to right +Y
            const float TileCenterY = Row % 2 ? GridStartLoc.Y + TileWidth + TileWidth * Column : GridStartLoc.Y + r + TileWidth * Column;
            // columns go from "up" +X to "down" -X
            const float TileCenterX = GridStartLoc.X - R - (Row * 3.f * R / 2.f);
            const FVector TileCenter = FVector(TileCenterX, TileCenterY, TileCenterZ);
            Tiles.Emplace(Row * ColumnsNum + Column, TileCenter, Row, Column, //
                // make sure that last column in each odd row will be empty (because it is out of right edge of game field)
                Row % 2 == 1 && Column == ColumnsNum - 1 ? true : false);
            
            // TODO debug
            DrawDebugCrosshairs(GetWorld(), Tiles[RowColumnToID(Row, Column)].Location, FRotator::ZeroRotator, 30, FColor::Red, true, -1,
                0);
        }
    }

    // set tile neighbours
    for (uint8 Row = 0; Row < RowsNum; ++Row)
    {
        for (uint8 Column = 0; Column < ColumnsNum; ++Column)
        {
            FTile& Tile = Tiles[Row * ColumnsNum + Column];
            if (Tile.bIsOutOfRightEdge) continue;
            SetValidNeighbor(Tile, &FTile::TopLeft);
            Tile.Neighbors.Add(Tile.TopLeft);
            SetValidNeighbor(Tile, &FTile::TopRight);
            Tile.Neighbors.Add(Tile.TopRight);
            SetValidNeighbor(Tile, &FTile::Left);
            Tile.Neighbors.Add(Tile.Left);
            SetValidNeighbor(Tile, &FTile::Right);
            Tile.Neighbors.Add(Tile.Right);
            SetValidNeighbor(Tile, &FTile::BottomLeft);
            Tile.Neighbors.Add(Tile.BottomLeft);
            SetValidNeighbor(Tile, &FTile::BottomRight);
            Tile.Neighbors.Add(Tile.BottomRight);
        }
    }
    UE_LOG(ASSGridLog, Display, TEXT("Grid generated. Rows %d, columns %d, %d tiles, %.2f width, %.2f height"), RowsNum, ColumnsNum,
        RowsNum * ColumnsNum,
        GridWidth, GridHeight);
}

void ASSGrid::SetValidNeighbor(FTile& Tile, const FTileMemberPtr TileMemberPtr)
{

    uint32 ID;
    const bool bIsOddRow = Tile.Row % 2 ? true : false;
    if (TileMemberPtr == &FTile::TopLeft)
    {
        ID = RowColumnToID(Tile.Row - 1, bIsOddRow ? Tile.Column : Tile.Column - 1);
    }
    else if (TileMemberPtr == &FTile::TopRight)
    {
        ID = RowColumnToID(Tile.Row - 1, bIsOddRow ? Tile.Column + 1 : Tile.Column);
    }
    else if (TileMemberPtr == &FTile::Left)
    {
        ID = RowColumnToID(Tile.Row, Tile.Column - 1);
    }
    else if (TileMemberPtr == &FTile::Right)
    {
        ID = RowColumnToID(Tile.Row, Tile.Column + 1);
    }
    else if (TileMemberPtr == &FTile::BottomLeft)
    {
        ID = RowColumnToID(Tile.Row + 1, bIsOddRow ? Tile.Column : Tile.Column - 1);
    }
    else if (TileMemberPtr == &FTile::BottomRight)
    {
        ID = RowColumnToID(Tile.Row + 1, bIsOddRow ? Tile.Column + 1 : Tile.Column);
    }
    else checkNoEntry();

    if (!Tiles.IsValidIndex(ID))
    {
        Tile.*TileMemberPtr = nullptr;
        return;
    };
    Tile.*TileMemberPtr = &Tiles[ID];
}
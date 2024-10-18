// LICENSE.md

#include "SsGrid.h"

#include "CoreTypes/SsBallType.h"
#include "GameModes/SsGameLevelGameMode.h"

#include <unordered_map>

DEFINE_LOG_CATEGORY_STATIC(ASSGridLog, All, All);

ASsGrid::ASsGrid()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASsGrid::GenerateGrid()
{
    ASsGameLevelGameMode* GameMode = Cast<ASsGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const float BallSize = GameMode->GetBallType()->MeshDiameter;
    const FVector PlayerBallLocation = GameMode->GetRollBallSpawn()->GetActorLocation();
    const FVector GridStartLoc = GetActorLocation();

    const float TileWidth = BallSize;
    // r is horizontal small radius (Inradius) in point top regular hexagon
    const float r = TileWidth / 2.f;
    // R is vertical big Radius (Circumradius) in point top regular hexagon
    const float R = 2.f * r / FMath::Sqrt(3.f);
    const float TileHeight = 2.f * R;

    const float GridWidth = FMath::Abs(GridStartLoc.Y) * 2.f;
    const float GridHeight = GridStartLoc.X - PlayerBallLocation.X;
    // find Rows from GridHeight = R + Rows * (R + R / 2.f) + R;
    RowsNum = FMath::CeilToInt((2.f * (GridHeight - 2.f * R)) / (3.f * R));
    
    ColumnsNum = GridWidth / TileWidth;

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
            // DrawDebugCrosshairs(GetWorld(), Tiles[RowColumnToID(Row, Column)].Location, FRotator::ZeroRotator, 30, FColor::Red, true, -1,
            //     0);
        }
    }

    // set tile neighbours
    for (uint8 Row = 0; Row < RowsNum; ++Row)
    {
        for (uint8 Column = 0; Column < ColumnsNum; ++Column)
        {
            FSsTile& Tile = Tiles[Row * ColumnsNum + Column];
            if (Tile.bIsOutOfRightEdge) continue;
            SetNeighbor(Tile, &FSsTile::TopLeft);
            SetNeighbor(Tile, &FSsTile::TopRight);
            SetNeighbor(Tile, &FSsTile::Left);
            SetNeighbor(Tile, &FSsTile::Right);
            SetNeighbor(Tile, &FSsTile::BottomLeft);
            SetNeighbor(Tile, &FSsTile::BottomRight);
        }
    }
    UE_LOG(ASSGridLog, Display, TEXT("Grid generated. Rows %d, columns %d, %d tiles, %.2f width, %.2f height"), RowsNum, ColumnsNum,
        RowsNum * ColumnsNum,
        GridWidth, GridHeight);
}

void ASsGrid::SetNeighbor(FSsTile& Tile, const FTileMemberPtr TileMemberPtr)
{

    const bool bIsOddRow = Tile.Row % 2 ? true : false;
    uint8 Row {0}; 
    uint8 Col {0}; 
    if (TileMemberPtr == &FSsTile::TopLeft)
    {
        Row = Tile.Row - 1;      Col = bIsOddRow ? Tile.Column : Tile.Column - 1;
    }
    else if (TileMemberPtr == &FSsTile::TopRight)
    {
        Row = Tile.Row - 1;      Col = bIsOddRow ? Tile.Column + 1 : Tile.Column;
    }
    else if (TileMemberPtr == &FSsTile::Left)
    {
        Row = Tile.Row;          Col = Tile.Column - 1;
    }
    else if (TileMemberPtr == &FSsTile::Right)
    {
        Row = Tile.Row;          Col = Tile.Column + 1;
    }
    else if (TileMemberPtr == &FSsTile::BottomLeft)
    {
        Row = Tile.Row + 1;      Col = bIsOddRow ? Tile.Column : Tile.Column - 1;
    }
    else if (TileMemberPtr == &FSsTile::BottomRight)
    {
        Row = Tile.Row + 1;      Col = bIsOddRow ? Tile.Column + 1 : Tile.Column;
    }
    const uint32 ID = RowColumnToID(Row, Col);
    if (Tiles.IsValidIndex(ID))
    {
        FSsTile* NeighborTile = &Tiles[ID];
        Tile.*TileMemberPtr = NeighborTile;
        Tile.Neighbors.insert(NeighborTile);
    };

}


bool ASsGrid::IsBallConnectedToTop(const FSsTile* TargetTile, const std::unordered_set<FSsTile*>& GhostTiles)
{
    // Use Greedy Best First Search. Grid is weighted, undirected graph. Tile with ball is node in graph.
    // Heuristic made with operator< for Tile row and priority_queue, so that top (up) tiles are in priority.    
    std::priority_queue<const FSsTile*, std::vector<const FSsTile*>, std::greater<const FSsTile*>> Frontier;
    std::unordered_map<const FSsTile*, const FSsTile*> CameFrom;
    Frontier.push(TargetTile);
    CameFrom[TargetTile] = TargetTile;

    while (!Frontier.empty())
    {
        const FSsTile* CurrentTile = Frontier.top();
        Frontier.pop();
        if (CurrentTile->Row == 0)
        {
            return true;
        }

        for (FSsTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty() || CameFrom.contains(Tile) || GhostTiles.contains(Tile)) continue;
            if (Tile->Row == 0) return true;
            CameFrom[Tile] = CurrentTile;
            Frontier.push(Tile);
        }
    }
    return false;
}

void ASsGrid::MoveDown()
{
    const ASsGameLevelGameMode* GameMode = Cast<ASsGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const TObjectPtr<USsBallType> BallType = GameMode->GetBallType();
    
    // todo debug
    // FlushPersistentDebugLines(GetWorld());
    
    const FVector MoveVector {-BallType->MeshDiameter * GameMode->GetGridMoveDistance(), 0, 0};
    SetActorLocation(GetActorLocation() + MoveVector);
    for (FSsTile& Tile: Tiles)
    {
        
        Tile.Location += MoveVector;
        
        // todo debug
    //     DrawDebugCrosshairs(GetWorld(), Tile.Location, FRotator::ZeroRotator, 30, FColor::Red, true, -1,
    // 0);
        
        if (Tile.Empty()) continue;
        Tile.Ball->SetActorLocation(Tile.Location);

    }
}

void ASsGrid::GetColorConnectedBalls(FSsTile* TargetTile, std::unordered_set<FSsTile*>& ResultSameColorConnectedTiles) const
{
    std::vector<FSsTile*> Frontier{TargetTile};
    while (!Frontier.empty())
    {
        FSsTile* CurrentTile = Frontier.back();
        ResultSameColorConnectedTiles.insert(CurrentTile);
        Frontier.pop_back();

        for (FSsTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty() || Tile->Color != TargetTile->Color || ResultSameColorConnectedTiles.contains(Tile)) continue;
            Frontier.push_back(Tile);
        }
    }
}

void ASsGrid::GetTilesNeighboursCloseToPointSorted(const FVector& PointLoc, const FSsTile* TargetTile, TArray<FSsTile*>& ResultTilesCloseToPointSorted) const
{
    Algo::CopyIf(TargetTile->Neighbors, ResultTilesCloseToPointSorted, //
    [](const FSsTile* NeighbourTile) { return NeighbourTile != nullptr; });
    Algo::Sort(ResultTilesCloseToPointSorted, //
        [&PointLoc](const FSsTile* A, const FSsTile* B)
        {
            return (A->Location - PointLoc).Length() < (B->Location - PointLoc).Length();
        });

}

void ASsGrid::GetBallsNotConnectedToTop(
    FSsTile* TargetTile, std::unordered_set<FSsTile*>& TilesNotConnectedToGrid, std::unordered_set<FSsTile*>& GhostTiles)
{
    std::vector<FSsTile*> Frontier{TargetTile};
    while (!Frontier.empty())
    {
        FSsTile* CurrentTile = Frontier.back();
        TilesNotConnectedToGrid.insert(CurrentTile);
        Frontier.pop_back();

        for (FSsTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty()) continue;
            if (TilesNotConnectedToGrid.contains(Tile) || GhostTiles.contains(Tile)) continue;

            GhostTiles.insert(Tile);
            Frontier.push_back(Tile);
        }
    }
}

void ASsGrid::GetOutOfGridBalls(
    const std::unordered_set<FSsTile*>& StarTiles, std::unordered_set<FSsTile*>& ResultNotAttachedTiles) const
{
    // find strike neighbours
    std::unordered_set<FSsTile*> StrikeNeighbours;
    for (FSsTile* StrikeTile : StarTiles)
    {
        for (FSsTile* NeighbourTile : StrikeTile->Neighbors)
        {
            if (!NeighbourTile || NeighbourTile->Empty() || StarTiles.contains(NeighbourTile) ||
                StrikeNeighbours.contains(NeighbourTile))
                continue;
            StrikeNeighbours.insert(NeighbourTile);
        }
    }

    // assume that start balls will be destroyed next moment so except them from next calculations
    std::unordered_set<FSsTile*> TilesToDestroy = StarTiles;

    // look for flying balls, so that we don't overuse Greedy Best First Search Algorithm (IsTileConnectedToGrid)
    std::unordered_set<FSsTile*>::iterator NextIter;
    for (std::unordered_set<FSsTile*>::iterator Iter = StrikeNeighbours.begin(), IteratorEnd = StrikeNeighbours.end(); Iter != IteratorEnd;
         ++Iter)
    {
        if (FSsTile* NeighbourTile = *Iter)
        {
            // We don't need check grid connection for every NeighbourTile, because there is neighbour of neighbour.
            // optimize so again, we don't overuse Greedy Best First Search Algorithm (IsTileConnectedToGrid)
            NextIter = std::next(Iter);
            if (NextIter != IteratorEnd && (*NextIter)->Neighbors.contains(NeighbourTile)) continue;

            if (!IsBallConnectedToTop(NeighbourTile, TilesToDestroy))
            {
                GetBallsNotConnectedToTop(NeighbourTile, ResultNotAttachedTiles, TilesToDestroy);
            }
        }
    }
}

void ASsGrid::SpawnBalls()
{
    const ASsGameLevelGameMode* GameMode = Cast<ASsGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const TObjectPtr<USsBallType> BallType = GameMode->GetBallType();
    
    const uint8 NumOfGridRowsWithBalls = GameMode->GetNumOfGridRowsWithBalls();
    const int64 MaxForRandom = BallType->MaterialInstances.Num() - 1;
    const FVector Scale{1.f};
    for (FSsTile& Tile : Tiles)
    {
        uint8 Row;
        uint8 Col;
        IDToRowColumn(Tile.ID, Row, Col);
        if (Row > NumOfGridRowsWithBalls - 1) break;

        if (Tile.bIsOutOfRightEdge) continue;
        const FTransform SpawnTransform{FRotator::ZeroRotator, Tile.Location, Scale};
        ASsSphere* Ball = GetWorld()->SpawnActorDeferred<ASsSphere>(BallType->SphereClass, SpawnTransform);

        // find random color
        ESsColor Color = ESsColor::ESSC_NoColor;
        while (Color == ESsColor::ESSC_NoColor)
        {
            Color = static_cast<ESsColor>(FMath::RandRange((int64)0, MaxForRandom));
        }

        // set tile
        Tile.Color = Color;
        Tile.Ball = Ball;

        // set random material
        Ball->Color = Color;
        Ball->StaticMeshComponent->SetMaterial(0, BallType->MaterialInstances[Color]);
        Ball->Tile = &Tile;
        Ball->FinishSpawning(SpawnTransform);

    }
    UE_LOG(LogTemp, Display, TEXT("%d balls created for %d rows, %d columns"), //
        NumOfGridRowsWithBalls * ColumnsNum, NumOfGridRowsWithBalls, ColumnsNum);
}

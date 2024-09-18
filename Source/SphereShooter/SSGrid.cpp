// LICENSE.md

#include "SSGrid.h"
#include "SSGameLevelGameMode.h"
#include "BallType.h"

#include <unordered_map>

DEFINE_LOG_CATEGORY_STATIC(ASSGridLog, All, All);

// Sets default values
ASSGrid::ASSGrid()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASSGrid::GenerateGrid()
{
    const ASSGameLevelGameMode* GameMode = Cast<ASSGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const float BallSize = GameMode->GetBallType()->MeshDiameter;
    const FVector PlayerBallLocation = GameMode->FindPlayerBallStartPosition();
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
                Row % 2 == 1 && Column == ColumnsNum - 1 ? true : false,
                Row == 0 ? true : false);
            
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
            SetNeighbor(Tile, &FTile::TopLeft);
            SetNeighbor(Tile, &FTile::TopRight);
            SetNeighbor(Tile, &FTile::Left);
            SetNeighbor(Tile, &FTile::Right);
            SetNeighbor(Tile, &FTile::BottomLeft);
            SetNeighbor(Tile, &FTile::BottomRight);
        }
    }
    UE_LOG(ASSGridLog, Display, TEXT("Grid generated. Rows %d, columns %d, %d tiles, %.2f width, %.2f height"), RowsNum, ColumnsNum,
        RowsNum * ColumnsNum,
        GridWidth, GridHeight);
}

void ASSGrid::SetNeighbor(FTile& Tile, const FTileMemberPtr TileMemberPtr)
{

    const bool bIsOddRow = Tile.Row % 2 ? true : false;
    uint8 Row {0}; 
    uint8 Col {0}; 
    if (TileMemberPtr == &FTile::TopLeft)
    {
        Row = Tile.Row - 1; Col = bIsOddRow ? Tile.Column : Tile.Column - 1;
    }
    else if (TileMemberPtr == &FTile::TopRight)
    {
        Row = Tile.Row - 1; Col = bIsOddRow ? Tile.Column + 1 : Tile.Column;
    }
    else if (TileMemberPtr == &FTile::Left)
    {
        Row = Tile.Row; Col = Tile.Column - 1;
    }
    else if (TileMemberPtr == &FTile::Right)
    {
        Row = Tile.Row; Col = Tile.Column + 1;
    }
    else if (TileMemberPtr == &FTile::BottomLeft)
    {
        Row = Tile.Row + 1; Col = bIsOddRow ? Tile.Column : Tile.Column - 1;
    }
    else if (TileMemberPtr == &FTile::BottomRight)
    {
        Row = Tile.Row + 1; Col = bIsOddRow ? Tile.Column + 1 : Tile.Column;
    }
    const uint32 ID = RowColumnToID(Row, Col);
    if (Tiles.IsValidIndex(ID))
    {
        FTile* NeighborTile = &Tiles[ID];
        Tile.*TileMemberPtr = NeighborTile;
        Tile.Neighbors.insert(NeighborTile);
    };

}

// check if ball is connected to any ball on grid top.
// for tiles with balls
bool ASSGrid::IsTileWithBallConnectedToTop(const FTile* TargetTile)
{
    // Use Greedy Best First Search. Grid is weighted, undirected graph. Tile with ball is node in graph.
    // Heuristic made with operator< for Tile row and priority_queue, so that top (up) tiles are in priority.    
    std::priority_queue<const FTile*, std::vector<const FTile*>, std::greater<const FTile*>> Frontier;
    std::unordered_map<const FTile*, const FTile*> CameFrom;
    Frontier.push(TargetTile);
    CameFrom[TargetTile] = TargetTile;

    while (!Frontier.empty())
    {
        const FTile* CurrentTile = Frontier.top();
        Frontier.pop();
        if (CurrentTile->Row == 0)
        {
            return true;
        }

        for (const FTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty() || CameFrom.contains(Tile)) continue;
            if (Tile->Row == 0) return true;
            CameFrom[Tile] = CurrentTile;
            Frontier.push(Tile);
        }
    }
    return false;
}

void ASSGrid::MoveDown()
{
    const ASSGameLevelGameMode* GameMode = Cast<ASSGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const TObjectPtr<UBallType> BallType = GameMode->GetBallType();
    
    // todo debug
    FlushPersistentDebugLines(GetWorld());
    
    const FVector MoveVector {-BallType->MeshDiameter * GameMode->GetGridMoveDistance(), 0, 0};
    SetActorLocation(GetActorLocation() + MoveVector);
    for (FTile& Tile: Tiles)
    {
        
        Tile.Location += MoveVector;
        
        // todo debug
        DrawDebugCrosshairs(GetWorld(), Tile.Location, FRotator::ZeroRotator, 30, FColor::Red, true, -1,
    0);
        
        if (Tile.Empty()) continue;
        Tile.Ball->SetActorLocation(Tile.Location);

    }
}

void ASSGrid::GetSameColorConnectedTilesWithBalls(FTile* TargetTile, std::unordered_set<FTile*>& SameColorConnectedTiles)
{
    std::vector<FTile*> Frontier{TargetTile};
    while (!Frontier.empty())
    {
        FTile* CurrentTile = Frontier.back();
        SameColorConnectedTiles.insert(CurrentTile);
        Frontier.pop_back();

        for (FTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty() || Tile->Color != TargetTile->Color || SameColorConnectedTiles.contains(Tile)) continue;
            Frontier.push_back(Tile);
        }
    }
}

void ASSGrid::GetTilesWithBallsNotConnectedToTop(FTile* TargetTile, std::unordered_set<FTile*>& TilesNotConnectedToGrid)
{
    std::vector<FTile*> Frontier{TargetTile};
    while (!Frontier.empty())
    {
        FTile* CurrentTile = Frontier.back();
        TilesNotConnectedToGrid.insert(CurrentTile);
        Frontier.pop_back();

        for (FTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty() || TilesNotConnectedToGrid.contains(Tile)) continue;
            Frontier.push_back(Tile);
        }
    }
}

void ASSGrid::SpawnBalls()
{
    const ASSGameLevelGameMode* GameMode = Cast<ASSGameLevelGameMode>(GetWorld()->GetAuthGameMode());
    const TObjectPtr<UBallType> BallType = GameMode->GetBallType();
    
    const uint8 NumOfGridRowsWithBalls = GameMode->GetNumOfGridRowsWithBalls();
    const int64 MaxForRandom = BallType->MaterialInstances.Num() - 1;
    const FVector Scale{1.f};
    for (FTile& Tile : Tiles)
    {
        uint8 Row;
        uint8 Col;
        IDToRowColumn(Tile.ID, Row, Col);
        if (Row > NumOfGridRowsWithBalls - 1) break;

        if (Tile.bIsOutOfRightEdge) continue;
        const FTransform SpawnTransform{FRotator::ZeroRotator, Tile.Location, Scale};
        ASSSphere* Ball = GetWorld()->SpawnActorDeferred<ASSSphere>(BallType->SphereClass, SpawnTransform);

        // find random color
        ESSColor Color = ESSColor::ESSC_NoColor;
        while (Color == ESSColor::ESSC_NoColor)
        {
            Color = static_cast<ESSColor>(FMath::RandRange((int64)0, MaxForRandom));
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

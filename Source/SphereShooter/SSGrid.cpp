// LICENSE.md

#include "SSGrid.h"
#include "SSGameLevelGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(ASSGridLog, All, All);

// Sets default values
ASSGrid::ASSGrid()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASSGrid::GenerateGrid(TArray<FTile>& Tiles) const
{
    const FVector GridStartLoc = GetActorLocation();
    const float BallSize = Cast<ASSGameLevelGameMode>(GetWorld()->GetAuthGameMode())->GetBallSize();

    const float TileWidth = BallSize;
    // r is horizontal small radius (Inradius) in point top regular hexagon
    const float r = TileWidth / 2.f;
    // R is vertical big Radius (Circumradius) in point top regular hexagon
    const float R = 2.f * r / FMath::Sqrt(3.f);
    const float TileHeight = 2.f * R;

    const float GridWidth = (-1.f) * GridStartLoc.Y * 2.f;
    const float GridHeight = R + Rows * (R + R / 2.f) + R;

    const uint8 Columns = uint8(GridWidth / TileWidth);

    Tiles.Reserve(Columns * Rows);

    float TileCenterX = 0.f;
    float TileCenterY = 0.f;
    float TileCenterZ = BallSize / 2.f;
    FVector TileCenter;
    for (uint8 Row = 0; Row < Rows; ++Row)
    {
        for (uint8 Column = 0; Column < Columns; ++Column)
        {
            if (Row % 2 == 1 && Column == Columns - 1) continue;
            // rows go from left -Y to right +Y
            TileCenterY = Row % 2 ? GridStartLoc.Y + TileWidth + TileWidth * Column : GridStartLoc.Y + r + TileWidth * Column;
            // columns go from "up" +X to "down" -X
            TileCenterX = GridStartLoc.X - R - (Row * 3.f * R / 2.f);
            TileCenter = FVector(TileCenterX, TileCenterY, TileCenterZ);
            Tiles.Emplace(FTile(Row * Columns + Column, TileCenter));
        }
    }
    UE_LOG(ASSGridLog, Display, TEXT("Grid %dx%d generated, %d tiles"), Rows, Columns, Rows * Columns);
}

// Called when the game starts or when spawned
void ASSGrid::BeginPlay()
{
    Super::BeginPlay();

    checkf(Rows, TEXT("Zero rows in Grid"));
}
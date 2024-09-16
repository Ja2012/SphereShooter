#include "SSGameLevelGameMode.h"
#include "SSPawn.h"
#include "SSSphere.h"
#include "Kismet/GameplayStatics.h"
#include "SSGrid.h"
#include "SSGameInstance.h"
#include "BallType.h"
#include "Engine/AssetManager.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "SSGameStateBase.h"

#include <unordered_map>
#include <unordered_set>

DEFINE_LOG_CATEGORY_STATIC(ASSGameLevelGameModeLog, All, All);

ASSGameLevelGameMode::ASSGameLevelGameMode()
{
    PrimaryActorTick.bStartWithTickEnabled = true;
    PrimaryActorTick.bCanEverTick = true;
    DefaultPawnClass = ASSPawn::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
    GameStateClass = ASSGameStateBase::StaticClass();
}

void ASSGameLevelGameMode::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>();
    Pawn = Cast<ASSPawn>(PlayerController->GetPawn());
    MyGameState = GetGameState<ASSGameStateBase>();
    LoadBallTypeDataAsset();

}

void ASSGameLevelGameMode::Init()
{
    SetBallCDO();
    SetupRollBall();
    SetBallsGrid();
}

void ASSGameLevelGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const FString DebugMessage { //
        "Points: " + FString::FromInt(MyGameState->PlayerPoints) + "\n" + //
        "Misses: " + FString::FromInt(MyGameState->MissesCount)};  
    GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, DebugMessage, false, FVector2d(2.f,2.f));
}

FVector ASSGameLevelGameMode::FindPlayerBallStartPosition() const
{
    TArray<AActor*> Array;
    UGameplayStatics::GetAllActorsWithTag(this, PlayerBallPositionMarkActorTag, Array);
    checkf(Array.Num() != 0, TEXT("No actor with tag %s"), *PlayerBallPositionMarkActorTag.ToString());
    return Array[0]->GetActorLocation();
}

void ASSGameLevelGameMode::LoadBallTypeDataAsset()
{
    BallTypeSoftPtr = Cast<USSGameInstance>(GetGameInstance())->GetCurrentBallType();
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    StreamableManager.RequestAsyncLoad(
        BallTypeSoftPtr.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ASSGameLevelGameMode::OnLoadBallTypeDataAsset));
}

void ASSGameLevelGameMode::OnLoadBallTypeDataAsset()
{
    BallType = BallTypeSoftPtr.Get();
    check(BallType);
    Init();
}

void ASSGameLevelGameMode::SetupRollBall() const
{
    const FVector PlayerBallLocation = FindPlayerBallStartPosition();
    // spawn
    const FTransform SpawnTransform{
        FRotator::ZeroRotator, FVector(PlayerBallLocation.X, PlayerBallLocation.Y, BallType->MeshDiameter / 2.f), FVector(1.f)};
    ASSSphere* RollBall = GetWorld()->SpawnActorDeferred<ASSSphere>(BallType->SphereClass, SpawnTransform);
    RollBall->TurnIntoRollBall();

    // find random color
    ESSColor Color = ESSColor::ESSC_NoColor;
    while (Color == ESSColor::ESSC_NoColor)
    {
        Color = static_cast<ESSColor>(FMath::RandRange((int64)0, int64(BallType->MaterialInstances.Num() - 1)));
    }

    // set random material
    RollBall->StaticMeshComponent->SetMaterial(0, BallType->MaterialInstances[Color]);
    RollBall->Color = Color;

    // finish
    RollBall->FinishSpawning(SpawnTransform);
    Pawn->SetRollBall(RollBall);
    RollBall->SphereCollisionComponent->OnComponentHit.AddDynamic(this, &ASSGameLevelGameMode::OnRollBallHit);
}

void ASSGameLevelGameMode::SetBallsGrid()
{
    Grid = Cast<ASSGrid>(UGameplayStatics::GetActorOfClass(this, ASSGrid::StaticClass()));
    checkf(Grid, TEXT("No ASSGrid actor on scene"));
    Grid->GenerateGrid();

    const int64 MaxForRandom = BallType->MaterialInstances.Num() - 1;
    const FVector Scale{1.f};
    for (FTile& Tile : Grid->Tiles)
    {
        uint8 Row;
        uint8 Col;
        Grid->IDToRowColumn(Tile.ID, Row, Col);
        if (Row > BallRowsNum - 1) break;

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
    UE_LOG(LogTemp, Display, TEXT("%d balls created for %d rows, %d columns"), BallRowsNum * Grid->ColumnsNum, BallRowsNum,
        Grid->ColumnsNum);
}

void ASSGameLevelGameMode::OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    if (IsValid(OtherActor) && !OtherActor->IsA(ASSSphere::StaticClass())) return;
    ASSSphere* RollBall = Cast<ASSSphere>(HitComponent->GetOwner());

    const ASSSphere* GridBall = Cast<ASSSphere>(OtherActor);
    const double HitNormalYaw = Hit.ImpactNormal.Rotation().Yaw;

    if (GridBall->Tile->Row == Grid->RowsNum - 1 && FMath::Abs(HitNormalYaw) >= 120)
    {
        GameOver();
        return;
    }

    // TODO debug
    UE_LOG(LogTemp, Display, TEXT("HIT!!! with yaw: %.2f"), HitNormalYaw);
    DrawDebugDirectionalArrow(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 100.f, BallType->MeshDiameter,
        FColor::White, false, 5, -1,
        3);

    // find GridBall neighbours and sort them in ascending distance to GridBall
    FVector HitPointLoc = Hit.ImpactPoint;
    TArray<FTile*> GridBallTileNeighbours;
    Algo::CopyIf(GridBall->Tile->Neighbors, GridBallTileNeighbours, [](const FTile* NeighbourTile) { return NeighbourTile != nullptr; });
    Algo::Sort(GridBallTileNeighbours, [&HitPointLoc](const FTile* A, const FTile* B)
    {
        return (A->Location - HitPointLoc).Length() < (B->Location - HitPointLoc).Length();
    });

    // find most close empty tile to land 
    FTile* TileToLand = nullptr;
    for (FTile* NeighbourTile : GridBallTileNeighbours)
    {
        if (NeighbourTile->Empty())
        {
            TileToLand = NeighbourTile;
            break;
        }
    }

    // edge cases
    // left
    if (HitNormalYaw < 0 && GridBall->Tile->Column == 0 && GridBall->Tile->Row % 2 == 0)
    {
        TileToLand = GridBall->Tile->BottomRight;
        UE_LOG(LogTemp, Display, TEXT("left edge shift"));
    }
    // right
    else if (HitNormalYaw > 0 && TileToLand && TileToLand->bIsOutOfRightEdge)
    {
        TileToLand = GridBall->Tile->Row % 2 == 0 ? GridBall->Tile->BottomLeft : GridBall->Tile->BottomRight;
        UE_LOG(LogTemp, Display, TEXT("right edge shift"));
    }

    // out of edge cases
    if (!TileToLand || (TileToLand && !TileToLand->Empty()))
    {
        ensureMsgf(false, TEXT("Roll Ball out of edge!!!"));
        RollBall->Destroy();
        SetupRollBall();
        return;
    }

    checkf(TileToLand->Ball == nullptr, TEXT("Landing tile is not empty"));

    // Landing
    RollBall->TurnIntoGridBall();
    RollBall->SetActorLocation(TileToLand->Location, false, nullptr, ETeleportType::ResetPhysics);
    TileToLand->Set(RollBall);

    uint8 StrikeCount = 0;
    uint8 DropCount = 0;

    // check if hit same color
    std::unordered_set<FTile*> SameColorConnectedTiles{};
    GetSameColorConnectedTiles(TileToLand, SameColorConnectedTiles);

    if (SameColorConnectedTiles.size() >= 3)
    {
        StrikeCount = SameColorConnectedTiles.size();

        // find strike neighbours and then destroy strike 
        std::unordered_set<FTile*> StrikeNeighborTiles{};
        for (FTile* StrikeTile : SameColorConnectedTiles)
        {
            for (FTile* NeighbourTile : StrikeTile->Neighbors)
            {
                if (!NeighbourTile || NeighbourTile->Empty() || SameColorConnectedTiles.contains(NeighbourTile) || StrikeNeighborTiles.contains(
                        NeighbourTile))
                    continue;
                StrikeNeighborTiles.insert(NeighbourTile);
            }
            if (ASSSphere* Ball = StrikeTile->Ball.Get()) Ball->Destroy();
            StrikeTile->Reset();
        }

        // look for flying balls, destroy and then repeat, so that we don't overuse Greedy Best First Search Algorithm (IsTileConnectedToGrid)
        std::unordered_set<FTile*> TilesToDrop{};
        // for (uint16 i = 0; i < StrikeNeighborTiles.size(); ++i)
        for (std::unordered_set<FTile*>::iterator Iter = StrikeNeighborTiles.begin(), IteratorEnd = StrikeNeighborTiles.end();
             Iter != IteratorEnd; ++Iter)
        {
            if (FTile* NeighbourTile = *Iter)
            {

                // We don't need check grid connection for every NeighbourTile, because there is neighbour of neighbour.
                // optimize so again, we don't overuse Greedy Best First Search Algorithm (IsTileConnectedToGrid)
                std::unordered_set<FTile*>::iterator NextIter = std::next(Iter);
                if (NextIter != IteratorEnd && (*NextIter)->Neighbors.contains(NeighbourTile)) continue;

                if (!IsTileConnectedToGrid(NeighbourTile))
                {
                    GetTilesNotConnectedToGrid(NeighbourTile, TilesToDrop);
                }

                for (FTile* TileToDrop : TilesToDrop)
                {
                    if (ASSSphere* Ball = TileToDrop->Ball.Get()) Ball->Destroy();
                    TileToDrop->Reset();
                    DropCount++;
                }
                TilesToDrop.clear();
            }
        }
    }

    
    SIZE_T PointsOld = MyGameState->PlayerPoints;
    // TODO here come code to count win points for strikes / VFX?
    if (StrikeCount > 2)
    {
        MyGameState->PlayerPoints += StrikeCount;
        UE_LOG(LogTemp, Display, TEXT("Strike %d balls | Points: %llu -> %llu"), StrikeCount, PointsOld, MyGameState->PlayerPoints);
    }
    
    // TODO here come code to count win points for drop balls / VFX?
    if (DropCount)
    {
        MyGameState->PlayerPoints += DropCount;
        UE_LOG(LogTemp, Display, TEXT("Drop %d balls | Points: %llu -> %llu"), DropCount, PointsOld, MyGameState->PlayerPoints);
    }

    if (!DropCount && !StrikeCount)
    {
        ++(MyGameState->MissesCount);
        UE_LOG(LogTemp, Display, TEXT("Miss %d -> %d our of %d"), MyGameState->MissesCount -1 , MyGameState->MissesCount, MissesLimitNum);

        if (MyGameState->MissesCount >= MissesLimitNum)
        {
            MovePlayerCloserToGrid();
            // check for gameover
            if (CheckIfGridBallCrossRollBallY())
            {
                GameOver();
                return;
            };
            MyGameState->MissesCount = 0;
        }
    }
    SetupRollBall();
}

// for tiles with balls
void ASSGameLevelGameMode::GetTilesNotConnectedToGrid(FTile* TargetTile, std::unordered_set<FTile*>& TilesNotConnectedToGrid)
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

// for tiles with balls
void ASSGameLevelGameMode::GetSameColorConnectedTiles(FTile* TargetTile, std::unordered_set<FTile*>& SameColorConnectedTiles)
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

void ASSGameLevelGameMode::MovePlayerCloserToGrid()
{
    // move pawn with his camera
    const FVector MoveDistance{BallType->MeshDiameter, 0, 0};
    Pawn->SetActorLocation(Pawn->GetActorLocation() + MoveDistance);

    // move roll ball spawn loc
    TArray<AActor*> Array;
    UGameplayStatics::GetAllActorsWithTag(this, PlayerBallPositionMarkActorTag, Array);
    Array[0]->SetActorLocation(Array[0]->GetActorLocation() + MoveDistance);
}

// check if pile of grid balls hangs too low and player hit it from below - end game
bool ASSGameLevelGameMode::CheckIfGridBallCrossRollBallY()
{
    const FTile* Tile = Grid->GetLowestTileWithBall();
    UE_LOG(LogTemp, Display, TEXT("Lowest ball is: %s, row %d, column %d"), *UEnum::GetValueAsString(Tile->Color), Tile->Row, Tile->Column);
    return (Tile->Location.X - BallType->MeshDiameter / 2.f) <= (FindPlayerBallStartPosition().X + BallType->MeshDiameter / 2.f);
}

void ASSGameLevelGameMode::GameOver()
{
    // TODO make game over
    UE_LOG(ASSGameLevelGameModeLog, Display, TEXT("Game Over"));
}

void ASSGameLevelGameMode::SetBallCDO() const
{
    ASSSphere* BallCDO = Cast<ASSSphere>(BallType->SphereClass->GetDefaultObject());

    BallCDO->StaticMeshComponent->SetStaticMesh(BallType->Mesh);
    BallCDO->SphereCollisionComponent->SetSphereRadius(BallType->CollisionDiameter / 2.f);

    const float MeshScale = (BallType->MeshDiameter / 2.f) / BallType->Mesh->GetBounds().GetSphere().W;
    BallCDO->StaticMeshComponent->SetWorldScale3D(FVector(MeshScale));
}

// check if ball is connected to any ball on grid top.
// for tiles with balls
bool ASSGameLevelGameMode::IsTileConnectedToGrid(const FTile* TargetTile) const
{
    // Use Greedy Best First Search. Grid is weighted, undirected graph. Tile with ball is node in graph.
    // Heuristic made with operator< for Tile row and priority_queue, so that top (up) tiles are in priority.    
    std::priority_queue<const FTile*, std::vector<const FTile*>, std::greater<const FTile*>> Frontier;
    std::unordered_map<const FTile*, const FTile*> CameFrom;
    Frontier.push(TargetTile);
    CameFrom[TargetTile] = TargetTile;

    SIZE_T Counter = 0;
    bool bIsConnected = false;

    while (!Frontier.empty())
    {
        const FTile* CurrentTile = Frontier.top();
        Frontier.pop();
        if (CurrentTile->Row == 0)
        {
            bIsConnected = true;
            break;
        }

        for (const FTile* Tile : CurrentTile->Neighbors)
        {
            if (!Tile || Tile->Empty() || CameFrom.contains(Tile)) continue;
            CameFrom[Tile] = CurrentTile;
            Frontier.push(Tile);
        }
        Counter++;
    }
    UE_LOG(LogTemp, Display, TEXT("IsTileConnectedToGrid: is connected %d, iterations %llu"), bIsConnected, Counter);
    return bIsConnected;
}
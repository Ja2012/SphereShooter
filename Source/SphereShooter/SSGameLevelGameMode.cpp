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

DEFINE_LOG_CATEGORY_STATIC(ASSGameLevelGameModeLog, All, All);

ASSGameLevelGameMode::ASSGameLevelGameMode()
{
    DefaultPawnClass = ASSPawn::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
}

void ASSGameLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>();
    Pawn = Cast<ASSPawn>(PlayerController->GetPawn());

    FindPlayerBallStartPosition();
    LoadBallTypeDataAsset();

}

void ASSGameLevelGameMode::Init()
{
    SetBallCDO();
    SetupRollBall();
    SetBallsGrid();
}

void ASSGameLevelGameMode::FindPlayerBallStartPosition()
{
    TArray<AActor*> Array;
    UGameplayStatics::GetAllActorsWithTag(this, PlayerBallPositionMarkActorTag, Array);
    checkf(Array.Num() != 0, TEXT("No actor with tag %s"), *PlayerBallPositionMarkActorTag.ToString());
    PlayerBallLocation = Array[0]->GetActorLocation();
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
}

void ASSGameLevelGameMode::OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    if (IsValid(OtherActor) && !OtherActor->IsA(ASSSphere::StaticClass())) return;
    ASSSphere* RollBall = Cast<ASSSphere>(HitComponent->GetOwner());
    RollBall->TurnIntoGridBall();
    const ASSSphere* GridBall = Cast<ASSSphere>(OtherActor);

    // const double YawAngle = (Hit.ImpactPoint - GridBall->GetActorLocation()).Rotation().Yaw;
    const double YawAngle = Hit.ImpactNormal.Rotation().Yaw;

    // check if pile of grid balls hangs too low - end game
    if (GridBall->Tile->Row == Grid->RowsNum - 1 && FMath::Abs(YawAngle) >= 120)
    {        
        // TODO make game over
        UE_LOG(ASSGameLevelGameModeLog, Display, TEXT("Game Over"));
        RollBall->Destroy();
        return;
    }
    
    
    // TODO debug
    UE_LOG(LogTemp, Display, TEXT("YawAngle %.2f"), YawAngle);
    DrawDebugDirectionalArrow(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 100.f, BallType->MeshDiameter,
        FColor::White, false, 5, -1,
        3);

    FVector RollBallLoc = Hit.ImpactPoint;
    TArray<FTile*> GridBallTileNeighbours;
    Algo::CopyIf(GridBall->Tile->Neighbors, GridBallTileNeighbours, [](const FTile* NeighbourTile) { return NeighbourTile != nullptr; });
    Algo::Sort(GridBallTileNeighbours, [&RollBallLoc](const FTile* A, const FTile* B)
    {
        return (A->Location - RollBallLoc).Length() > (B->Location - RollBallLoc).Length();
    });

    // tile where roll ball should land
    FTile* Tile = nullptr;  
    for (FTile* NeighbourTile : GridBallTileNeighbours)
    {
        if (NeighbourTile->Ball == nullptr) { Tile = NeighbourTile; }
    }
    checkf(Tile, TEXT("All tiles around GridBall with color %s are busy"), *UEnum::GetValueAsString(GridBall->Color));
    
    // check bound values - we don't put roll ball outside right edge - no can do
    if (YawAngle > 0 && Tile && Tile->bIsOutOfRightEdge)
    {
        Tile = GridBall->Tile->Row % 2 == 0 ? GridBall->Tile->BottomLeft : GridBall->Tile->BottomRight;
        UE_LOG(LogTemp, Display, TEXT("right edge shift"));
    }
    // check bound values - do not put roll ball outside left edge - no can do
    else if (YawAngle < 0 && GridBall->Tile->Column == 0 && GridBall->Tile->Row % 2 == 0)
    {
        Tile = GridBall->Tile->BottomRight;
        UE_LOG(LogTemp, Display, TEXT("left edge shift"));

    }
    checkf(Tile->Ball == nullptr, TEXT("Landing tile is not empty"));

    RollBall->SetActorLocation(Tile->Location, false, nullptr, ETeleportType::ResetPhysics);
    Tile->Set(RollBall);

    // TODO debug
    // UE_LOG(ASSGameLevelGameModeLog, Display, TEXT("GridBall row %d, col %d, loc %s"), GridBall->Tile->Row, GridBall->Tile->Column,
    //     *GridBall->Tile->Location.ToString());
    // UE_LOG(ASSGameLevelGameModeLog, Display, TEXT("Tile to connect row %d, col %d, loc %s"), Tile->Row, Tile->Column,
    //     *Tile->Location.ToString());
    // UE_LOG(ASSGameLevelGameModeLog, Display, TEXT("RollBall row %d, col %d, loc %s\n"), RollBall->Tile->Row, RollBall->Tile->Column,
    //     *RollBall->Tile->Location.ToString());

    bool bIsSameColor = false;
    for (FTile* NeighborTile : Tile->Neighbors)
    {
        if (!NeighborTile || NeighborTile->Color != RollBall->Color) continue;
        NeighborTile->Ball->Destroy();
        NeighborTile->Reset();
        bIsSameColor = true;
    }
    if (bIsSameColor)
    {
        RollBall->Destroy();
        Tile->Reset();
    }
    SetupRollBall();
}

void ASSGameLevelGameMode::SetBallCDO() const
{
    ASSSphere* BallCDO = Cast<ASSSphere>(BallType->SphereClass->GetDefaultObject());

    BallCDO->StaticMeshComponent->SetStaticMesh(BallType->Mesh);
    BallCDO->SphereCollisionComponent->SetSphereRadius(BallType->CollisionDiameter / 2.f);

    const float MeshScale = (BallType->MeshDiameter / 2.f) / BallType->Mesh->GetBounds().GetSphere().W;
    BallCDO->StaticMeshComponent->SetWorldScale3D(FVector(MeshScale));
}
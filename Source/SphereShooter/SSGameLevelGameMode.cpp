#include "SSGameLevelGameMode.h"
#include "SSPawn.h"
#include "SSSphere.h"
#include "Kismet/GameplayStatics.h"
#include "SSGrid.h"
#include "SSGameInstance.h"
#include "BallType.h"
#include "Engine/AssetManager.h"
#include "Components/SphereComponent.h"

ASSGameLevelGameMode::ASSGameLevelGameMode()
{

    DefaultPawnClass = ASSPawn::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
}

void ASSGameLevelGameMode::BeginPlay()
{
    Super::BeginPlay();
    FindPlayerBallStartPosition();
    LoadBallTypeDataAsset();
}

void ASSGameLevelGameMode::Init() 
{
    SetBallCDO();
    SetRollBall();
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
    check(BallType = BallTypeSoftPtr.Get());  
    Init();
}

void ASSGameLevelGameMode::SetRollBall()
{
    const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>();
    ASSPawn* Pawn = Cast<ASSPawn>(PlayerController->GetPawn());

    // spawn
    const FTransform SpawnTransform{
        FRotator::ZeroRotator, FVector(PlayerBallLocation.X, PlayerBallLocation.Y, BallType->MeshDiameter / 2.f), FVector(1.f)};
    ASSSphere* RollBall = GetWorld()->SpawnActorDeferred<ASSSphere>(BallType->SphereClass, SpawnTransform);
    RollBall->TurnIntoRollBall();
    RollBall->FinishSpawning(SpawnTransform);
    Pawn->SetRollBall(RollBall);
}

void ASSGameLevelGameMode::SetBallsGrid()
{
    const ASSGrid* Grid = Cast<ASSGrid>(UGameplayStatics::GetActorOfClass(this, ASSGrid::StaticClass()));
    checkf(Grid, TEXT("No ASSGrid actor on scene"));
    Grid->GenerateGrid(Tiles);

    const FVector Scale{1.f};
    for (FTile& Tile : Tiles)
    {
        if (Tile.bIsOutOfRightEdge) continue;
        const FTransform SpawnTransform{FRotator::ZeroRotator, Tile.Location, Scale};
        ASSSphere* Ball = GetWorld()->SpawnActorDeferred<ASSSphere>(BallType->SphereClass, SpawnTransform);
        Ball->FinishSpawning(SpawnTransform);
        Tile.Ball = Ball;
    }
}

void ASSGameLevelGameMode::SetBallCDO()
{
    ASSSphere* BallCDO = Cast<ASSSphere>(BallType->SphereClass->GetDefaultObject());

    BallCDO->StaticMeshComponent->SetStaticMesh(BallType->Mesh);
    BallCDO->StaticMeshComponent->SetMaterial(0, BallType->Material);

    BallCDO->SphereCollisionComponent->SetSphereRadius(BallType->CollisionDiameter / 2.f);

    float MeshScale = (BallType->MeshDiameter / 2.f) / BallType->Mesh->GetBounds().GetSphere().W;
    BallCDO->StaticMeshComponent->SetWorldScale3D(FVector(MeshScale));
}
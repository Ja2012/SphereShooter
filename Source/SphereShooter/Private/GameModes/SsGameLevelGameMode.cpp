#include "GameModes/SsGameLevelGameMode.h"

#include "CoreTypes/SsBallType.h"
#include "Player/SsPawn.h"
#include "SsSphere.h"
#include "SsGrid.h"
#include "SsGameInstance.h"
#include "SsGameStateBase.h"
#include "Player/SsGameLevelPlayerController.h"
#include "UI/GameLevel/SsGameLevelHUD.h"
#include "Player/SsPlayerState.h"
#include "CoreTypes/SsSaveGame.h"
#include "UI/GameLevel/SsGameLevelWidget.h"
#include "UI/GameLevel/SsMatchInfoWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "NativeGameplayTags.h"

#include <unordered_set>


DEFINE_LOG_CATEGORY_STATIC(ASSGameLevelGameModeLog, All, All);

ASsGameLevelGameMode::ASsGameLevelGameMode()
{
    DefaultPawnClass = ASsPawn::StaticClass();
    PlayerControllerClass = ASsGameLevelPlayerController::StaticClass();
    GameStateClass = ASsGameStateBase::StaticClass();
    HUDClass = ASsGameLevelHUD::StaticClass();
    PlayerStateClass = ASsPlayerState::StaticClass();
}

void ASsGameLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    SetKeyActors();

    GameInstance = GetGameInstance<USsGameInstance>();
    MyGameState = GetGameState<ASsGameStateBase>();
    PlayerController = GetWorld()->GetFirstPlayerController<ASsGameLevelPlayerController>();
    PlayerState = PlayerController->GetPlayerState<ASsPlayerState>();
    Pawn = Cast<ASsPawn>(PlayerController->GetPawn());
    HUD = PlayerController->GetHUD<ASsGameLevelHUD>();

    LoadBallTypeDataAsset();
}

void ASsGameLevelGameMode::Init()
{
    SetBallCDO();
    SetRollBall();
    InitGrid();

    PlayerState->UpdateFromPlayerData(GameInstance->GetSaveGameInstance()->GetLastPlayer());
    HUD->GetWidget()->OnExitClicked.AddUObject(this, &ASsGameLevelGameMode::ExitLevel);
    HUD->UpdateMatchInfo(PlayerState);
}

void ASsGameLevelGameMode::SetKeyActors()
{
    TArray<AActor*> TaggedActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASsTaggedActor::StaticClass(), TaggedActors);
    
    for (AActor* Actor : TaggedActors)
    {
        const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor);
        if (Interface->HasMatchingGameplayTag(Tag_Grid)) Grid = Cast<ASsGrid>(Actor);
        else if (Interface->HasMatchingGameplayTag(Tag_RollBallSpawn)) RollBallSpawn = Actor;
        else if (Interface->HasMatchingGameplayTag(Tag_CrossLine)) CrossLine = Actor;
    }
}

void ASsGameLevelGameMode::LoadBallTypeDataAsset()
{
    BallTypeSoftPtr = GameInstance->GetCurrentBallType();
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    StreamableManager.RequestAsyncLoad(
        BallTypeSoftPtr.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ASsGameLevelGameMode::OnLoadBallTypeDataAsset));
}

void ASsGameLevelGameMode::OnLoadBallTypeDataAsset()
{
    BallType = BallTypeSoftPtr.Get();
    check(BallType);
    Init();
}

void ASsGameLevelGameMode::SetRollBall(ESsColor Color) const
{
    const FVector PlayerBallLocation = RollBallSpawn->GetActorLocation();
    // spawn
    const FTransform SpawnTransform{
        FRotator::ZeroRotator, FVector(PlayerBallLocation.X, PlayerBallLocation.Y, BallType->MeshDiameter / 2.f), FVector(1.f)};
    ASsSphere* RollBall = GetWorld()->SpawnActorDeferred<ASsSphere>(BallType->SphereClass, SpawnTransform);
    RollBall->TurnIntoRollBall();

    if (Color == ESsColor::ESSC_NoColor)
    {
        // find random color
        while (Color == ESsColor::ESSC_NoColor)
        {
            Color = static_cast<ESsColor>(FMath::RandRange((int64)0, int64(BallType->MaterialInstances.Num() - 1)));
        }        
    }

    // set random material
    RollBall->StaticMeshComponent->SetMaterial(0, BallType->MaterialInstances[Color]);
    RollBall->Color = Color;

    // finish
    RollBall->FinishSpawning(SpawnTransform);
    if (Pawn) Pawn->SetRollBall(RollBall);
    RollBall->SphereCollisionComponent->OnComponentHit.AddDynamic(this, &ASsGameLevelGameMode::OnRollBallHit);
}

void ASsGameLevelGameMode::InitGrid()
{
    Grid = Cast<ASsGrid>(UGameplayStatics::GetActorOfClass(this, ASsGrid::StaticClass()));
    checkf(Grid, TEXT("No ASSGrid actor on scene"));
    Grid->GenerateGrid();
    Grid->SpawnBalls();
}


void ASsGameLevelGameMode::OnRollBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    if (IsValid(OtherActor) && !OtherActor->IsA(ASsSphere::StaticClass())) return;
    ASsSphere* RollBall = Cast<ASsSphere>(HitComponent->GetOwner());

    const ASsSphere* GridBall = Cast<ASsSphere>(OtherActor);
    const double HitNormalYaw = Hit.ImpactNormal.Rotation().Yaw;

    // TODO debug
    UE_LOG(LogTemp, Display, TEXT("HIT!!! with yaw: %.2f"), HitNormalYaw);
    // DrawDebugDirectionalArrow(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 100.f, BallType->MeshDiameter,
    //     FColor::White, false, 5, -1,
    //     3);

    // find GridBall neighbours and sort them in ascending distance to GridBall
    FVector HitPointLoc = Hit.ImpactPoint;
    TArray<FSsTile*> GridBallTileNeighbours;
    Grid->GetTilesNeighboursCloseToPointSorted(Hit.ImpactPoint, GridBall->Tile, GridBallTileNeighbours);

    // find most close empty tile to land 
    FSsTile* TileToLand = nullptr;
    for (FSsTile* NeighbourTile : GridBallTileNeighbours)
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
        SetRollBall(RollBall->Color);
        RollBall->Destroy();
        return;
    }

    checkf(TileToLand->Ball == nullptr, TEXT("Landing tile is not empty"));

    // Landing
    RollBall->TurnIntoGridBall();
    RollBall->SetActorLocation(TileToLand->Location, false, nullptr, ETeleportType::ResetPhysics);
    TileToLand->Set(RollBall);

    if (IsBallsCrossedLine())
    {
        GameOver();
        return;
    }

    // check hits
    std::unordered_set<FSsTile*> SameColorBalls;
    Grid->GetColorConnectedBalls(TileToLand, SameColorBalls);

    std::unordered_set<FSsTile*> NotAttachedBalls;
    if (SameColorBalls.size() >= BallsMinimumToGetStrike)
    {
        Grid->GetOutOfGridBalls(SameColorBalls, NotAttachedBalls);
        HandleStrikesDestroy(SameColorBalls);
        HandleDropsDestroy(NotAttachedBalls);
        AddPoints(SameColorBalls.size(), NotAttachedBalls.size());
    }
    else
    {
        HandleMisses();
    }
    HUD->UpdateMatchInfo(PlayerState);

    if (IsBallsCrossedLine())
    {
        GameOver();
        return;
    }    
    SetRollBall();
}

void ASsGameLevelGameMode::AddPoints(const uint8 StrikeCount, const uint8 DropCount) const
{
    const uint64 PointsOld = PlayerState->GetScore();

    PlayerState->SetScore(PointsOld + StrikeCount);
    UE_LOG(LogTemp, Display, TEXT("Strike %d balls | Points: %d -> %d"), StrikeCount, PointsOld, PlayerState->GetScore());    

    if (DropCount)
    {
        PlayerState->SetScore(PointsOld + DropCount);
        UE_LOG(LogTemp, Display, TEXT("Drop %d balls | Points: %d -> %d"), DropCount, PointsOld, PlayerState->GetScore());
    }

    PlayerState->UpdateMaxScore(PlayerState->GetScore());
}

void ASsGameLevelGameMode::HandleStrikesDestroy(const std::unordered_set<FSsTile*>& Tiles)
{
    for (FSsTile* TileToBoom : Tiles)
    {
        if (ASsSphere* Ball = TileToBoom->Ball.Get()) Ball->Destroy();
        TileToBoom->Reset();
    }
}

void ASsGameLevelGameMode::HandleDropsDestroy(const std::unordered_set<FSsTile*>& Tiles)
{
    for (FSsTile* TileToDrop : Tiles)
    {
        if (ASsSphere* Ball = TileToDrop->Ball.Get()) Ball->Destroy();
        TileToDrop->Reset();
    }
}

void ASsGameLevelGameMode::HandleMisses() const
{    
    PlayerState->AddMissesCount();
    UE_LOG(LogTemp, Display, TEXT("Miss %d -> %d out of %d"), //
        PlayerState->GetMissesCount() - 1, PlayerState->GetMissesCount(), MissesLimitNum);

    if (PlayerState->GetMissesCount() >= MissesLimitNum)
    {
        Grid->MoveDown();
        PlayerState->ResetMissesCount();
    }   
}

void ASsGameLevelGameMode::ExitLevel() 
{
    GameInstance->GetSaveGameInstance()->GetLastPlayer()->UpdateFromPlayerState(PlayerState);
    GameInstance->SaveGame(GameInstance->GetSaveGameInstance());
    UGameplayStatics::OpenLevel(this, GameInstance->GetMainMenuLevelName());
}

bool ASsGameLevelGameMode::IsBallsCrossedLine() const
{
    return (Grid->GetLowestTileWithBall()->Location.X - (BallType->MeshDiameter / 2.f)) <= CrossLine->GetActorLocation().X;
}

void ASsGameLevelGameMode::GameOver()
{
    // TODO make game over
    UE_LOG(ASSGameLevelGameModeLog, Display, TEXT("Game Over"));
    PlayerController->GetHUD<ASsGameLevelHUD>()->ShowGameOverDialog();
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ASsGameLevelGameMode::SetBallCDO() const
{
    const ASsSphere* BallCDO = Cast<ASsSphere>(BallType->SphereClass->GetDefaultObject());

    BallCDO->SphereCollisionComponent->SetSphereRadius(BallType->CollisionDiameter / 2.f);
    const float MeshScale = (BallType->MeshDiameter / 2.f) / BallCDO->StaticMeshComponent->GetStaticMesh()->GetBounds().GetSphere().W;
    BallCDO->StaticMeshComponent->SetWorldScale3D(FVector(MeshScale));
}
#include "SSGameLevelGameMode.h"
#include "SSPawn.h"
#include "SSPlayerController.h"
#include "SSSphere.h"
#include "Kismet/GameplayStatics.h"

ASSGameLevelGameMode::ASSGameLevelGameMode() 
{

    DefaultPawnClass =  ASSPawn::StaticClass();
	PlayerControllerClass = ASSPlayerController::StaticClass();
}

void ASSGameLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    SetRollBall();

    SetBallsGrid();
}

void ASSGameLevelGameMode::SetRollBall() 
{
    const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>();
    ASSPawn* Pawn = Cast<ASSPawn>(PlayerController->GetPawn());
    
    // find roll ball start position
    TArray<AActor*> Array;
    UGameplayStatics::GetAllActorsWithTag(this, PlayerBallPositionMarkActorTag, Array);
    checkf(Array.Num() != 0, TEXT("No actor with tag %s"), *PlayerBallPositionMarkActorTag.ToString());
    PlayerBallPositionMarker = Array[0];
    const FVector PlayerBallXYLoc = PlayerBallPositionMarker->GetActorLocation();

    // spawn
    const float BallScale = BallSize / BallSizeDefault;
    const FTransform SpawnTransform{FRotator::ZeroRotator, FVector(PlayerBallXYLoc.X, PlayerBallXYLoc.Y, BallSize / 2.f),  //
        FVector(BallScale, BallScale, BallScale)};
    ASSSphere* RollBall = GetWorld()->SpawnActorDeferred<ASSSphere>(BallClass, SpawnTransform);

    RollBall->TurnIntoRollBall();
    RollBall->FinishSpawning(SpawnTransform);
    Pawn->SetRollBall(RollBall);
}

void ASSGameLevelGameMode::SetBallsGrid() 
{
    const ASSGrid* Grid = Cast<ASSGrid>(UGameplayStatics::GetActorOfClass(this, ASSGrid::StaticClass()));
    checkf(Grid, TEXT("No ASSGrid actor on scene"));

    Grid->GenerateGrid(Tiles);
    const float BallScale = BallSize / BallSizeDefault;
    for (FTile& Tile : Tiles)
    {
        if (Tile.bIsOutOfRightEdge) continue;
        const FTransform SpawnTransform{FRotator::ZeroRotator, Tile.Location,  //
            FVector(BallScale, BallScale, BallScale)};
        ASSSphere* Ball = GetWorld()->SpawnActorDeferred<ASSSphere>(BallClass, SpawnTransform);        
        Ball->FinishSpawning(SpawnTransform);
        Tile.Ball = Ball;
    }
}

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

    TArray<AActor*> Array;
    UGameplayStatics::GetAllActorsWithTag(this, PlayerBallPositionMarkActorTag, Array);
    checkf(Array.Num() != 0, TEXT("No actor with tag %s"), *PlayerBallPositionMarkActorTag.ToString());
    PlayerBallPositionMarkActor = Array[0];

    APlayerController* PC = GetWorld()->GetFirstPlayerController<APlayerController>();
    ASSPawn* Pawn = Cast<ASSPawn>(PC->GetPawn());

    FVector PlayerBallXYLoc = PlayerBallPositionMarkActor->GetActorLocation();
    float BallScale = BallSize / BallSizeDefault;
    FTransform SpawnTransform{FRotator::ZeroRotator, FVector(PlayerBallXYLoc.X, PlayerBallXYLoc.Y, BallSize / 2.f),  //
        FVector(BallScale, BallScale, BallScale)};

    ASSSphere* RollBall = GetWorld()->SpawnActorDeferred<ASSSphere>(RollBallClass, SpawnTransform);
    
    RollBall->TurnIntoRollBall();
    RollBall->FinishSpawning(SpawnTransform);
    Pawn->SetRollBall(RollBall);
}

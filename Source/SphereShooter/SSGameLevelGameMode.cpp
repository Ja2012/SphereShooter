#include "SSGameLevelGameMode.h"
#include "SSPawn.h"
#include "SSPlayerController.h"
#include "SSSphere.h"

ASSGameLevelGameMode::ASSGameLevelGameMode() 
{

    DefaultPawnClass =  ASSPawn::StaticClass();
	PlayerControllerClass = ASSPlayerController::StaticClass();

}

void ASSGameLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (!GetWorld()) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController<APlayerController>();
    if (!PC) return;

    ASSPawn* Pawn = Cast<ASSPawn>(PC->GetPawn());
    if (!Pawn) return;

    // set location for RollBoll so it lay firmly on zero plane    
    //ASSSphere* RollBall = GetWorld()->SpawnActor<ASSSphere>(RollBallClass, //
    //    FTransform(FVector(PlayerBallLoc.X, PlayerBallLoc.Y, BallSize / 2.f)) //
    //);

    float BallScale = BallSize / BallSizeDefault;
    FTransform SpawnTransform { FRotator::ZeroRotator, FVector(PlayerBallLoc.X, PlayerBallLoc.Y, BallSize / 2.f), //
        FVector(BallScale, BallScale, BallScale)};

    //ASSSphere* RollBall = GetWorld()->SpawnActor<ASSSphere>(RollBallClass, SpawnTransform);
    ASSSphere* RollBall = GetWorld()->SpawnActorDeferred<ASSSphere>(RollBallClass, SpawnTransform);
    if (!RollBall) return;
    
    RollBall->TurnIntoRollBall();
    RollBall->FinishSpawning(SpawnTransform);
    





    Pawn->SetRollBall(RollBall);
}

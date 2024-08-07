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
    ASSSphere* RollBall = GetWorld()->SpawnActor<ASSSphere>(RollBallClass, //
        FTransform(FVector(PlayerBallLoc.X, PlayerBallLoc.Y, BallSize / 2.f)) //
    );
    if (!RollBall) return;

    Pawn->SetRollBall(RollBall);
}

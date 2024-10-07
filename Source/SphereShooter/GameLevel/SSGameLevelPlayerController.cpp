// LICENSE.md

#include "SSGameLevelPlayerController.h"
#include "UI/SSGameLevelHUD.h"
#include "SSPawn.h"
#include "SSGameLevelGameMode.h"

#include "Blueprint/UserWidget.h"

void ASSGameLevelPlayerController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
         
    FHitResult HitResult;
    GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);    
    if (!HitResult.bBlockingHit) return;

    const FRotator Rotator {0, (HitResult.Location - RollBallSpawnLoc).Rotation().Yaw, 0};    
    SSPawn->SetAimBeamRotation(Rotator);
}

void ASSGameLevelPlayerController::BeginPlay()
{
    Super::BeginPlay();

    PrimaryActorTick.bCanEverTick = true;

    bShowMouseCursor = true;
    FInputModeGameOnly InputMode;
    InputMode.SetConsumeCaptureMouseDown(false);
    SetInputMode(InputMode);

    SSGameMode = GetWorld()->GetAuthGameMode<ASSGameLevelGameMode>();
    RollBallSpawnLoc = SSGameMode->GetRollBallSpawn()->GetActorLocation();
    SSPawn = Cast<ASSPawn>(GetPawn());
}

// LICENSE.md

#include "SphereShooter/Public/Player/SsGameLevelPlayerController.h"
#include "SphereShooter/Public/UI/GameLevel/SsGameLevelHUD.h"
#include "SphereShooter/Public/Player/SsPawn.h"
#include "SphereShooter/Public/GameModes/SsGameLevelGameMode.h"

#include "Blueprint/UserWidget.h"

void ASsGameLevelPlayerController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
         
    FHitResult HitResult;
    GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);    
    if (!HitResult.bBlockingHit) return;

    const FRotator Rotator {0, (HitResult.Location - RollBallSpawnLoc).Rotation().Yaw, 0};    
    SSPawn->SetAimBeamRotation(Rotator);
}

void ASsGameLevelPlayerController::BeginPlay()
{
    Super::BeginPlay();

    PrimaryActorTick.bCanEverTick = true;

    bShowMouseCursor = true;
    FInputModeGameOnly InputMode;
    InputMode.SetConsumeCaptureMouseDown(false);
    SetInputMode(InputMode);

    SSGameMode = GetWorld()->GetAuthGameMode<ASSGameLevelGameMode>();
    RollBallSpawnLoc = SSGameMode->GetRollBallSpawn()->GetActorLocation();
    SSPawn = Cast<ASsPawn>(GetPawn());
}

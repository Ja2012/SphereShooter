// SPDX-License-Identifier: MIT

#include "Player/SsGameLevelPlayerController.h"
#include "UI/GameLevel/SsGameLevelHUD.h"
#include "Player/SsPawn.h"
#include "GameModes/SsGameLevelGameMode.h"

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

    SSGameMode = GetWorld()->GetAuthGameMode<ASsGameLevelGameMode>();
    RollBallSpawnLoc = SSGameMode->GetRollBallSpawn()->GetActorLocation();
    SSPawn = Cast<ASsPawn>(GetPawn());
}

// LICENSE.md


#include "SphereShooter/Public/Player/SsMainMenuPlayerController.h"

void ASsMainMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    
}

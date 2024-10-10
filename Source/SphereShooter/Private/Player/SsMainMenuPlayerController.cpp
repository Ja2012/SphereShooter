// LICENSE.md


#include "Player/SsMainMenuPlayerController.h"

void ASsMainMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    
}

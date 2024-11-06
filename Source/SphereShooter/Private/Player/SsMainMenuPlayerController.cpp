// SPDX-License-Identifier: MIT


#include "Player/SsMainMenuPlayerController.h"

void ASsMainMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    
}

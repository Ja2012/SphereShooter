// LICENSE.md


#include "SSMainMenuPlayerController.h"

void ASSMainMenuPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
    
}

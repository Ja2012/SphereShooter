// LICENSE.md

#include "SSGameLevelPlayerController.h"

void ASSGameLevelPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
}

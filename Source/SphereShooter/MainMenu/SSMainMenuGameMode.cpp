// LICENSE.md


#include "SSMainMenuGameMode.h"
#include "../UI/SSMainMenuHUD.h"
#include "SSMainMenuPlayerController.h"

ASSMainMenuGameMode::ASSMainMenuGameMode()
{
    HUDClass = ASSMainMenuHUD::StaticClass();
    PlayerControllerClass = ASSMainMenuPlayerController::StaticClass();
}
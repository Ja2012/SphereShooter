// LICENSE.md

#include "GameModes/SsMainMenuGameMode.h"

#include "UI/MainMenu/SsMainMenuHUD.h"
#include "Player/SsMainMenuPlayerController.h"

ASsMainMenuGameMode::ASsMainMenuGameMode()
{
    HUDClass = ASsMainMenuHUD::StaticClass();
    PlayerControllerClass = ASsMainMenuPlayerController::StaticClass();
}
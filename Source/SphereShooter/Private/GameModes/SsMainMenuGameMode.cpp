// LICENSE.md

#include "SphereShooter/Public/GameModes/SsMainMenuGameMode.h"

#include "SphereShooter/Public/UI/MainMenu/SsMainMenuHUD.h"
#include "SphereShooter/Public/Player/SsMainMenuPlayerController.h"

ASSMainMenuGameMode::ASSMainMenuGameMode()
{
    HUDClass = ASsMainMenuHUD::StaticClass();
    PlayerControllerClass = ASsMainMenuPlayerController::StaticClass();
}
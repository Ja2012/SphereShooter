// SPDX-License-Identifier: MIT

#include "GameModes/SsMainMenuGameMode.h"

#include "SsGameInstance.h"
#include "SsGameStateBase.h"
#include "Player/SsMainMenuPlayerController.h"
#include "UI/MainMenu/SsMainMenuHUD.h"
#include "Player/SsPlayerState.h"

ASsMainMenuGameMode::ASsMainMenuGameMode()
{
    GameStateClass = ASsGameStateBase::StaticClass();
    PlayerStateClass = ASsPlayerState::StaticClass();

    HUDClass = ASsMainMenuHUD::StaticClass();
    PlayerControllerClass = ASsMainMenuPlayerController::StaticClass();
}

void ASsMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameInstance = GetGameInstance<USsGameInstance>();
    MyGameState = GetGameState<ASsGameStateBase>();
    PlayerController = GetWorld()->GetFirstPlayerController<ASsMainMenuPlayerController>();
    PlayerState = PlayerController->GetPlayerState<ASsPlayerState>();
    HUD = PlayerController->GetHUD<ASsMainMenuHUD>();
}
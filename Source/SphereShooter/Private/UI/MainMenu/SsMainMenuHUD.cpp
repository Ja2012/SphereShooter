// SPDX-License-Identifier: MIT


#include "UI/MainMenu/SsMainMenuHUD.h"

#include "UI/MainMenu/SsMainMenuWidget.h"

void ASsMainMenuHUD::BeginPlay()
{
    Super::BeginPlay();
    if (MenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<USsMainMenuWidget>(GetWorld(), MenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }
}

// LICENSE.md


#include "UI/MainMenu/SsMainMenuHUD.h"

#include "UI/MainMenu/SsMainMenuWidget.h"

void ASsMainMenuHUD::BeginPlay() 
{
    Super::BeginPlay();
    if (MenuWidgetClass)
    {
        USsMainMenuWidget* MenuWidget = CreateWidget<USsMainMenuWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
        }
    }
}

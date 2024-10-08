// LICENSE.md


#include "SphereShooter/Public/UI/MainMenu/SsMainMenuHUD.h"

#include "SphereShooter/Public/UI/MainMenu/SsMainMenuWidget.h"

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

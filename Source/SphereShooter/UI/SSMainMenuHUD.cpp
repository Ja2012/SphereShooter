// LICENSE.md


#include "SSMainMenuHUD.h"
#include "SSMainMenuWidget.h"

void ASSMainMenuHUD::BeginPlay() 
{
    Super::BeginPlay();
    if (MenuWidgetClass)
    {
        USSMainMenuWidget* MenuWidget = CreateWidget<USSMainMenuWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
        }
    }
}

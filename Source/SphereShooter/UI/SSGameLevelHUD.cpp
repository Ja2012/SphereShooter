// LICENSE.md

#include "UI/SSGameLevelHUD.h"
#include "UI/SSGameLevelWidget.h"

// USSGameLevelWidget
void ASSGameLevelHUD::BeginPlay()
{
    Super::BeginPlay();
    if (GameLevelWidgetClass)
    {
        GameLevelWidget = CreateWidget<USSGameLevelWidget>(GetWorld(), GameLevelWidgetClass);
        if (GameLevelWidget)
        {
            GameLevelWidget->AddToViewport();
        }
    }
}
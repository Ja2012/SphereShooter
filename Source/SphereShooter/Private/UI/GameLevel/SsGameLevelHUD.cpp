// LICENSE.md

#include "SphereShooter/Public/UI/GameLevel/SsGameLevelHUD.h"
#include "SphereShooter/Public/UI/GameLevel/SsGameLevelWidget.h"

void ASsGameLevelHUD::ShowGameOverDialog()
{
    GameLevelWidget->ShowGameOverDialog();
}

// USsGameLevelWidget
void ASsGameLevelHUD::BeginPlay()
{
    Super::BeginPlay();
    if (GameLevelWidgetClass)
    {
        GameLevelWidget = CreateWidget<USsGameLevelWidget>(GetWorld(), GameLevelWidgetClass);
        if (GameLevelWidget)
        {
            GameLevelWidget->AddToViewport();
        }
    }
}
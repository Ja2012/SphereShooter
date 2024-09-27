// LICENSE.md


#include "SSMainMenuWidget.h"
#include "Components/Button.h"

void USSMainMenuWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &USSMainMenuWidget::OnStart);
    }
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &USSMainMenuWidget::OnExit);
    }
}

void USSMainMenuWidget::OnStart()
{
    UE_LOG(LogTemp, Display, TEXT("OnStart"));
}

void USSMainMenuWidget::OnExit()
{
    UE_LOG(LogTemp, Display, TEXT("OnExit"));
}


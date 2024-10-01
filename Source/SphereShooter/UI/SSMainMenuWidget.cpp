// LICENSE.md

#include "SSMainMenuWidget.h"

#include "Auxiliary/SSLevelData.h"
#include "SSLevelItemWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


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
    InitLevelItems();
}

void USSMainMenuWidget::OnStart()
{
    USSGameInstance* SSGameInstance = GetSTUGameInstance();
    if (!SSGameInstance) return;

    UGameplayStatics::OpenLevel(this, SSGameInstance->GetStartupLevel()->LevelName);
}

void USSMainMenuWidget::OnExit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USSMainMenuWidget::InitLevelItems()
{
    USSGameInstance* SSGameInstance = GetSTUGameInstance();
    if (!SSGameInstance) return;

    checkf(SSGameInstance->GetLevels()->Num() != 0, TEXT("no levels"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    for (const FSSLevelData& LevelData : *SSGameInstance->GetLevels())
    {
        USSLevelItemWidget* LevelItemWidget = CreateWidget<USSLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USSMainMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if (!SSGameInstance->GetStartupLevel())
    {
        OnLevelSelected(SSGameInstance->GetLevels()->GetData());
    }
    else
    {
        OnLevelSelected(SSGameInstance->GetStartupLevel());
    }
}

void USSMainMenuWidget::OnLevelSelected(FSSLevelData* Data)
{
    USSGameInstance* SSGameInstance = GetSTUGameInstance();
    if (!SSGameInstance) return;

    SSGameInstance->SetStartupLevel(Data);

    for (USSLevelItemWidget* LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const bool IsSelected = Data->LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

USSGameInstance* USSMainMenuWidget::GetSTUGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USSGameInstance>();
}

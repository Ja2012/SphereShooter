// LICENSE.md

#include "UI/MainMenu/SsMainMenuWidget.h"

#include "CoreTypes/SsLevelData.h"
#include "UI/MainMenu/SsLevelItemWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


void USsMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &USsMainMenuWidget::OnStart);
    }
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &USsMainMenuWidget::OnExit);
    }
    InitLevelItems();
}

void USsMainMenuWidget::OnStart()
{
    USsGameInstance* SSGameInstance = GetSTUGameInstance();
    if (!SSGameInstance) return;

    UGameplayStatics::OpenLevel(this, SSGameInstance->GetStartupLevel()->LevelName);
}

void USsMainMenuWidget::OnExit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USsMainMenuWidget::InitLevelItems()
{
    USsGameInstance* SSGameInstance = GetSTUGameInstance();
    if (!SSGameInstance) return;

    checkf(SSGameInstance->GetLevels()->Num() != 0, TEXT("no levels"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    for (const FSsLevelData& LevelData : *SSGameInstance->GetLevels())
    {
        USsLevelItemWidget* LevelItemWidget = CreateWidget<USsLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USsMainMenuWidget::OnLevelSelected);

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

void USsMainMenuWidget::OnLevelSelected(FSsLevelData* Data)
{
    USsGameInstance* SSGameInstance = GetSTUGameInstance();
    if (!SSGameInstance) return;

    SSGameInstance->SetStartupLevel(Data);

    for (USsLevelItemWidget* LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const bool IsSelected = Data->LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

USsGameInstance* USsMainMenuWidget::GetSTUGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USsGameInstance>();
}
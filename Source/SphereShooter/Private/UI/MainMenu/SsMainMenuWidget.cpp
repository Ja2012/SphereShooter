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
    USsGameInstance* SSGameInstance = GetGameInstance();
    if (!SSGameInstance) return;

    UGameplayStatics::OpenLevel(this, SSGameInstance->GetStartupLevel()->LevelName);
}

void USsMainMenuWidget::OnExit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USsMainMenuWidget::InitLevelItems()
{
    USsGameInstance* SsGameInstance = GetGameInstance();
    if (!SsGameInstance) return;

    checkf(SsGameInstance->GetLevels()->Num() != 0, TEXT("no levels"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    for (const FSsLevelData& LevelData : *SsGameInstance->GetLevels())
    {
        USsLevelItemWidget* LevelItemWidget = CreateWidget<USsLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USsMainMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if (!SsGameInstance->GetStartupLevel())
    {
        OnLevelSelected(SsGameInstance->GetLevels()->GetData());
    }
    else
    {
        OnLevelSelected(SsGameInstance->GetStartupLevel());
    }
}

void USsMainMenuWidget::OnLevelSelected(FSsLevelData* Data)
{
    USsGameInstance* SsGameInstance = GetGameInstance();
    if (!SsGameInstance) return;

    SsGameInstance->SetStartupLevel(Data);

    for (USsLevelItemWidget* LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const bool IsSelected = Data->LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

USsGameInstance* USsMainMenuWidget::GetGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USsGameInstance>();
}

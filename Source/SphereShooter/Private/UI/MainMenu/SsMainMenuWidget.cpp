// LICENSE.md

#include "UI/MainMenu/SsMainMenuWidget.h"

#include "CoreTypes/SsLevelData.h"
#include "UI/MainMenu/SsLevelItemWidget.h"
#include "CoreTypes/SsSaveGame.h"
#include "UI/MainMenu/SsPlayerItemWidget.h"
#include "UI/MainMenu/SsAddPlayerWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
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
    if (AddPlayerWidget)
    {
        AddPlayerWidget->OnPlayerAdd.AddUObject(this, &USsMainMenuWidget::OnAddPlayer);
    }

    InitLevelItems();
    InitPlayerItems();
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

void USsMainMenuWidget::InitPlayerItems()
{
    USsGameInstance* SsGameInstance = GetGameInstance();
    if (!SsGameInstance) return;
    USsSaveGame* Save = SsGameInstance->GetSaveGameInstance();
    if (!Save) return;
    checkf(!Save->GetPlayers().IsEmpty(), TEXT("no players"));

    PlayerItemsBox->ClearChildren();

    Save->SortPlayersByScore();

    for (const TPair<FString, FSsPlayerData>& PlayerDataPair : Save->GetPlayers())
    {
        USsPlayerItemWidget* Item = CreateWidget<USsPlayerItemWidget>(GetWorld(), PlayerItemWidgetClass);
        if (!Item) continue;

        Item->SetPlayerData(PlayerDataPair.Key, &PlayerDataPair.Value);
        Item->OnPlayerItemSelected.AddUObject(this, &USsMainMenuWidget::OnPlayerSelected);

        PlayerItemsBox->AddChild(Item);
        PlayerItemWidgets.Add(Item);
    }

    OnPlayerSelected(Save->GetLastPlayerName());
}

void USsMainMenuWidget::OnLevelSelected(FSsLevelData* Data)
{
    USsGameInstance* SsGameInstance = GetGameInstance();
    if (!SsGameInstance) return;

    SsGameInstance->SetStartupLevel(Data);
    SsGameInstance->GetSaveGameInstance()->GetLastPlayer()->LevelName = Data->LevelName;

    for (USsLevelItemWidget* LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const bool IsSelected = Data->LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

void USsMainMenuWidget::OnPlayerSelected(FString PlayerName)
{
    USsGameInstance* SsGameInstance = GetGameInstance();
    if (!SsGameInstance) return;
    USsSaveGame* Save = SsGameInstance->GetSaveGameInstance();
    if (!Save) return;

    checkf(Save->GetPlayers().Contains(PlayerName), TEXT("no player name %s"), *PlayerName);

    for (USsPlayerItemWidget* PlayerItemWidget : PlayerItemWidgets)
    {
        const bool bIsSelected = PlayerName == PlayerItemWidget->GetPlayerName();
        PlayerItemWidget->SetSelected(bIsSelected);
    }

    Save->SetLastPlayerName(PlayerName);
}

void USsMainMenuWidget::OnAddPlayer(FString PlayerName)
{
    USsGameInstance* SsGameInstance = GetGameInstance();
    if (!SsGameInstance) return;
    USsSaveGame* Save = SsGameInstance->GetSaveGameInstance();
    if (!Save) return;

    Save->AddPlayer(PlayerName, FSsPlayerData{0, FDateTime::Now(), SsGameInstance->GetStartupLevel()->LevelName});
    InitPlayerItems();
    OnPlayerSelected(PlayerName);
}

USsGameInstance* USsMainMenuWidget::GetGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USsGameInstance>();
}

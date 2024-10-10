#include "SphereShooter/Public/SsGameInstance.h"
#include "SphereShooter/Public/CoreTypes/SsSaveGame.h"

#include "Kismet/GameplayStatics.h"

void USsGameInstance::Init()
{
    Super::Init();

    if (SaveGameInstance = Cast<USsSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultGameSlotName, DefaultUserIndex)))
    {
    }
    else
    {
        SaveGameInstance = Cast<USsSaveGame>(UGameplayStatics::CreateSaveGameObject(USsSaveGame::StaticClass()));
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, DefaultGameSlotName, DefaultUserIndex);
    }

    if (SaveGameInstance->Players.IsEmpty())
    {
        SaveGameInstance->Players.Emplace(DefaultPlayerName, 0, FDateTime::Now(), GameLevelsData[0].LevelName);
    }
}

void USsGameInstance::SaveGame(USsSaveGame* NewSaveGameInstance)
{
    UGameplayStatics::SaveGameToSlot(NewSaveGameInstance, DefaultGameSlotName, DefaultUserIndex);
    SaveGameInstance = NewSaveGameInstance;
}

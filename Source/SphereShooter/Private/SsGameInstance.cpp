#include "SsGameInstance.h"
#include "CoreTypes/SsSaveGame.h"

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

    if (SaveGameInstance->GetPlayers().IsEmpty())
    {
        SaveGameInstance->GetPlayers().Emplace(DefaultPlayerName, FSsPlayerData(0, FDateTime::Now(), GameLevelsData[0].LevelName));

        // TODO debug
        //for (int i = 0; i < 100; ++i)
        //{
        //    SaveGameInstance->GetPlayers().Emplace(FString::FromInt(i), FSsPlayerData(0, FDateTime::Now(), GameLevelsData[0].LevelName));
        //}

        SaveGameInstance->SetLastPlayerName(DefaultPlayerName);
    }
}

void USsGameInstance::SaveGame(USsSaveGame* NewSaveGameInstance)
{
    UGameplayStatics::SaveGameToSlot(NewSaveGameInstance, DefaultGameSlotName, DefaultUserIndex);
    SaveGameInstance = NewSaveGameInstance;
}

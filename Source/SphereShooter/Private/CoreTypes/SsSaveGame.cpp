// SPDX-License-Identifier: MIT

#include "CoreTypes/SsSaveGame.h"

#include "Player/SsPlayerState.h"

void FSsPlayerData::UpdateFromPlayerState(const ASsPlayerState* PlayerState)
{
    MaxScore = PlayerState->GetMaxScore();
    WhenMaxScore = PlayerState->GetWhenMaxScore();
}

void USsSaveGame::AddPlayer(const FString PlayerName, const FSsPlayerData& PlayerData)
{
    Players.Add(PlayerName, PlayerData);
}

void USsSaveGame::SortPlayersByScore() 
{
    Players.ValueSort([](const FSsPlayerData& A, const FSsPlayerData& B) { return A.MaxScore > B.MaxScore; });
}

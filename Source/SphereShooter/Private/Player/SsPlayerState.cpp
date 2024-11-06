// SPDX-License-Identifier: MIT


#include "Player/SsPlayerState.h"
#include "CoreTypes/SsSaveGame.h"

void ASsPlayerState::UpdateFromPlayerData(FSsPlayerData* PlayerData)
{
    Score = 0;
    MaxScore = PlayerData->MaxScore;
    WhenMaxScore = PlayerData->WhenMaxScore;
}

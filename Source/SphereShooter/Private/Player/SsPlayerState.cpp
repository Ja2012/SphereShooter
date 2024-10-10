// LICENSE.md


#include "SphereShooter/Public/Player/SsPlayerState.h"
#include "SphereShooter/Public/CoreTypes/SsSaveGame.h"

void ASsPlayerState::UpdateFromPlayerData(FSsPlayerData* PlayerData)
{
    Score = 0;
    MaxScore = PlayerData->MaxScore;
    WhenMaxScore = PlayerData->WhenMaxScore;
}

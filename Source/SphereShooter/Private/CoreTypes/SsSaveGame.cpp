// LICENSE.md

#include "SphereShooter/Public/Player/SsPlayerState.h"

#include "CoreTypes/SsSaveGame.h"

void FSsPlayerData::UpdateFromPlayerState(const ASsPlayerState* PlayerState)
{
    MaxScore = PlayerState->GetMaxScore();
    WhenMaxScore = PlayerState->GetWhenMaxScore();
}

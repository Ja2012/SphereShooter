// LICENSE.md

#include "CoreTypes/SsSaveGame.h"

#include "Player/SsPlayerState.h"


void FSsPlayerData::UpdateFromPlayerState(const ASsPlayerState* PlayerState)
{
    MaxScore = PlayerState->GetMaxScore();
    WhenMaxScore = PlayerState->GetWhenMaxScore();
}

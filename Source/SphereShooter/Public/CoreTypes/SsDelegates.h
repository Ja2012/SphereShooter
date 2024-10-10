// LICENSE.md

#pragma once

#include "SphereShooter/Public/CoreTypes/SsLevelData.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, FSsLevelData*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnConfirmClickedSignature, const bool);
DECLARE_MULTICAST_DELEGATE(FOnExitLevelClickedSignature);
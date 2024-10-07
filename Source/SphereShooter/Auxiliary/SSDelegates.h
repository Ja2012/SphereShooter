// LICENSE.md

#pragma once

#include "SSLevelData.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, FSSLevelData*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnConfirmClickedSignature, const bool);
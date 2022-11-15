#pragma once
//////////////////////////////////////////////////////////////////////
// Dark Forces
// Handles the Phase Two Dark Trooper AI.
//////////////////////////////////////////////////////////////////////
#include <TFE_System/types.h>
#include <TFE_DarkForces/logic.h>
#include "actor.h"

namespace TFE_DarkForces
{
	void bobaFett_exit();
	Logic* bobaFett_setup(SecObject* obj, LogicSetupFunc* setupFunc);
}  // namespace TFE_DarkForces
#pragma once
#include "BaseObject.h"
class UpdatingObject :
	public BaseObject
{
	virtual void Tick(GameData* _GD) = 0;
};


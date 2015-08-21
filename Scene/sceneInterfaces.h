#pragma once

#include "../stdafx.h"

class IGuiButtonEvents
{
public:
	virtual void OnButtonClicked(int id) = 0;
};
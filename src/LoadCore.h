#pragma once

#include "IAppState.h"

namespace appstate
{

class LoadCore :
	public IAppState
{
public:
	LoadCore(void);
	~LoadCore(void);

	void action();
};

} //namespace appstate
#pragma once
#include "RenderEngine.h"
class RenderSoftware :
	public RenderEngine
{
public:
	RenderSoftware();
	virtual ~RenderSoftware();
	virtual void Render(World & world);
};


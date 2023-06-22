#ifndef CAMERACONTROLLER_SYSTEM__H
#define CAMERACONTROLLER_SYSTEM__H

#include "Flood/ECS/Core/System.h"

namespace Flood
{
	class CameraController : public ECS::System
	{
	public:
		virtual void OnUpdate(float dt) override;
	};
}

#endif // define 

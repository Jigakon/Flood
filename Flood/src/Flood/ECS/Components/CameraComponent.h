#ifndef CAMERACOMPONENT__H
#define CAMERACOMPONENT__H

#include "Flood/Scene/SceneCamera.h"

namespace Flood
{
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};
}

#endif // ! CAMERACOMPONENT__H
